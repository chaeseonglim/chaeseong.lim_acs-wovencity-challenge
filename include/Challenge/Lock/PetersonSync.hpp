#ifndef CHLLANGE_LOCK_PETERSON_SYNC_HPP_
#define CHLLANGE_LOCK_PETERSON_SYNC_HPP_

#include <atomic>
#include <Challenge/Lock/SyncObject.hpp>

namespace Challenge {

// A class that implements the Peterson algorithm.
class PetersonSync final : public SyncObject<PetersonSync>
{
public:
    PetersonSync() noexcept(false);

    void enter();
    void leave();

private:
    size_t _id; // A slot idx taken by this sync object
};

}; // namespace Challenge

#endif // CHLLANGE_LOCK_PETERSON_SYNC_HPP_
