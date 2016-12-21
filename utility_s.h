/**********************************************************
 * File£º utility_s.h
 *
 * Author£º shenglong (shenglong@vip.126.com)
 *
 * Description£ºcontains simple auto_ptr class, simple pair class and shared_ptr class.
 *
 *	State : handling.
 *				
 * 
 * 
 * 
 */
// state : handling....

#ifndef UTILITY_S_H
#define UTILITY_S_H
#include "include_s.h"
#include "memory_s.h" // for construct/destroy.
#include "iterator_s.h" // for _type_trait.

namespace safe {
// auto_ptr.
template<typename T>
class auto_ptr {
public:
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
public:
	explicit auto_ptr( pointer x ) : p( x ) { }
	~auto_ptr() { 
		safe::destroy( p ); 
		// delete variable in stack or heap ?
	}
private:
	// default constructor.
	auto_ptr( );
	auto_ptr( const auto_ptr& x );
private:
	T* p;
};
// shared_ptr.
// pair<>.
template<typename First, typename Second>
class pair {
public:
	explicit pair( const First& x, const Second& y ) : first( x ), second( y ) { }
private:
	pair( );
	pair( const pair& x );
private:
	First first;
	Second second;
};



} // namespace safe.
#endif // UTILITY_S_H