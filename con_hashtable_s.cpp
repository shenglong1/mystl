#include "stdafx.h"
#include "include_s.h"
#include "con_hashtable_s.h"
#include <algorithm> // for lower_bound.

namespace safe {
// define short name.
#ifndef HASH_TEMPLATE
#define HASH_TEMPLATE \
	template<typename Value, \
		typename Key, \
		typename HashFn, \
		typename ExtractKey, \
		typename EqualKey, \
		typename Alloc>
#define HASHTABLE \
	hashtable<Value, \
		Key, HashFn, \
		ExtractKey, \
		EqualKey, Alloc> 
#endif // HASH_TEMPLATE.

// function interface.
HASH_TEMPLATE
	typename HASHTABLE::size_type HASHTABLE::bucket_count( ) {
		return buckets.size( );
}
HASH_TEMPLATE
	typename HASHTABLE::iterator HASHTABLE::insert_equal( const value_type& obj ) {
		resize( num_elements + 1 );
		return insert_equal_noresize( obj );
}
HASH_TEMPLATE
	typename HASHTABLE::iterator HASHTABLE::insert_equal_noresize( const value_type& obj ) {
		const size_type n = bkt_num( obj );
		node* first = buckets[ n ];
		for( node* cur = first; cur; cur = cur->next ) {
			if ( equal( get_key( cur->value ), get_key( obj ) ) ) {
				// equal, insert one after cur.
				node* tmp = new_node( obj );
				tmp->next = cur->next;
				cur->next = tmp;
				++num_elements;
				return iterator( tmp, obj );
			}
		}
		// insert before place.
		node* tmp = new_node( obj );
		tmp->next = first;
		buckets[ n ] = tmp;
		++num_elements;
		return iterator( tmp, obj );
}
HASH_TEMPLATE
	safe::pair<typename HASHTABLE::iterator, bool> HASHTABLE::insert_unique( const value_type& obj ) {
		resize( num_elements + 1 );
		return insert_unique_noresize( obj );
}
HASH_TEMPLATE
	safe::pair<typename HASHTABLE::iterator, bool> HASHTABLE::insert_unique_noresize( const value_type& obj ) {
		const size_type n = bkt_num( obj );
		node* first = bucket[ n ];
		for ( node* cur = first; cur; cur = cur->next ) {
			if ( equal( get_key( cur->value ), get_key( obj ) ) )
				return safe::pair<HASHTABLE::iterator, bool>( iterator( cur, this ), false );
		}
		// insert before first.
		node* tmp = new_node( obj );
		tmp->next = first;
		buckets[ n ] = tmp;
		++num_elements;
		return safe::pair<HASHTABLE::iterator, bool>( iterator( tmp, this ), true );
}
HASH_TEMPLATE
	void HASHTABLE::resize( size_type num_elements_hint ) {
		const size_type old_n = buckets.size( );
		if ( num_elements_hint > old_n ) {
			const size_type n = next_size( num_elements_hint );
			if ( n > old_n ) {
				std::vector<node*, Alloc> tmp( n, ( node* )0 );
				try {
					for ( size_type bucket = 0; bucket < old_n; ++bucket ) {
						node* first = buckets[ bucket ];
						while ( first ) {
							size_type new_bucket = bkt_num( first->value, n );
							buckets[ bucket ] = first->next;
							first->next = tmp[ new_bucket ];
							tmp[ new_bucket ] = first;
							first = buckets[ bucket ]; // move first to next node.
						}
					} // for( ; ; )
					buckets.swap( tmp );
				} // try
			}
		}
}
HASH_TEMPLATE
	void HASHTABLE::clear( ) {
		for ( size_type i  = 0; i < buckets.size( ); ++i ) {
			node* cur = buckets[ i ];
			while ( cur ) {
				node* next = cur->next;
				delete_node( cur );
				cur = next;
			}
			buckets[ i ] = 0;
		}
		num_elements = 0;
}
HASH_TEMPLATE
	void HASHTABLE::copy_from( const hashtable& ht ) {
		buckets.clear( );
		buckets.reserve( ht.buckets.size( ) );
		buckets.insert( buckets.end( ), ht.buckets.size( ), ( node* )0 );
		try {
			for ( size_type i = 0; i < ht.buckets.size( ); ++i ) {
				if ( const node* cur = ht.buckets[ i ] ) {
					node* copy = new_node( cur->value );
					bucket[ i ] = copy;

					for ( node* next = cur->next; next; cur = next, next = cur->next, copy = copy->next )
						// copy one chain.
						copy->next = new_node( next->value );
				}
			}
			num_elements = ht.num_elements;
		}
		catch ( ... ) {
			clear( );
		}
}
HASH_TEMPLATE
	typename HASHTABLE::node* HASHTABLE::new_node( const value_type& obj ) {
		node* n = node_allocator::allocate( );
		n->next = 0;
		try {
			safe::construct( &n->value, obj );
			return n;
		}
		catch ( ... ) {
			node_allocator::deallocate( n );
		}
}
HASH_TEMPLATE
	void HASHTABLE::delete_node( node* n ) {
		safe::destroy( &n->value );
		node_allocator::deallocate( n );
}
HASH_TEMPLATE
	void HASHTABLE::initialize_buckets( size_type n ) {
		const size_type n_buckets = ( size_type )_stl_next_prime( n );
		buckets.reserve( n_buckets );
		buckets.insert( buckets.end( ), n_buckets, ( node* )n );
		num_elements = 0;
}
HASH_TEMPLATE
	typename HASHTABLE::size_type HASHTABLE::bkt_num( const value_type& obj ) const {
		return bkt_num_key( get_key( obj ) );
}
HASH_TEMPLATE
	typename HASHTABLE::size_type HASHTABLE::bkt_num( const value_type& obj, size_type n ) const {
		return bkt_num_key( get_key( obj ), n );
}
HASH_TEMPLATE
	typename HASHTABLE::size_type HASHTABLE::bkt_num_key( const key_type& key ) const {
		return bkt_num_key( key, buckets.size( ) );
}
HASH_TEMPLATE
	typename HASHTABLE::size_type HASHTABLE::bkt_num_key( const key_type& key, size_type n ) const {
		// n means bucket size.
		return hash( key ) % n;
}
HASH_TEMPLATE 
	const int HASHTABLE::_stl_num_primes = 28;
HASH_TEMPLATE 
	const unsigned long HASHTABLE::_stl_prime_list[ _stl_num_primes ] = 
	{ 53,97,193,389,769,1543,3079,6151,12289,24593,49157,98317,196613,393241,786433,
		1572869,3145739,6291469,12582917,25165843,50331653,10066319,201326611,
		402653189,805306457,1610612741,3221225473ul,429496729ul };
HASH_TEMPLATE
	unsigned long HASHTABLE::_stl_next_prime( unsigned long cur ) {
		const unsigned long* first = _stl_prime_list;
		const unsigned long* last = _stl_prime_list + _stl_num_primes;
		const unsigned long* pos = std::lower_bound( first, last, n );
		return pos == last ? *( last - 1 ) : *pos;
}
HASH_TEMPLATE
	typename HASHTABLE::size_type HASHTABLE::max_bucket_count( ) {
		return _stl_prime_list[ _stl_num_primes - 1 ];
}



} // namespace safe.