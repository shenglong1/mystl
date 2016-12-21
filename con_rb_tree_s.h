/**********************************************************
 * File£º con_rb_tree_s.h
 *
 * Author£º shenglong (shenglong@vip.126.com)
 *
 * Description£º_rb_tree_node_base,_rb_tree_node,_rb_tree_iterator,rb_tree,_rb_tree_rotate_left,_rb_tree_rotate_right,_rb_tree_rebalance.
 *
 *	State : handling.
 *				
 * 
 * 
 * 
 */
// state : handling....
// container must have : 
// type_interface :	key_type;
//							value_type;
//							pointer;
//							const_pointer;
//							reference;
//							const_reference;
//							size_type;
//							difference_type;
//							iterator/iterator_type;
//							node/node_type;
//							allocator/allocator_type.
// function_interface : 
// _rb_tree_node_base	-------------- _rb_tree_base_iterator, include member node/node_type.
// _rb_tree_node<>		-------------- _rb_tree_iterator<>, include member node_type.
#ifndef CON_RB_TREE_S_H
#define CON_RB_TREE_S_H
#include "include_s.h"
#include "utility_s.h"		// for pair.
#include "memory_s.h"	// for alloc class.
#include "iterator_s.h"	// for bidirectional_iterator_tag.

namespace safe {
// predeclare.
template<typename T1, typename T2>
class pair;

// color_type.
typedef bool _rb_tree_color_black;
static const _rb_tree_color_black _rb_tree_red = false;
static const _rb_tree_color_black _rb_tree_black = true;
// node_base.
struct _rb_tree_node_base {
	typedef _rb_tree_color_black	color_type;
	typedef _rb_tree_node_base*	base_ptr; // node_base_type.
	color_type color;
	base_ptr parent;
	base_ptr left;
	base_ptr right;
	static base_ptr minimum( base_ptr x ) {
		if ( x != 0 )
			while ( x->left != 0 )
				x = x->left;
		return x;
	}
	static base_ptr maximum( base_ptr x ) {
		if ( x != 0 )
			while ( x->right != 0 )
				x = x->right;
		return x;
	}
};
// node.
template<typename Value>
struct _rb_tree_node : public _rb_tree_node_base {
	typedef _rb_tree_node<Value>* link_type; // node_type.
	Value value_field;
};
// iterator_base.
struct _rb_tree_base_iterator {
	typedef _rb_tree_node_base::base_ptr	base_ptr; // node_base_type.
	typedef safe::bidirectional_iterator_tag	iterator_category;
	typedef std::ptrdiff_t									difference_type;
	
	base_ptr node;
	void increment( );	// successor.
	void decrement( );	// predecessor.
};
// iterator.
template<typename Value, typename Ref, typename Ptr>
struct _rb_tree_iterator : public _rb_tree_base_iterator {
	typedef Value												value_type;
	typedef safe::bidirectional_iterator_tag		iterator_category;
	typedef Ptr													pointer;
	typedef Ref													reference;
	typedef std::ptrdiff_t										difference_type;
	typedef _rb_tree_iterator<Value, Ref, Ptr>										self;
	typedef _rb_tree_iterator<Value, Value&, Value*>						iterator;			// iterator_type.
	typedef _rb_tree_iterator<Value, const Value&, const Value*>	const_iterator;
	typedef _rb_tree_node<Value>*														link_type;		// node_type.
	// implicit node var : _rb_tree_base_iterator::base_ptr node;

	_rb_tree_iterator( ) { }
	explicit _rb_tree_iterator( link_type x ) { node = x; }
	explicit _rb_tree_iterator( const iterator& it ) { node = it.node; }
	virtual ~_rb_tree_iterator( ) { }
	reference operator*( ) const { return link_type( node )->value_field; }
	pointer operator->( ) const { return &operator*( ); }
	self& operator++( ) { 
		increment( );
		return *this;
	}
	self operator++( int ) {
		self tmp = *this;
		increment( ); 
		return tmp;
	}
	self& operator--( ) {
		decrement( );
		return *this;
	}
	self operator--( int ) {
		self tmp = *this;
		decrement( ); 
		return tmp;
	}
};
// RB-Tree.
// public :		constructor.
//					type_interface.
//					function_interface.
// private :	data.
//					inner_type.
//					inner_function.
template<typename T1, typename T2> class pair; // pre-declare.
template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = alloc>
class rb_tree {
	// iter / type
	// node / type
	// alloc / alloc type
public:
	// constructor.
	rb_tree( const Compare& comp = Compare( ) ) : node_count( 0 ), key_compare( comp ) { _init( ); }
	~rb_tree( ) { clear( ); put_node( header ); }
	rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
		operator=( const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& x );
public:
	// type interface.
	typedef Key								key_type;
	typedef Value							value_type;
	typedef value_type*					pointer;
	typedef const value_type*		const_pointer;
	typedef value_type&				reference;
	typedef const value_type&		const_reference;
	typedef std::size_t					size_type;
	typedef std::ptrdiff_t					difference_type;
public:
	typedef _rb_tree_iterator<value_type, reference, pointer> iterator;			// iter_type.
	safe::pair<iterator, bool> insert_unique( const value_type& v );
	iterator insert_equal( const value_type& v );
public:
	// function interface.
	Compare key_comp( ) const { return key_compare; }
	iterator begin( ) { return leftmost( ); }
	iterator end( ) { return header; }
	bool empty( ) { return 0 == node_count; }
	size_type size( ) const { return node_count; }
	size_type max_size( ) const { return size_type( -1 ); }
	iterator find( const key_type& k ) { 
		link_type y = header;
		link_type x = root( );
		while ( x != 0 ) {
			if ( key_compare( k, key( x ) ) ) {
				// left down.
				y = x;
				x = x->left;
			} else {
				y = x;
				x = x->right;
			}
			iterator j = iterator( y );
			return ( j == end( ) || key_compare( k, key( j->node ) ) ) ? end( ) : j;
		}
	}
protected:
	// inner type declare.
	// rb_tree node interface.
	typedef void*														void_pointer;
	typedef _rb_tree_color_black								color_type;
	typedef _rb_tree_node<Value>							rb_tree_node;				// node_type.
	typedef _rb_tree_node_base*								base_ptr;
	typedef _rb_tree_node<Value>*							link_type;
	typedef simple_allocator<rb_tree_node, Alloc>	rb_tree_node_allocator;	// alloc_type.
protected:
	// inner implement function.
	link_type get_node( ) { return rb_tree_node_allocator::allocate( ); }			// allocate a rb_tree_node.
	void put_node( link_type p ) { rb_tree_node_allocator::deallocate( p ); }		// deallocate a rb_tree_node.
	link_type create_node( const value_type& x ) {										// allocate a rb_tree_node and init it.
		link_type tmp = get_node( );
		try {
			safe::construct( &tmp->value_field, x );
		}
		catch ( ... ) {
			put_node( tmp );
		}
		return tmp;
	}
	link_type clone_node( link_type x ) {						// create a node and copy it.
		link_type tmp = create_node( x->value_field );
		tmp->color = x->color;
		tmp->left = 0;
		tmp->right = 0;
		return tmp;
	}
	void destroy_node( link_type p ) {							// destruct a node.
		safe::destroy( &p->value_field );									
	}
protected:
	static link_type minimum( link_type x ) {
		return ( link_type ) _rb_tree_node_base::minimum( x ); // poly.
	}
	static link_type maximum( link_type x ) {
		return ( link_type ) _rb_tree_node_base::maximum( x ); // poly.
	}
protected:
	// inner helper function.
	link_type& root( ) const { return ( link_type& )header->parent; }
	link_type& leftmost( ) const { return ( link_type& )header->left; }
	link_type& rightmost( ) const { return ( link_type& )header->right; }

	static link_type& left( link_type x ) { return ( link_type& )( x->left ); }
	static link_type& right( link_type x ) { return ( link_type& )( x->right ); }
	static link_type& parent( link_type x ) { return ( link_type& )( x->parent ); }
	static reference value( link_type x ) { return x->value_field; }
	static key_type& key( link_type x ) { return KeyOfValue( )( value( x ) ); }
	static color_type& color( link_type x ) { return ( color_type& )( x->color ); }

	static link_type& left( base_ptr x ) { return ( link_type& )( x->left ); }
	static link_type& right( base_ptr x ) { return ( link_type& )( x->right ); }
	static link_type& parent( base_ptr x ) { return ( link_type& )( x->parent ); }
	static reference value( base_ptr x ) { return ( ( link_type )x )->value_field; }
	static key_type& key( base_ptr x ) { return KeyOfValue( )( value( link_type( x ) ) ); }
	static color_type& color( base_ptr x ) { return ( color_type& )( link_type( x )->color ); }
private:
	iterator _insert( base_ptr _x, base_ptr _y, const value_type& v );
	link_type _copy( link_type x, link_type p );
	void _erase( link_type x );
	void _init( );
private:
	// state data.
	size_type node_count;
	link_type header;
	Compare key_compare;
};
// rb_tree global function.
// while(parent==red) :	parent is left :		uncle red :									black parent/uncle, red gradpa, x move up 2 levels.
//																uncle is 0 or uncle is black :		inner rotate correcting to standard mode, 
//																													then color(x) and rotate gradpa.
//
//									parent is right :	uncle red :									black parent/uncle, red gradpa, x move up 2 levels.
//																uncle is 0 or uncle is black :		inner rotate correcting to standard mode, 
//																													then color(x) and rotate gradpa. 
inline void _rb_tree_rotate_left( _rb_tree_node_base* x, _rb_tree_node_base* root ) {
	// x meas the top node in main roller.
	_rb_tree_node_base* y = x->right;
	x->right = y->left;
	if ( y->left != 0 )
		y->left->parent = x;

	if ( x == root )
		root = y;
	else if ( x == x->parent->left )
		x->parent->left = y;
	else
		x->parent->right = y;
	y->parent = x->parent;

	y->left = x;
	x->parent = y;
}
inline void _rb_tree_rotate_right( _rb_tree_node_base* x, _rb_tree_node_base* root ) {
	_rb_tree_node_base* y = x->left;
	x->left = y->right;
	if ( y->right != 0 )
		y->right->parent = x;

	if ( x == root )
		root = y;
	else if ( x == x->parent->left )
		x->parent->left = y;
	else
		x->parent->right = y;
	y->parent = x->parent;

	y->right = x;
	x->parent = y;
}
inline void _rb_tree_rebalance( _rb_tree_node_base* x, _rb_tree_node_base* root ) {
	x->color = _rb_tree_red;
	while ( x != root && x->parent->color == _rb_tree_red ) {
		// parent is left son.
		if ( x->parent == x->parent->parent->left ) {
			_rb_tree_node_base* y = x->parent->parent->right;
			if ( y && y->color == _rb_tree_red ) {
				x->parent->color = _rb_tree_black;
				y->color = _rb_tree_black;
				x->parent->parent->color = _rb_tree_red;
				x = x->parent->parent;
			} else {
				if ( x == x->parent->right ) { 
					x = x->parent;
					_rb_tree_rotate_left( x, root );
				}
				x->parent->color = _rb_tree_black;
				x->parent->parent->color = _rb_tree_red;
				_rb_tree_rotate_right( x->parent->parent, root );
			}
		} else {
			// parent is right son.
			_rb_tree_node_base* y = x->parent->parent->left;
			if ( y && y->color == _rb_tree_red ) {
				x->parent->color = _rb_tree_black;
				y->color = _rb_tree_black;
				x->parent->parent->color = _rb_tree_red;
				x = x->parent->parent;
			} else {
				if ( x == x->parent->left ) {
					x = x->parent;
					_rb_tree_rotate_right( x, root );
				}
				x->parent->color = _rb_tree_black;
				x->parent->parent->color = _rb_tree_red;
				_rb_tree_rotate_left( x->parent->parent, root );
			}
		} // x->parent == x->parent->parent->right.
	} // while.
}


} // namespace safe.
#endif // CON_RB_TREE_S_H