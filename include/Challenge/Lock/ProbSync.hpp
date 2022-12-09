#ifndef CHLLANGE_LOCK_PROB_SYNC_HPP_
#define CHLLANGE_LOCK_PROB_SYNC_HPP_

#include <atomic>
#include <Challenge/Lock/SyncObject.hpp>

namespace Challenge {

// A class that implements the algorithm in the challange statement.
class ProbSync final : public SyncObject<ProbSync>
{
public:
    ProbSync() noexcept(false);

    void enter();
    void leave();

private:
    size_t _id;
};

}; // namespace Challenge

#endif // CHLLANGE_LOCK_PROB_SYNC_HPP_
