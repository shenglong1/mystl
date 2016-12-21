/**********************************************************
 * File£º memory_s.h
 *
 * Author£º shenglong (shenglong@vip.126.com)
 *
 * Description£ºthis file contains classes and functions for allocating memory.
 *		include : allocator interface :	simple_allocator<alloc>.
 *					  allocator imp :	normal allocator.
 *					  						_malloc_alloc_template.
 * 					  						_default_alloc_template.
 * 					  global functions :	allocate/deallocate.
 * 					  							construct/destroy.
 *												uninitialized_x.
 *
 * State : Completed.
 *
 *
 */					 						
#ifndef MEMORY_S_H
#define MEMORY_S_H
#include "include_s.h"
#include "iterator_s.h"  // for _true_type, _false_type, _type_traits.

namespace safe {
// global functions.
// global allocate for class allocator.
template<typename T>
inline T* allocate( std::ptrdiff_t size, T* ) {
	set_new_handler( 0 );
	T* tmp = ( T* )( ::operator new( ( size_t )( size * sizeof( T ) ) ) );
	if( tmp == 0 ) {
		std::cerr << "out of memory." << std::endl;
		exit( 1 );
	}
	return tmp;
}
template<typename T>
inline void deallocate( T* buffer ) {
	::operator delete( buffer );
}
// global constructor.
template<typename T1, typename T2>
inline void construct( T1* p, const T2& value ) {
	new( p ) T1( value );
}
inline void destroy( char*, char* );
inline void destroy( wchar_t*, wchar_t* );
template<typename T>
inline void destroy( T* p ) {
	p->~T( );
}
template<typename ForwardIterator>
inline void destroy( ForwardIterator first, ForwardIterator last ) {
	_destroy( first, last, safe::_value_type( first ) );
}
template<typename ForwardIterator, typename T>
inline void _destroy( ForwardIterator first, ForwardIterator last, T& ) {
	typedef typename safe::_type_trait<T>::has_trivial_destructor trivial_destructor;
	_destroy_aux( first, last, trivial_destructor( ) );
}
template<typename ForwardIterator>
inline void _destroy_aux( ForwardIterator first, ForwardIterator last, _true_type ) {
	// directly destroy.
	// do nothing.
}
template<typename ForwardIterator>
inline void _destroy_aux( ForwardIterator first, ForwardIterator last, _false_type ) {
	// generic destroy.
	for( ; first < last; first++ )
		destroy( &*first );
}

template<typename InputIterator, typename ForwardIterator> // interface func.
ForwardIterator uninitialized_copy( InputIterator first, InputIterator last, ForwardIterator result );
template<typename InputIterator, typename ForwardIterator, typename T> // get type traits.
ForwardIterator _uninitialized_copy( InputIterator first, InputIterator last, ForwardIterator result, T& );
template<typename InputIterator, typename ForwardIterator> // implement overload.
ForwardIterator _uninitialized_copy_aux( InputIterator first, InputIterator last, ForwardIterator result, _true_type );
template<typename InputIterator, typename ForwardIterator> // implement overload.
ForwardIterator _uninitialized_copy_aux( InputIterator first, InputIterator last, ForwardIterator result, _false_type );

template<typename ForwardIterator, typename T>
void uninitialized_fill( ForwardIterator first, ForwardIterator last, const T& value );
template<typename ForwardIterator, typename T, typename T1>
void _uninitialized_fill( ForwardIterator first, ForwardIterator last, const T& value, T1& );
template<typename ForwardIterator, typename T>
void _uninitialized_fill_aux( ForwardIterator first, ForwardIterator last, const T& value, _true_type );
template<typename ForwardIterator, typename T>
void _uninitialized_fill_aux( ForwardIterator first, ForwardIterator last, const T& value, _false_type );

template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n( ForwardIterator first, Size n, const T& value );
template<typename ForwardIterator, typename Size, typename T, typename T1>
ForwardIterator _uninitialized_fill_n( ForwardIterator first, Size n, const T& value, T1& );
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator _uninitialized_fill_n_aux( ForwardIterator first, Size n, const T& value, _true_type );
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator _uninitialized_fill_n_aux( ForwardIterator first, Size n, const T& value, _false_type );
// allocator interface
template<typename T, typename Alloc>
class simple_allocator {
public:
	static T* allocate( std::size_t n ); // allocate n * sizeof( T ).
	static T* allocate( void );
	static void deallocate( T* p, std::size_t n );
	static void deallocate( void );
};
// standard allocator class.
// caution : all allocators doesn't know what type they're allocating , just size.
template<typename T>
class allocator {
public: 
	// type interface.
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
public:
	// constructor.
	allocator( ) { }
	allocator( const allocator<T>& ) { }
	template<typename U> 
	explicit allocator( const allocator<U>& ) { }
	~allocator( ) { }
public:
	// method interface.
	pointer allocate( size_type n ) { return safe::allocate( n, (pointer)0 ); } // allocate n.
	void deallocate( pointer p ) { safe::deallocate( p ); }
	pointer address( reference x ) { return ( pointer )&x; }
	const_pointer const_address( const_reference x ) { return ( const_pointer )&x; }
	size_type init_page_size( ) {
		return max( size_type( 1 ), size_type( 4096/sizeof( value_type ) ) );
	}
	size_type max_size( ) {
		return max( size_type( 1 ), size_type( UNIX_MAX/sizeof( value_type ) ) );
	}
};
// alloc classes.
template<int inst>
class _malloc_alloc_template;
template<bool threads, int inst>
class _default_alloc_template;

#define _NODE_ALLOCATOR_THREADS false
#ifdef _USE_MALLOC
typedef _malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else
typedef _default_alloc_template<_NODE_ALLOCATOR_THREADS, 0> alloc;
#endif

// class one, directly allocate.
#if 0
#include <new>
#	define _THROW_BAD_ALLOC throw bad_alloc;
#elif !defined( _THROW_BAD_ALLOC )
#	include <iostream>
#	define _THROW_BAD_ALLOC std::cerr << "out of memory!" < std::endl; exit( 1 );
#endif

template<int inst>
class _malloc_alloc_template {
public:
	static void* allocate( std::size_t n );
	static void deallocate( void* p, std::size_t = n );
	static void* reallocate( void* p, std::size_t old_sz, std::size_t new_sz );
	// func.
	static void ( *set_malloc_handler( void ( *f )( ) ) ); 
private:
	// implement func.
	// recycle malloc/realloc till success.
	static void* oom_malloc( std::size_t );
	static void* oom_realloc( void* p, std::size_t n );
	// func pointer var.
	static void ( *_malloc_alloc_oom_handler )( ); 
};
typedef _malloc_alloc_template<0> malloc_alloc;

// class two, list allocate.
template<bool threads, int inst>
class _default_alloc_template {
public:
	enum { _ALIGN = 8 };
	enum { _MAX_BYTES = 128 };
	enum { _NFREELISTS = _MAX_BYTES/_ALIGN }; // for class two.
	union obj {
		union obj* free_list_link;
		char client_data[ 1 ];
	};
	typedef obj node;
public:
	static void* allocate( std::size_t n );
	static void deallocate( void* p, std::size_t n );
	static void* reallocate( void* p, std::size_t old_sz, std::size_t new_sz );
private:
	static char* start_free;
	static char* end_free;
	static std::size_t heap_size;
	static node* volatile free_list[ _NFREELISTS ];
private:
	static std::size_t ROUND_UP( std::size_t bytes ) {
		return ( ( bytes + _ALIGN - 1 ) & ~( _ALIGN - 1 ) );
	}
	static std::size_t FREELIST_INDEX( std::size_t bytes ) {
		// get list offset from bytes.
		return ( bytes / ( _ALIGN - 1 ) + 1 );
	}
	static void* refill( std::size_t n );
	static char* chunk_alloc( std::size_t size, int& nobjs );
};


} // namespace safe.
#endif // MEMORY_S_H