/**********************************************************
 * File£º adapter_s.h
 *
 * Author£º shenglong (shenglong@vip.126.com)
 *
 * Description£ºAdapter :	container_adapter : stack / queue / heap / priority queue.
 *					   					iterator_adapter :	inserter : back_inserter.
 *																					front_inserter.
 *																					inserter.
 * 					   												reverse.
 * 					   												iostream :	istream_iterator.
 * 																					ostream_iterator.
 *	
 *	State : handling.
 *	
 * 
 * 
 * 
 */
#ifndef ADEPTER_S_H
#define ADEPTER_S_H
#include "include_s.h"
#include "iterator_s.h"			// for _value_type, _distance_type, iterator_category.
#include "functional_s.h"		// for less<T> class.
#include <vector>					// for vector in priority queue,
#include <deque>					// for deque in stack, queue.
#include <iostream>				// for istream, ostream in iostream iterator class.

namespace safe {
// container adapter.
template<typename T,typename Sequence = std::deque<T> >
class stack {
public:
	// type interface.
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
	typedef typename Sequence::size_type size_type;
public:
	// constructor.

	// function interface.
	bool empty( ) const { return seq.empty( ); }
	size_type size( ) const { return seq.size( ); }
	reference top( ) { return seq.back( ); }
	const_reference top( ) const { return seq.back( ); }
	void push( const value_type& v ) { seq.push_back( v ); }
	void pop( ) { seq.pop_back( ); }
protected:
	friend bool operator==( const stack& x, const stack& y ) {
		return x.seq == y.seq;
	}
	friend bool operator<( const stack& x, const stack& y ) {
		return x.seq < y.seq;
	}
protected:
	Sequence seq;
};
template<typename T, typename Sequence = std::deque<T> >
class queue {
public:
	// type interface.
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
	typedef typename Sequence::size_type size_type;
public:
	// constructor.

	// function interface.
	bool empty( ) const { return seq.empty( ); }
	size_type size( ) const { return seq.size( ); }
	reference front( ) { return seq.front( ); }
	const_reference front( ) const { return seq.front( ); }
	reference back( ) { return seq.back( ); }
	const_reference back( ) const { return seq.back( ); }
	void push( const value_type& v ) { seq.push_back( v ); }
	void pop( ) { seq.pop_front( ); }
protected:
	friend bool operator==( const queue& x, const queue& y ) {
		return x.seq == y.seq;
	}
	friend bool operator<( const queue& x, const queue& y ) {
		return x.seq < y.seq;
	}
protected:
	Sequence seq;
};
template<typename T, 
	typename Sequence = std::vector<T>,
	typename Compare = safe::less<T> >
class priority_queue {
public:
	// type interface.
	typedef typename Sequence::value_type value_type;
	typedef typename Sequence::reference reference;
	typedef typename Sequence::const_reference const_reference;
	typedef typename Sequence::size_type size_type;
public:
	// constructor.
	priority_queue( ) : seq( ) { }
	explicit priority_queue( const Compare& x ) : seq( ), comp( x ) { }

	template<typename InputIterator>
	priority_queue( InputIterator first, InputIterator last ) : seq( first, last ) {
		make_heap( seq.begin( ), seq.end( ), comp );
	}
	template<typename InputIterator>
	priority_queue( InputIterator first, InputIterator last, const Compare& x ) : 
		seq( first, last ), comp( x ) {
			make_heap( seq.begin( ), seq.end( ), comp );
		}
public:
	// function interface.
	bool empty( ) const { return seq.empty( ); }
	size_type size( ) const { return seq.size( ); }
	const_reference top( ) const { return seq.front( ); }
	void push( const value_type& v ) { 
		try {
			seq.push_back( v );
			push_heap( seq.begin( ), seq.end( ), comp );
		}
		catch ( ... ) {
			seq.clear( );
		}
	}
	void pop( ) {
		try {
			pop_heap( seq.begin( ), seq.end( ), comp );
			seq.pop_back( );
		}
		catch ( ... ) {
			seq.clear( );
		}
	}
protected:
	Sequence seq;
	Compare comp;
};
// global heap function.
// push, pop, sort, adjust, make heap.
template<typename RandomAccessIterator>
inline void push_heap( RandomAccessIterator first, 
	RandomAccessIterator last ) {
		_push_heap( first, last, _distance_type( first ), _value_type( first ) );
}
template<typename RandomAccessIterator, 
	typename Distance,
	typename T>
inline void _push_heap( RandomAccessIterator first, 
	RandomAccessIterator last,
	Distance&,
	T& ) {
		_push_heap_aux( first, Distance( first - last - 1 ), Distance( 0 ), T( *( last - 1 ) ) );
}
template<typename RandomAccessIterator
	typename Distance,
	typename T>
inline void _push_heap_aux( RandomAccessIterator first, 
	Distance endHole,
	Distance topIndex,
	T end_hole_value ) {
	Distance parent = ( endHole - 1 ) / 2;
	while ( endHole > topIndex && *( first + parent ) < end_hole_value ) {
		*( first + endHole ) = *( first + parent ); // minority parent goes down.
		endHole = parent;
		parent = ( endHole - 1 ) / 2;
	}
	*( first + endHole ) = end_hole_value;
}
template<typename RandomAccessIterator>
inline void pop_heap( RandomAccessIterator first, 
	RandomAccessIterator last ) {
		_pop_heap( first, last, _value_type( first ) );
}
template<typename RandomAccessIterator,
	typename T>
inline void _pop_heap( RandomAccessIterator first,
	RandomAccessIterator last, T& ) {
		_pop_heap_aux( first, last - 1, last - 1, T( *( last - 1 ) ), _distance_type( first ) );
}
template<typename RandomAccessIterator,
	typename T,
	typename Distance>
inline void _pop_heap_aux( RandomAccessIterator first,
	RandomAccessIterator last,
	RandomAccessIterator result,
	T value,
	Distance& ) {
		*result = *first;
		_adjust_heap( first, Distance( 0 ), Distance( last - first ), value );
}
template<typename RandomAccessIterator,
	typename Distance,
	typename T>
void _adjust_heap( RandomAccessIterator first,
	Distance holeIndex,
	Distance len,
	const T& value ) {
		Distance topIndex = holeIndex;
		Distance secondChild = 2 * holeIndex + 2;
		while ( secondChild < len ) {
			if ( *( first + secondChild ) < *( first + secondChild - 1 ) )
				--secondChild;
			*( first + holeIndex ) = *( first + secondChild );

			holeIndex = secondChild;						// move down holeIndex and secondChild.
			secondChild = 2 * ( secondChild - 1 );
		}
		if ( secondChild == len ) {
			*( first + holeIndex ) = *( first + secondChild - 1 );
			holeIndex = secondChild - 1;
		}
		_push_heap_aux( first, holeIndex, topIndex, value );
}
template<typename RandomAccessIterator>
inline void sort_heap( RandomAccessIterator first,
	RandomAccessIterator last) {
		while ( last - first > 1 ) {
			pop_heap( first, last );
		}
}
template<typename RandomAccessIterator>
inline void make_heap( RandomAccessIterator first,
	RandomAccessIterator last ) {
		_make_heap( first, last, _distance_type( first ), _value_type( first ) );
}
template<typename RandomAccessIterator,
	typename Distance,
	typename T>
void _make_heap( RandomAccessIterator first,
	RandomAccessIterator last,
	Distance&,
	T& ) {
		// adjust every node from last to first.
		if ( last - first < 2 ) return;
		Distance len = last - first;
		Distance parent = ( len - 2 ) / 2;
		while ( true ) {
			_adjust_heap( first, parent, len, T( *( first + parent ) ) );
			if ( parent == 0 ) return;
			--parent;
		}
}
// iterator adapter : insert/reverse/iostream.
// open/close interface.

// insert iterator.
// iter = value. ---------- container.push_back( value ). 
template<typename Container>
class back_insert_iterator {
public:
	// type interface.
	typedef safe::output_iterator_tag iterator_category;
	typedef void value_type;
	typedef void pointer;
	typedef void reference;
	typedef void difference_type;
public:
	// constructor.
	explicit back_insert_iterator( Container& x ) : con( x ) { }
	// opened interface.
	back_insert_iterator<Container>& operator=( const typename Container::value_type& value ) {
		con->push_back( value );
		return *this;
	}
	// closed interface.
	back_insert_iterator<Container>& operator*( ) { return *this; }
	back_insert_iterator<Container>& operator++( ) { return *this;	}
	back_insert_iterator<Container>& operator++( int ) { return *this; }
protected:
	Container* con;
};
template<typename Container>
inline back_insert_iterator<Container> back_inserter( Container& x ) {
	return back_insert_iterator<Container>( x );
}
// iter = value. ---------- container.push_front( value ). 
template<typename Container>
class front_insert_iterator {
public:
	// type interface.
	typedef safe::output_iterator_tag iterator_category;
	typedef void value_type;
	typedef void pointer;
	typedef void reference;
	typedef void difference_type;
public:
	// constructor.
	explicit front_insert_iterator( Container& x ) : con( x ) { }
	// opened interface.
	front_insert_iterator<Container>& operator=( const typename Container::value_type& value ) {
		con->push_front( value );
		return *this;
	}
	// closed interface.
	front_insert_iterator<Container>& operator*( ) { return *this; }
	front_insert_iterator<Container>& operator++( ) { return *this;	}
	front_insert_iterator<Container>& operator++( int ) { return *this; }
protected:
	Container* con;
};
template<typename Container>
inline front_insert_iterator<Container> front_inserter( Container& x ) {
	return front_insert_iterator<Container>( x );
}
// iter = value. ---------- container.insert( iter, value ). 
template<typename Container>
class insert_iterator {
public:
	// type interface.
	typedef safe::output_iterator_tag iterator_category;
	typedef void value_type;
	typedef void pointer;
	typedef void reference;
	typedef void difference_type;
public:
	// constructor.
	explicit insert_iterator( Container& x, typename Container::iterator it ) : con( x ), iter( it ) { }
	// opened interface.
	insert_iterator<Container>& operator=( typename Container::value_type& value ) {
		iter = con->insert( iter, value );
		++iter;
		return *this;
	}
	// closed interface.
	insert_iterator<Container>& operator*( ) { return *this; }
	insert_iterator<Container>& operator++( ) { return *this;	}
	insert_iterator<Container>& operator++( int ) { return *this; }
protected:
	Container* con;
	typename Container::iterator iter;
};
template<typename Container, typename Iterator>
inline insert_iterator<Container> inserter( Container& x, Iterator i ) {
	typedef typename Container::iterator iter_type;
	return insert_iterator<Container>( x, iter_type( i ) );
}
// reverse iterator.
template<typename Iterator>
class reverse_iterator {
public:
	// tpye interface.
	typedef typename safe::iterator_traits<Iterator>::value_type value_type;
	typedef typename safe::iterator_traits<Iterator>::pointer pointer;
	typedef typename safe::iterator_traits<Iterator>::reference reference;
	typedef typename safe::iterator_traits<Iterator>::difference_type difference_type;
	typedef typename safe::iterator_traits<Iterator>::iterator_category iterator_category;

	typedef Iterator iterator_type;
	typedef reverse_iterator<Iterator> self;
public:
	// constructor.
	reverse_iterator( ) { }
	explicit reverse_iterator( iterator_type x ) : current( x ) { }
	reverse_iterator( const self& x ) : current( x.current ) { }
public:
	// function interface.
	iterator_type base( ) const { return current; }
	reference operator*( ) { 
		iterator_type tmp = current;
		return *( --tmp ); 
	}
	pointer operator->( ) {
		return &( operator*( ) );
	}
	reference operator[ ]( difference_type n ) {
		// return *( *this + n ); // ???
		return *( current - n - 1 );
	}
	self& operator++( ) {
		--current;
		return *this;
	}
	self operator++( int ) {
		self tmp = *this;
		--current;
		return tmp;
	}
	self& operator--( ) {
		++current;
		return *this;
	}
	self operator--( int ) {
		self tmp = *this;
		++current;
		return *this;
	}
	self operator+( difference_type n ) const {
		return self( current - n );
	}
	self& operator+=( difference_type n ) {
		current -= n;
		return *this;
	}
	self operator-( difference_type n ) const {
		return self( current + n );
	}
	self& operator-=( difference_type n ) {
		current += n;
		return *this;
	}
protected:
	Iterator current;
};
template<typename Iterator>
inline reverse_iterator<Iterator> reverser( Iterator& x ) {
	return reverse_iterator<Iterator>( x );
}
// iostream iterator.
template<typename T>
class ostream_iterator {
public:
	// type interface.
	typedef safe::output_iterator_tag iterator_category;
	typedef void value_type;
	typedef void pointer;
	typedef void reference;
	typedef void difference_type;
public:
	// constructor.
	ostream_iterator( std::ostream& s ) : con( s ), string( 0 ) { }
	ostream_iterator( std::ostream& s, const char* c ) : con( s ), string( c ) { }
public:
	// opened function interface.
	ostream_iterator<T>& operator=( const T& value ) {
		*con << value;
		if ( string )
			*con << string;
		return *this;
	}
	// closed function interface.
	ostream_iterator<T>& operator*( ) { return *this; }
	ostream_iterator<T>& operator++( ) { return *this; }
	ostream_iterator<T>& operator++( int ) { return *this; }
protected:
	std::ostream* con;
	const char* string;
};
template<typename T, typename Distance = std::ptrdiff_t>
class istream_iterator {
	// type interface.
	// data.
	// constructor.
	// opened function interface.
	// closed function interface.
};

// functor adapter.
// see functional_s.h.


} // namespace safe.
#endif // ADEPTER_S_H