#ifndef CHLLANGE_LOCK_LOCK_HPP_
#define CHLLANGE_LOCK_LOCK_HPP_

#include <type_traits>
#include <Challenge/Lock/SyncObject.hpp>

namespace Challenge {

// Inspired by https://stackoverflow.com/questions/257288/templated-check-for-the-existence-of-a-class-member-function
template <typename...>
using void_t = void;

// Primary template handles all types not supporting the operation.
template <typename, template <typename> class, typename = void_t<>>
struct detect : std::false_type {};

// Specialization recognizes/validates only types supporting the archetype.
template <typename T, template <typename> class Op>
struct detect<T, Op, void_t<Op<T>>> : std::true_type {};

// Check if T has required methods for Lock.
template<typename T>
using enter_t = decltype( std::declval<T&>().enter() );
template<typename T>
using leave_t = decltype( std::declval<T&>().leave() );
template <typename T>
constexpr bool is_syncable = ( detect<T, enter_t>{} ) && ( detect<T, leave_t>{} );

// The class which provides API to access their critical sections exclusively.
// It utilizes RAII pattern to do lock and unlock the associated sync object.
template <typename T>
class Lock final
{
public:
    static_assert( std::is_base_of_v<SyncObject<T>, T> );
    static_assert( is_syncable<T> );

    Lock() = delete;    // No default constructor
    Lock( const Lock& ) = delete; // No copy constructor
    Lock( Lock&& ) = delete; // No move constructor;
    explicit Lock( T& sync );
    ~Lock();

    Lock& operator=( const Lock& ) = delete; // No copy operator
    Lock& operator=( Lock&& ) = delete; // No move operator

private:
    T& _sync;
};

template <typename T>
Lock<T>::Lock( T& sync )
    : _sync( sync )
{
    _sync.lock();
}

template <typename T>
Lock<T>::~Lock()
{
    _sync.unlock();
}

}; // namespace Challenge

#endif // CHLLANGE_LOCK_LOCK_HPP_
