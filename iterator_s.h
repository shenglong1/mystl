/**********************************************************
 * File£º iterator_s.h
 *
 * Author£º shenglong (shenglong@vip.126.com)
 *
 * Description£ºthis file contains basic components of iterator.
 *		include:	tag.
 *						traits :	iterator_traits<Con>.
 *									_type_traits<Value>.
 * 						base iterator.
 * 						functions :	type_x.
 * 											iterator handler.
 *
 * State : handling.
 * 
 */
#ifndef ITERATOR_S_H
#define ITERATOR_S_H
#include "include_s.h"

namespace safe {
// tag.
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag { };
struct bidirectional_iterator_tag { };
struct random_access_iterator_tag { };
// iterator_traits
template<typename Iter>
struct iterator_traits {
	typedef typename Iter::value_type				value_type;
	typedef typename Iter::iterator_category	iterator_category;
	typedef typename Iter::pointer					pointer;
	typedef typename Iter::reference				reference;
	typedef typename Iter::difference_type		difference_type;
};
template<typename T>
struct iterator_traits<T*> {
	typedef typename T											value_type;
	typedef typename random_access_iterator_tag	iterator_category;
	typedef typename T*											pointer;
	typedef typename T&											reference;
	typedef typename std::ptrdiff_t							difference_type;
};
template<typename T>
struct iterator_traits<const T*> {
	typedef typename T											value_type;
	typedef typename random_access_iterator_tag	iterator_category;
	typedef typename const T*									pointer;
	typedef typename const T&								reference;
	typedef typename std::ptrdiff_t							difference_type;
};
// _type_trait.
class _true_type { }; // inner type, directly copy and construct.
class _false_type { }; // generic type, using its copy and construct member function only.
template<typename T> // generic type.
struct _type_traits {
	typedef _false_type		has_trivial_default_constructor;
	typedef _false_type		has_trivial_copy_constructor;
	typedef _false_type		has_trivial_assignment_operator;
	typedef _false_type		has_trivial_destructor;
	typedef _false_type		is_POD_type;
};
template<typename T>
struct _type_traits<T*> {
	typedef _true_type		has_trivial_default_constructor;
	typedef _true_type		has_trivial_copy_constructor;
	typedef _true_type		has_trivial_assignment_operator;
	typedef _true_type		has_trivial_destructor;
	typedef _true_type		is_POD_type;
};
template<typename T>
struct _type_traits<const T*> {
	typedef _true_type		has_trivial_default_constructor;
	typedef _true_type		has_trivial_copy_constructor;
	typedef _true_type		has_trivial_assignment_operator;
	typedef _true_type		has_trivial_destructor;
	typedef _true_type		is_POD_type;
};
template<> // partial specialization for inner type.
struct _type_traits<int> {
	typedef _true_type		has_trivial_default_constructor;
	typedef _true_type		has_trivial_copy_constructor;
	typedef _true_type		has_trivial_assignment_operator;
	typedef _true_type		has_trivial_destructor;
	typedef _true_type		is_POD_type;
}; // add other inner type ....
// base iterator.
template<typename T, 
	typename Category, 
	typename Pointer = T*,
	typename Reference = T&, 
	typename Difference = std::ptrdiff_t> 
struct iterator {
	typedef T					value_type;
	typedef Category		iterator_category;
	typedef Pointer		pointer;
	typedef Reference	reference;
	typedef Difference	difference_type;
};
// global functions, type func.
template<typename Iterator>
inline typename iterator_traits<Iterator>::value_type
	_value_type( const Iterator& ) {
		typedef typename iterator_traits<Iterator>::value_type value_type;
		return value_type( );
}
template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type
	_distance_type( const Iterator& ) {
		typedef typename iterator_traits<Iterator>::difference_type difference;
		return difference( );
}
template<typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
	_iterator_category_type( const Iterator& ) {
		typedef typename iterator_traits<Iterator>::iterator_category category;
		return category( );
}
// global functions, func for iterator.
template<typename InputIterator> // interface function
inline typename iterator_traits<InputIterator>::difference_type
	distance(  InputIterator first, InputIterator last ) {
		typedef typename iterator_traits<InputIterator>::iterator_category category;
		return _distance( first, last, category( ) );
}
template<typename InputIterator> // overload 1.
inline typename iterator_traits<InputIterator>::difference_type
	_distance( InputIterator first, InputIterator last, input_iterator_tag& ) {
		typename iterator_traits<InputIterator>::difference_type n = 0; 
		while ( first != last ) {
			++first;
			++n;
		}
		return n;
}
template<typename InputIterator> // overload 2.
inline typename iterator_traits<InputIterator>::difference_type
	_distance( InputIterator first, InputIterator last, random_access_iterator_tag& ) {
		return last - first;
}
template<typename InputIterator, typename Distance> // interface func.
inline void advance( InputIterator& iter, const Distance& n ) {
	typedef typename iterator_traits<InputIterator>::iterator_category category;
	_advance( iter, n, category( ) );
}
template<typename InputIterator, typename Distance> // overload 1.
inline void _advance( InputIterator& iter, Distance& n, input_iterator_tag& ) {
	while ( n-- )
		++iter;
}
template<typename BidirectionalIterator, typename Distance> // overload 2.
inline void _advance( BidirectionalIterator& iter, Distance& n, bidirectional_iterator_tag& ) {
	if ( n >= 0 )
		while ( n-- ) ++iter; // move right.
	else
		while ( n++ ) --iter; // move left.
}
template<typename RandomIterator, typename Distance>
inline void _advance( RandomIterator& iter, Distance& n, random_access_iterator_tag& ) {
	iter += n;
}

} // namespace safe.
#endif // ITERATOR_S_H