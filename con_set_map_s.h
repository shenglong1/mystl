/**********************************************************
 * File£º con_set_map_s.h
 *
 * Author£º shenglong (shenglong@vip.126.com)
 *
 * Description£ºthis file contains basic components of set/map based on rb-tree.
 *
 *	State : handling.
 *				
 * 
 * 
 * 
 */
#ifndef CON_SET_MAP_S_H
#define CON_SET_MAP_S_H
#include "include_s.h"
#include "con_rb_tree_s.h"		// for rb_tree class.
#include "con_hashtable_s.h"	// for hashtable class.
#include "utility_s.h"					// for pair<>.
#include "memory_s.h"				// for alloc.
#include "functional_s.h"			// for less<>, identity<>.

namespace safe {
// predeclare.
template<typename T1, typename T2>
class pair;

// set based on rb_tree.
template<typename Key, 
	typename Compare = safe::less<Key>,
	typename Alloc = alloc>
class set {
public:
	typedef Key value_type;
	typedef Key key_type;
	typedef Compare value_compare;
	typedef Compare key_compare;
public:
	// rb_tree<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
	typedef typename safe::rb_tree<Key, Key, identity<Key>, Compare, Alloc> rep_type;
	typedef typename rep_type::const_pointer pointer;
	typedef typename rep_type::const_pointer const_pointer;
	typedef typename rep_type::const_reference reference;
	typedef typename rep_type::const_reference const_reference;
	typedef typename rep_type::size_type size_type;
	typedef typename rep_type::difference_type difference_type;
	typedef typename rep_type::iterator iterator;

	typedef typename set<Key,Compare,Alloc> self;
	typedef safe::pair<iterator, bool> pair_iterator_bool;
public:
	// constructor.
	set( ) : t( Compare( ) ) { }
	explicit set( const Compare& comp ) : t( comp ) { }
	explicit set( const self& x ) : t( x.t ) { }
	template<typename InputIterator>
	set( InputIterator first, InputIterator last ) : t( Compare( ) ) {
		t.insert_unique( first, last );
	}
	template<typename InputIterator>
	set( InputIterator first, InputIterator last, const Compare& comp ) : t( comp ) {
		t.insert_unique( first, last );
	}
	self& operator=( const self& x ) {
		return self( x );
	}
public:
	// function interface.
	key_compare key_comp( ) const { return t.key_comp(); }
	value_compare value_comp( ) const { return t.key_comp(); }
	iterator begin( ) { return t.begin(); }
	iterator end( ) { return t.end(); }
	bool empty( ) { return t.empty(); }
	size_type size( ) { return t.size();
	size_type max_size( ) { return t.max_size(); }
	void swap( self& x ) { t.swap( x.t ); }
	pair_iterator_bool insert( const value_type& x ) {
		return t.insert_unique( x );
	}
	iterator insert( iterator pos, const value_type& x ) {
		return t.insert_unique( pos, x );
	}
	template<typename InputIterator>
	void insert( InputIterator first, InputIterator last ) {
		t.insert_unique( first, last );
	}
	void erase( iterator pos ) {
		
	}
	size_type erase( const key_type& x ) {

	}
	void erase( iterator first, iterator last ) {
		
	}
	void clear( ) { }
	iterator find( const key_type& x ) const { return t.find( x ); }
	size_type count( const key_type& x ) const { }
	iterator lower_bound( const key_type& x ) const { }
	iterator upper_bound( const key_type& x ) const { }
	safe::pair<iterator,iterator> equal_range( const key_type& x ) const { }
	friend bool operator==( const self& x, const self& y );
	friend bool operator<( const self& x, const self& y );
 private:
	rep_type t;
};
// map.
// multiset.
// multimap.
// hash_set.
// hash_map.
// hash_multiset.
// hash_multimap.



} // namespace safe.
#endif // CON_SET_MAP_S_H