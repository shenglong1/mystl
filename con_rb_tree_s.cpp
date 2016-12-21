#include "stdafx.h"
#include "include_s.h"
#include "con_rb_tree_s.h"

namespace safe {
// iterator_base.
void _rb_tree_base_iterator::increment( ) {
	// successor : right son min or first left parent.
	if ( node->right != 0 )
		node = _rb_tree_node_base::minimum( node->right );
	else {
		base_ptr parent = node->parent;
		while ( 0 != parent && node == parent->right ) {
			node = parent;
			parent = parent->parent;
		}
		if ( 0 != parent )
			node = parent;
	}
}
void _rb_tree_base_iterator::decrement( ) {
	// predecessor : left son max or first right parent.
	if ( node->color == _rb_tree_red &&
		node->parent->parent == 0 ) {
			node = node->right;
	} else if ( node->left != 0 ) {
		node = _rb_tree_node_base::maximum( node->left );
	} else {
		base_ptr parent = node->parent;
		while ( parent != 0 && node == parent->left ) {
			node = parent;
			parent = parent->parent;
		}
		if ( parent != 0 )
			node = parent;
	}
}
// RB-Tree.
template<typename Key, 
	typename Value, 
	typename KeyOfValue, 
	typename Compare, 
	typename Alloc>
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
	rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
	operator=( const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x ) {
		return rb_tree<Key, Value, KeyOfValue, Compare, Alloc>( x.key_comp );
}
template<typename Key, 
	typename Value, 
	typename KeyOfValue, 
	typename Compare, 
	typename Alloc>
	safe::pair<typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator, bool>
	rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
	insert_unique( const value_type& v ) {
		link_type y = root( );
		link_type x = header;
		bool comp = true;
		while ( x != 0 ) {
			y = x;
			comp = key_compare( KeyOfValue()( v ), Key( y ) );
			x = comp ? left( x ) : right( x );
		}
		iterator j = iterator( y );
		if ( comp ) {
			// insert left son.
			if ( j == begin( ) )
				return pair<iterator, bool>( _insert( x, y, v ), true );
			else
				--j;
		}
		if ( key_compare( key( j.node ), KeyOfValue()( v ) ) ) // y < v.
			// insert right son.
			return pair<iterator, bool>( _insert( x, y, v ), true );
		return pair<iterator, bool>( j, false );
}
template<typename Key, 
	typename Value, 
	typename KeyOfValue, 
	typename Compare, 
	typename Alloc>
	typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
	rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
	insert_equal( const value_type& v ) {
		link_type y = root( );
		link_type x = header;
		while ( x != 0 ) {
			y = x;
			x = key_compare( KeyOfValue()( v ), Key( x ) ) ? left( x ) : right( x ) ;
		}
		return _insert( x, y, v );
}
template<typename Key, 
	typename Value, 
	typename KeyOfValue, 
	typename Compare, 
	typename Alloc>
	typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
	rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
	_insert( base_ptr x, base_ptr y, const value_type& v ) {
		// insert point s.
		// y == x->parent.
		link_type x = ( link_type )_x;
		link_type y = ( link_type )_y;
		link_type z  = 0; // store new node.
		if ( y == header || x != 0 || key_compare( KeyOfValue()( v ), key( y ) ) ) {
			// insert left.
			z = create_node( v );
			y->left = z;
			if ( y == header ) {
				root( ) = z;
				rightmost( ) = z;
			}
			else if ( y == leftmost( ) ) 
				leftmost( ) = z;
		} else {
			// insert right.
			z = create_node( v );
			y->right = z;
			if ( y == rightmost( ) ) 
				rightmost( ) = z;
		}
		z->parent = y; // z is leaf node.
		z->left = 0;
		z->right = 0;

		_rb_tree_rebalance( z, header->parent );
		++node_count;
		return iterator( z );
}
template<typename Key, 
	typename Value, 
	typename KeyOfValue, 
	typename Compare, 
	typename Alloc>
	typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::link_type
	rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
	_copy( link_type x, link_type p ) {
		return 0;
}
template<typename Key, 
	typename Value, 
	typename KeyOfValue, 
	typename Compare, 
	typename Alloc>
	void
	rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
	_erase( link_type x ) {

}
template<typename Key, 
	typename Value, 
	typename KeyOfValue, 
	typename Compare, 
	typename Alloc>
	void
	rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
	_init( ) {
		header = get_node( );
		header->color = _rb_tree_red;
		root( ) = 0;
		leftmost( ) = header;
		rightmost( ) = header;
}


} // namespace safe.