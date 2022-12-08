#ifndef CHLLANGE_LOCK_SYNC_OBJECT_HPP_
#define CHLLANGE_LOCK_SYNC_OBJECT_HPP_

namespace Challenge {

// A base class that implement syncObjects for concurrent programming.
// Inherited classes will be used along with Lock.
class SyncObject
{
protected:
    SyncObject() = default; // hide the default constructor

private:
    // Inherited classes are required to implement below two methods.
    // void lock();
    // void unlock();
};

}; // namespace Challenge

#endif // CHLLANGE_LOCK_SYNC_OBJECT_HPP_
