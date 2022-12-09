#include <atomic>
#include <stdexcept>
#include <Challenge/Lock/ProbSync.hpp>

namespace Challenge {

// A singleton class that contains the global variables for ProbSync.
// DISCLAIMER: I chose to use singleton pattern though not a big fan of it because
// I wanted to keep APIs rather simple in this challange.
class ProbSyncStore
{
public:
    static ProbSyncStore& GetInstance()
    {
        // Thread safety guranteed since c++11
        static ProbSyncStore inst;
        return inst;
    }

    size_t assignSlot() noexcept(false)
    {
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

ProbSync::ProbSync()
    : _id( ProbSyncStore::GetInstance().assignSlot() )
{}

void ProbSync::enter()
{
    auto& store = ProbSyncStore::GetInstance();
    auto& turn = store.getTurn();
    auto& flag = store.getFlag( _id );

    while( turn.load() != _id )
    {
        while( flag.load() ) {
            // no-op
        }
        turn.store( _id );
    }
}

void ProbSync::leave()
{
    auto& store = ProbSyncStore::GetInstance();
    auto& flag = store.getFlag( _id );

    flag.store( false );
}

}; // namespace Challenge
