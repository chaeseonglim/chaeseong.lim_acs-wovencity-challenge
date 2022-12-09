#include <atomic>
#include <stdexcept>
#include <Challenge/Lock/PetersonSync.hpp>

namespace Challenge {

// A singleton class that contains the global variables for PetersonSync.
class PetersonSyncStore
{
public:
    static PetersonSyncStore& GetInstance()
    {
        // Thread safety guaranteed since C++11
        static PetersonSyncStore inst;
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
    inline std::atomic<size_t>& getTurn() {
        return _turn;
    }

private:
    static constexpr size_t _kMaxSlots = 2;
    std::atomic<bool> _flags[_kMaxSlots];
    std::atomic<size_t> _turn;

    std::atomic<size_t> _nextFlagIdx{}; // to assign flag slots automatically
};

PetersonSync::PetersonSync()
    : _id( PetersonSyncStore::GetInstance().assignSlot() )
{}

void PetersonSync::enter()
{
    // It is the Perterson algorithm with atomocity support.

    auto& store = PetersonSyncStore::GetInstance();
    auto& turn = store.getTurn();
    auto& myFlag = store.getFlag( _id );
    auto& otherFlag = store.getFlag( !_id );

    // Set my flag to signal you want to enter.
    myFlag.store( true );

    // Set turn to other to give them a chance first.
    turn.store( !_id );

    // Wait until you get a chance.
    while( otherFlag.load() && turn.load() != _id ) {
        // no-op
    }
}

void PetersonSync::leave()
{
    auto& store = PetersonSyncStore::GetInstance();
    auto& myFlag = store.getFlag( _id );

    // Set flag to false when leaving it.
    myFlag.store( false );
}

}; // namespace Challenge
