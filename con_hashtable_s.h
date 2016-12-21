/**********************************************************
 * File£º con_hashtable_s.h
 *
 * Author£º shenglong (shenglong@vip.126.com)
 *
 * Description£ºthis file contains basic components of hash table class.
 *						include _hashtable_node,_hashtable_const_iterator,_hashtable_iterator,hashtable.
 *	
 *	State : handling.			
 * 
 * 
 * 
 */
#ifndef CON_HASHTABLE_S_H
#define CON_HASHTABLE_S_H
#include "include_s.h"
#include "memory_s.h"	// for alloc class, simple_allocator<>.
#include "iterator_s.h"	// for forward_iterator_tag class.
#include <vector>			// for vector<>.

namespace safe {
// predeclare.
template<typename T1, typename T2>
class pair;

// node.
template<typename T>
struct _hashtable_node {
	_hashtable_node* next;
	T value;
};
// declare.
template<typename Value,
	typename Key,
	typename HashFn,
	typename ExtractKey,
	typename EqualKey,
	typename Alloc>
struct _hashtable_const_iterator; // declare.
template<typename Value,
	typename Key,
	typename HashFn,
	typename ExtractKey,
	typename EqualKey,
	typename Alloc>
class hashtable; // declare.

template<typename Value,
	typename Key,
	typename HashFn,
	typename ExtractKey,
	typename EqualKey,
	typename Alloc>
struct _hashtable_iterator {
	typedef forward_iterator_tag iterator_category;
	typedef Value value_type;
	typedef Value* pointer;
	typedef Value& reference;
	typedef std::ptrdiff_t difference_type;
	typedef std::size_t size_type;

	typedef hashtable<Value, Key, HashFn, ExtractKey, EqualKey, Alloc> hashtable;
	typedef _hashtable_iterator<Value, Key, HashFn, ExtractKey, EqualKey, Alloc> iterator;
	typedef _hashtable_const_iterator<Value, Key, HashFn, ExtractKey, EqualKey, Alloc> const_iterator;
	typedef _hashtable_node<Value> node;

	node* cur_node;
	hashtable* ht;

	_hashtable_iterator( ) { }
	_hashtable_iterator( node* n, hashtable* h ) : cur_node( n ), ht( h ) { }

	reference operator*( ) { return cur_node->value; }
	pointer operator->( ) { return &( cur_node->value ); }
	iterator operator++( ) { 
		const node* old = cur_node;
		cur_node = cur_node->next;
		if ( !cur_node ) {
			// adjust to another bucket.
			size_type bucket = ht->bkt_num( old->value );
			while ( !cur_node && ++bucket < ht->buckets.size( ) )
				cur_node = ht->buckets[ bucket ];
		}
		return *this;
	}
	iterator operator++( int ) { 
		iterator tmp( this->cur_node, this->ht );
		++( *this ); // call iterator::operator++();
		return tmp
	}
	bool operator==( const iterator& it ) const { return cur_node == it.cur_node; }
	bool operator!=( const iterator& it ) const { return cur_node != it.cur_node; }
};
template<typename Value,
	typename Key,
	typename HashFn,
	typename ExtractKey,
	typename EqualKey,
	typename Alloc>
struct _hashtable_const_iterator {

};
template<typename Value,
	typename Key,
	typename HashFn,
	typename ExtractKey,
	typename EqualKey,
	typename Alloc = safe::alloc>
class hashtable {
public:
	// type interface.
	typedef Value value_type;
	typedef Key key_type;
	typedef Value* pointer;
	typedef const Value* const_pointer;
	typedef Value& reference;
	typedef const Value& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	typedef HashFn hasher;
	typedef EqualKey key_equal;
private:
	typedef _hashtable_iterator<Value, Key, HashFn, ExtractKey, EqualKey, Alloc> iterator;
	typedef _hashtable_node<Value> node;
	typedef safe::simple_allocator<node, Alloc> node_allocator;
public:
	// constructor.
	hashtable( size_type n, const hasher& hf, const key_equal& eq ) : 
	  hash( hf ), equals( eq ), get_key( ExtractKey( ) ), num_elements( 0 ) {
		  initialize_buckets( n );
	  }
public:
	// function interface.
	class iterator;
	size_type bucket_count( );
	iterator insert_equal( const value_type& obj );
	iterator insert_equal_noresize( const value_type& obj );
	safe::pair<iterator, bool> insert_unique( const value_type& obj );
	safe::pair<iterator, bool> insert_unique_noresize( const value_type& obj );
	void resize( size_type num_elements_hint );
	void clear( );
	void copy_from( const hashtable& ht );
protected:
	// method.
	node* new_node( const value_type& obj );
	void delete_node( node* n );
	void initialize_buckets( size_type n );
	// bucket.
	size_type bkt_num( const value_type& obj ) const;
	size_type bkt_num( const value_type& obj, size_type n ) const;
	
	size_type bkt_num_key( const key_type& key ) const;
	size_type bkt_num_key( const key_type& key, size_type n ) const;
private:
	// data.
	hasher hash;
	key_equal equals;
	ExtractKey get_key;
	std::vector<node*, Alloc> buckets;
	size_type num_elements;
private:
	static const int _stl_num_primes;
	static const unsigned long _stl_prime_list[ _stl_num_primes ];
	inline unsigned long _stl_next_prime( unsigned long cur );
	inline size_type max_bucket_count( );
};


} // namespace safe.
#endif // CON_HASHTABLE_S_H