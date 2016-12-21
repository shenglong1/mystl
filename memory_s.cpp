#include "stdafx.h"
#include "include_s.h"
#include "memory_s.h"

namespace safe {
// destroy
inline void destroy( char*, char* ) {
	return; // doesn't need to destruct.
}
inline void destroy( wchar_t*, wchar_t* ) {
	return; // doesn't need to destruct.
}
// uninitialized_copy
template<typename InputIterator, typename ForwardIterator> // interface func.
ForwardIterator uninitialized_copy( InputIterator first, InputIterator last, ForwardIterator result ) {
	return _uninitialized_copy( first, last, result, safe::_value_type( first ) );
}
template<typename InputIterator, typename ForwardIterator, typename T> // get type traits.
ForwardIterator _uninitialized_copy( InputIterator first, InputIterator last, ForwardIterator result, T& ) {
	typedef typename safe::_type_traits<T>::is_POD_type is_POD;
	return _uninitialized_copy_aux( first, last, result, is_POD( ) );
}
template<typename InputIterator, typename ForwardIterator> // implement overload.
ForwardIterator _uninitialized_copy_aux( InputIterator first, InputIterator last, ForwardIterator result, _true_type ) {
	return copy( first, last, result );
}
template<typename InputIterator, typename ForwardIterator> // implement overload.
ForwardIterator _uninitialized_copy_aux( InputIterator first, InputIterator last, ForwardIterator result, _false_type ) {
	ForwardIterator cur = result;
	for ( ; first != last; ++first, ++cur )
		construct( &*cur, *first );
	return cur;
}
// uninitialized_fill
template<typename ForwardIterator, typename T>
void uninitialized_fill( ForwardIterator first, ForwardIterator last, const T& value ) {
	_uninitialized_fill( first, last, value, safe::_value_type( first ) );
}
template<typename ForwardIterator, typename T, typename T1>
void _uninitialized_fill( ForwardIterator first, ForwardIterator last, const T& value, T1& ) {
	typedef typename safe::_type_traits<T1>::is_POD_type is_POD;
	_uninitialized_fill_aux( first, last, value, is_POD( ) );
}
template<typename ForwardIterator, typename T>
void _uninitialized_fill_aux( ForwardIterator first, ForwardIterator last, const T& value, _true_type ) {
	fill( first, last, value );
}
template<typename ForwardIterator, typename T>
void _uninitialized_fill_aux( ForwardIterator first, ForwardIterator last, const T& value, _false_type ) {
	ForwardIterator cur = first;
	for ( ; first != last; ++first, ++cur )
		construct( &*cur, value );
}
// uninitialized_fill_n
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n( ForwardIterator first, Size n, const T& value ) {
	return _uninitialized_fill_n( first, n, value, safe::_value_type( first ) );
}
template<typename ForwardIterator, typename Size, typename T, typename T1>
ForwardIterator _uninitialized_fill_n( ForwardIterator first, Size n, const T& value, T1& ) {
	typedef typename safe::_type_traits<T1>::is_POD_type is_POD;
	return _uninitialized_fill_n_aux( first, n, value, is_POD( ) );
}
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator _uninitialized_fill_n_aux( ForwardIterator first, Size n, const T& value, _true_type ) {
	return fill_n( first, n, value );
}
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator _uninitialized_fill_n_aux( ForwardIterator first, Size n, const T& value, _false_type ) {
	ForwardIterator cur = first;
	for ( ; n > 0; --n, ++cur )
		construct( &*cur, value );
	return cur;
}
// simple_allocator.
template<typename T, typename Alloc>
T* simple_allocator<T, Alloc>::allocate( std::size_t n ) {
	return 0 == n ? 0 : ( T* ) Alloc::allocate( n * sizeof( T ) );
}
template<typename T, typename Alloc>
T* simple_allocator<T, Alloc>::allocate( void ) {
	return ( T* ) Alloc::allocate( sizeof( T ) );
}
template<typename T, typename Alloc>
void simple_allocator<T, Alloc>::deallocate( T* p, std::size_t n ) {
	if ( n != 0 )
		Alloc::deallocate( p, n * sizeof( T ) );
}
template<typename T, typename Alloc>
void simple_allocator<T, Alloc>::deallocate( void ) {
	Alloc::deallocate( p, sizeof( T ) );
}
// definition of class one, directly allocate.
template<int inst>
void* _malloc_alloc_template<inst>::allocate( std::size_t n ) {
	void* result = std::malloc( n );
	if ( result == 0 )
		result = oom_malloc( n );
	return result;
}
template<int inst>
void _malloc_alloc_template<inst>::deallocate( void* p, std::size_t /* = n */ ) {
	std::free( p );
}
template<int inst>
void* _malloc_alloc_template<inst>::reallocate( void* p, std::size_t old_sz, std::size_t new_sz ) {
	void* result = std::realloc( p, new_sz );
	if ( result == 0 ) 
		result = oom_realloc( p, new_sz );
	return result;
}
template<int inst>
void* _malloc_alloc_template<inst>::set_malloc_handler( void ( *f )( ) ) {
	// reserve new handler, return old handler.
	void ( *old )( ) = _malloc_alloc_oom_handler;
	_malloc_alloc_oom_handler = f;
	return old;
}
template<int inst>
void* _malloc_alloc_template<inst>::oom_malloc( std::size_t n ) {
	void ( * my_malloc_handler ) ( );
	void* result;
	for ( ; ; ) {
		my_malloc_handler = _malloc_alloc_oom_handler;
		if ( 0 == my_malloc_handler ) // no handler.
			_THROW_BAD_ALLOC; 
		( *my_malloc_handler )( );

		result = std::malloc( n );
		if ( result != 0 )
			return result;
	}
}
template<int inst>
void* _malloc_alloc_template<inst>::oom_realloc( void* p, std::size_t n ) {
	void ( *my_malloc_handler )( );
	void* result;
	for ( ; ; ) {
		my_malloc_handler = _malloc_alloc_oom_handler;
		if ( 0 == my_malloc_handler ) // no handler.
			_THROW_BAD_ALLOC; 
		( *my_malloc_handler )( );

		result = std::realloc( p, n );
		if ( result != 0 )
			return result;
	}
}
template<int inst>
void ( * _malloc_alloc_template<inst>::_malloc_alloc_oom_handler )( ) = 0;
// definition of class two, list allocate.
template<bool threads, int inst>
char* _default_alloc_template<threads, inst>::start_free = 0;
template<bool threads, int inst>
char* _default_alloc_template<threads, inst>::end_free = 0;
template<bool threads, int inst>
std::size_t _default_alloc_template<threads, inst>::heap_size = 0;
template<bool threads, int inst>
typename _default_alloc_template<threads, inst>::node* 
	volatile _default_alloc_template<threads, inst>::free_list[ _NFREELISTS ] = { 0 };

template<bool threads, int inst>
void* _default_alloc_template<threads, inst>::allocate( std::size_t n ) {
	node* volatile my_free_list;
	node* result;
	if ( n > ( std::size_t )_MAX_BYTES ) {
		return malloc_alloc::allocate( n );
	}
	my_free_list = free_list + FREELIST_INDEX( n );
	result = *my_free_list;	// ?
	if ( result == 0 ) {
		void* r = refill( ROUND_UP( n ) );
		return r;
	}
	*my_free_list = result->free_list_link;
	return result;
}
template<bool threads, int inst>
void _default_alloc_template<threads, inst>::deallocate( void* p, std::size_t n ) {
	node* q = ( node* )p;
	node* volatile *my_free_list;
	if ( n > ( std::size_t )_MAX_BYTES ) {
		malloc_alloc::deallocate( p, n );
		return;
	}
	my_free_list = free_list + FREELIST_INDEX( n );
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}
template<bool threads, int inst>
void* _default_alloc_template<threads, inst>::reallocate( void* p, std::size_t old_sz, std::size_t new_sz ) {
	return 0;
}
template<bool threads, int inst>
void* _default_alloc_template<threads, inst>::refill( std::size_t n ) {
	int nobjs = 20;
	char* chunk = chunk_alloc( n, nobjs );
	node* volatile * my_free_list = 0;
	node *result = 0, *currrent_obj = 0, *next_obj = 0;
	int i = 0;
	if ( nobjs == 1 ) return chunk;
	my_free_list = free_list + FREELIST_INDEX( n );
	result = ( node* )chunk;
	*my_free_list = next_obj = ( node* )( chunk + n );
	for ( i = 1; ; i++ ) {
		currrent_obj = next_obj;
		next_obj = ( node* )( ( char* )next_obj + n );
		if ( nobjs - 1 == i ) {
			currrent_obj->free_list_link = 0;
			break;
		} else {
			currrent_obj->free_list_link = next_obj;
		}
	}
	return result;
}
template<bool threads, int inst>
char* _default_alloc_template<threads, inst>::chunk_alloc( std::size_t size, int& nobjs ) {
	char* result = 0;
	std::size_t total_bytes = size * nobjs;
	std::size_t bytes_left = end_free - start_free;
	if ( bytes_left >= total_bytes ) {
		result = start_free;
		start_free += total_bytes;
		return result;
	} else if { bytes_left >= size } {
		nobjs = bytes_left / size;
		total_bytes = size * nobjs;
		result = start_free;
		start_free += total_bytes;
		return result;
	} else {
		std::size_t bytes_to_get = 2 * total_bytes + ROUND_UP( heap_size >> 4 );
		if ( bytes_left > 0 ) {
			node* volatile* my_free_list = free_list + FREELIST_INDEX( bytes_left );
			( ( node* )start_free )->free_list_link = *my_free_list;
			*my_free_list = ( node* )start_free;
		}
		start_free = ( char* )malloc( bytes_to_get );
		if ( 0 == start_free ) {
			int i = 0;
			node* volatile *my_free_list, *p;
			for ( i = size; i <= _MAX_BYTES; i += _ALIGN ) {
				my_free_list = free_list + FREELIST_INDEX( i );
				p = *my_free_list;
				if ( 0 != p ) {
					*my_free_list = p->free_list_link;
					start_free = ( char* )p;
					end_free = start_free + i;
					return chunk_alloc( size, nobjs );
				}
			}
			end_free = 0;
			start_free = ( char* )safe::malloc_alloc::allocate( bytes_to_get ); // class two allocator.
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return chunk_alloc( size, nobjs );
	}
}


} // namespace safe.