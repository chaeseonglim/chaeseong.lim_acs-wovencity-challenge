#include <atomic>
#include <stdexcept>
#include <Challenge/Lock/TestSetSync.hpp>

namespace Challenge {

// A singleton class that contains the global variables for TestSetSync.
class TestSetSyncStore
{
public:
    static TestSetSyncStore& GetInstance()
    {
        // Thread safety guaranteed since C++11
        static TestSetSyncStore inst;
        return inst;
    }

    size_t assignSlot() noexcept(false)
    {
        // It assigns the remaining slot to a new sync object automatically.
        auto nextSlot = _nextFlagIdx.fetch_add( 1, std::memory_order_relaxed );
        if( nextSlot >= _kMaxSlots ) {
            throw std::out_of_range( "Not enough slots for sync objects." );
        }
        return nextSlot;
    }

    inline std::atomic<bool>& getFlag( size_t idx ) {
        return _flags[idx];
    }
    inline std::atomic_flag& getLock() {
        return _lock;
    }
    static constexpr size_t getMaxSlots() {
        return _kMaxSlots;
    }

private:
    static constexpr size_t _kMaxSlots = 10;    // Limit to max 10 sync objects
    std::atomic<bool> _flags[_kMaxSlots];
    std::atomic_flag _lock;

    std::atomic<size_t> _nextFlagIdx{}; // to assign flag slots automatically
};

TestSetSync::TestSetSync()
    : _id( TestSetSyncStore::GetInstance().assignSlot() )
{}

void TestSetSync::enter()
{
    // It is an algorithm introduced as "Bounded-waiting Mutual Exclusion with
    // test_and_set" at many places but the origin is still unknown to me.

    auto& store = TestSetSyncStore::GetInstance();
    auto& lock = store.getLock();
    auto& myFlag = store.getFlag( _id );

    // Set my flag to signal that I want to enter.
    myFlag.store( true );

    // Wait until achieving the key (permission) to access with atomic
    // test_and_set() operation.
    bool key = true;
    while( myFlag.load() && key ) {
        key = lock.test_and_set();
    }

    // Set my flag to false to indicate that I'm in.
    myFlag.store( false );
}

void TestSetSync::leave()
{
    auto& store = TestSetSyncStore::GetInstance();
    constexpr auto kMaxSlots = TestSetSyncStore::getMaxSlots();
    auto& lock = store.getLock();

    // Find a suitable next ID which wait for their turn.
    size_t nextId = ( _id + 1 ) % kMaxSlots;
    while( ( nextId != _id ) && !store.getFlag( nextId ).load() ) {
        nextId = ( nextId + 1 ) % kMaxSlots;
    }

    if( nextId == _id ) {
        // Clear lock if there's no one waiting.
        lock.clear();
    }
    else {
        // Set the flag of next ID if there is.
        store.getFlag( nextId ).store( false );
    }
}

}; // namespace Challenge
