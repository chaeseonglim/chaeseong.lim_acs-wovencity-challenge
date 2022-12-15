#ifndef CHLLANGE_LOCK_SYNC_OBJECT_HPP_
#define CHLLANGE_LOCK_SYNC_OBJECT_HPP_

namespace Challenge {

// A CRTP class that defines the methods for concurrent programming.
// Inherited classes need to associate with Lock.
template<typename T>
class SyncObject
{
public:
    SyncObject( const SyncObject& ) = delete; // no copy constructor
    SyncObject( SyncObject&& ) = delete; // no move constructor
    ~SyncObject() = default; // no virtual keyword

    SyncObject& operator=( const SyncObject& ) = delete; // no copy assignment
    SyncObject& operator=( SyncObject&& ) = delete; // no move assignment

    // CRTP entries
    // It is intended to provide interface templates without virtual functions.
    void lock() noexcept(true) {
        static_cast<T&>( *this ).enter();
    }
    void unlock() noexcept(true) {
        static_cast<T&>( *this ).leave();
    }

protected:
    SyncObject() = default; // hide the default constructor
};

}; // namespace Challenge

#endif // CHLLANGE_LOCK_SYNC_OBJECT_HPP_
