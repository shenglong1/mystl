/**********************************************************
 * File£º functional_s.h
 *
 * Author£º shenglong (shenglong@vip.126.com)
 *
 * Description£ºthis file contains basic components of functional.
 *						include: Predict :	argument_type, bool.
 *									 				first_argument_type, second_argument_type, result_type.
 *									 Operation :	argument_type, result_type.
 * 									 					first_argument_type, second_argument_type, result_type.
 * 									 base :	unary_function.
 *									 			binary_function.
 *									 predefine : 
 *											 f :	pointer_to_unary_function.
 *											 		pointer_to_binary_function.
 *											 mf :	mem_fun_t/const_mem_fun_t.
 *											 		mem_fun1_t/const_mem_fun1_t.
 *											 		mem_fun_ref/const_mem_fun_ref.
 *											 		mem_fun1_ref/const_mem_fun1_ref.
 *											 FO :	binder1st/binder2nd.
 *											 		unary_compose/binary_compose.
 *
 */
#ifndef FUNCTIONAL_S_H
#define FUNCTIONAL_S_H
#include "include_s.h"

namespace safe {
//////////////////////////////   base   //////////////////////////////////////////// 
template<typename Result>
struct generate_function {
	typedef Result result_type;
};
template<typename Arg,typename Result>
struct unary_function {
	typedef Arg arugment_type;
	typedef Result result_type;
};
template<typename Arg1,typename Arg2,typename Result>
struct binary_function {
	typedef Arg1 first_argument_type;
	typedef Arg2 second_argument_type;
	typedef Result result_type;
};
//////////////////////////////   pre-difine   //////////////////////////////////////////// 
// calculate : negate, plus, minus, multiples, divides, modulus
template<typename Type>
struct negate : public unary_function<Type,Type> {
	Type& operator ()( Type& _Left ) const {
		return -_Left;
	}
};
template<typename Type>
struct plus : public binary_function<Type,Type,Type> {
	Type operator()( const Type& _Left,const Type& _Right ) const {
		return _Left + _Right;
	}
};
template<class Type>
struct minus : public binary_function<Type, Type, Type> 
{
	Type operator()( const Type& _Left,const Type& _Right ) const {
		return _Left - _Right;
	};
};
template<typename Type>
struct multiples : public binary_function<Type,Type,Type> {
	Type operator()( const Type& _Left,const Type& _Right ) const {
		return _Left * _Right;
	}
};
template<typename Type>
struct divides : public binary_function<Type,Type,Type> {
	Type operator()( const Type& _Left,const Type& _Right ) const {
		return _Left / _Right;
	}
};
template<typename Type>
struct modulus : public binary_function<Type,Type,Type> {
	Type operator()( const Type& _Left,const Type& _Right ) const {
		return _Left % _Right;
	}
};
// logic : not, and, or
template<typename Type>
struct logic_not : public unary_function<Type,Type> {
	bool operator()(Type& _Left) const {
		return !_Left;
	}
};
template<typename Type>
struct logic_and : public binary_function<Type,Type,Type> {
	bool operator()(const Type& _Left,const Type& _Right) const {
		return _Left && _Right;
	}
};
template<typename Type>
struct logic_or : public binary_function<Type,Type,Type> {
	bool operator()(const Type& _Left,const Type& _Right) const {
		return _Left || _Right;
	}
};
// equal_to , greater_equal, greater, less_equal, less, not_equal_to
template<typename Type>
struct equal_to : public binary_function<Type,Type,bool> {
	bool operator()( const Type& _Left,const Type& _Right ) {
		return _Left == _Right;
	}
};
template<typename Type>
struct greater_equal : public binary_function<Type,Type,bool> {
	bool operator()( const Type& _Left,const Type& _Right ) {
		return _Left >= _Right;
	}
};
template<typename Type>
struct greater : public binary_function<Type,Type,bool> {
	bool operator()( const Type& _Left,const Type& _Right ) {
		return _Left > _Right;
	}
};
template<typename Type>
struct less_equal : public binary_function<Type,Type,bool> {
	bool operator()( const Type& _Left,const Type& _Right ) {
		return _Left <= _Right;
	}
};
template<typename Type>
struct less : public binary_function<Type,Type,bool> {
	bool operator()( const Type& _Left,const Type& _Right ) {
		return _Left < _Right;
	}
};
template<typename Type>
struct not_equal_to : public binary_function<Type,Type,bool> {
	bool operator()( const Type& _Left,const Type& _Right ) {
		return _Left != _Right;
	}
};
//////////////////////////////   f   //////////////////////////////////////////// 
template<typename Result>
class generator : public generate_function<Result> {
	public:
		explicit generator() {}
		Result operator()() {
			return rand();
		}
};
template<typename Arg,typename Result>
class pointer_to_unary_function : public unary_function<Arg,Result> {
	public:
		explicit pointer_to_unary_function(Result (*_pfunc)(Arg)){
			_f = _pfunc;
		}
		Result operator()(const Arg _Left) const {
			return _f(_Left);
		}
	private:
		Result (*_f)(Arg);
};
template<typename Arg1,typename Arg2,typename Result>
class pointer_to_binary_function : public binary_function<Arg1,Arg2,Result> {
	public:
		explicit pointer_to_binary_function(Result (*_pfunc)(Arg1,Arg2)) {
			_f = _pfunc;
		}
		Result operator()(const Arg1 _Left,const Arg2 _Right) const {
			return _f(_Left,_Right);
		}
	private:
		Result (*_f)(Arg1,Arg2);
};
template<typename Arg,typename Result>
pointer_to_unary_function<Arg,Result> ptr_fun(Result (*_pfunc)(Arg)) {
	return pointer_to_unary_function<Arg,Result>(_pfunc);
}
template<typename Arg1,typename Arg2,typename Result>
pointer_to_binary_function<Arg1,Arg2,Result> ptr_fun(Result (*_pfunc)(Arg1,Arg2)) {
	return pointer_to_binary_function<Arg1,Arg2,Result>(_pfunc);
}
//////////////////////////////   mf   //////////////////////////////////////////// 
// unary mf
template<typename Type,typename Result>
class men_fun_t : public unary_function<Type*,Result> {
	public:
		explicit men_fun_t(Result (Type::*_pfunc)()) {
			_f = _pfunc;
		}
		Result operator()(Type* _pLeft) const {
			return _pLeft->_f();
		}
	private:
		Result (Type::*_f)();
};
template<typename Type,typename Result>
class men_fun_ref_t : public unary_function<Type,Result> {
	public:
		explicit men_fun_ref_t(Result (Type::*_pfunc)()) {
			_f = _pfunc;
		}
		Result operator()(Type& _Left) const {
			return _Left._f();
		}
	private:
		Result (Type::*_f)();
};
template<typename Type,typename Result>
class const_men_fun_t : public unary_function<Type*,Result> {
	public:
		explicit const_men_fun_t(Result (Type::*_pfunc)() const) {
			_f = _pfunc;
		}
		Result operator()(const Type* _pLeft) const {
			return _pLeft->_f();
		}
	private:
		Result (Type::*_f)() const;
};
template<typename Type,typename Result>
class const_men_fun_ref_t : public unary_function<Type,Result> {
	public:
		explicit const_men_fun_ref_t(Result (Type::*_pfunc)() const) {
			_f = _pfunc;
		}
		Result operator()(const Type& _Left) const {
			return _Left._f();
		}
	private:
		Result (Type::*_f)() const;
};
// binary mf
template<typename Type,typename Arg,typename Result>
class men_fun1_t : public binary_function<Type*,Arg,Result> {
	 public:
		 explicit men_fun1_t(Result (Type::*_pfunc)(Arg)) {
			_f = _pfunc;
		 }
		 Result operator()(Type* _pLeft,Arg _Right) const {
			return _Left->_f(_Right);
		 }
	 private:
		 Result (Type::*_f)(Arg);
};
template<typename Type,typename Arg,typename Result>
class men_fun1_ref_t : public binary_function<Type,Arg,Result> {
	public:
		explicit men_fun1_ref_t(Result (Type::*_pfunc)(Arg)) {
			_f = _pfunc;x
		}
		Result operator()(Type& _Left,Arg _Right) const {
			return _Left._f(_Right);
		}
	private:
		Result (Type::*_f)(Arg);
};
template<typename Type,typename Arg,typename Result>
class const_men_fun1_t : public binary_function<Type*,Arg,Result> {
	public:
		explicit const_men_fun1_t(Result (Type::*_pfunc)(Arg) const) {
			_f = _pfunc;
		}
		Result operator()(const Type* _pLeft,Arg _Right) {
			return _pLeft->_f(_Right);
		}
	private:
		Result (Type::*_f)(Arg) const;
};
template<typename Type,typename Arg,typename Result>
class const_men_fun1_ref_t : public binary_function<Type,Arg,Result> {
	public:
		explicit const_men_fun1_ref_t(Result (Type::*_pfunc)(Arg) const) {
			_f = _pfunc;
		}
		Result operator()(const Type& _Left,Arg _Right) const {
			return _Left._f(_Right);
		}
	private:
		Result (Type::*_f)(Arg) const;
};
// men_fun for pointer.
template<typename Type,typename Result>
men_fun_t<Type,Result> men_fun(Result (Type::*_Pm)()) {
	return men_fun_t<Type,Result>(_Pm);
}
template<typename Type,typename Result>
const_men_fun_t<Type,Result> men_fun(Result (Type::*_Pm)() const) {
	return const_men_fun_t<Type,Result>(_Pm);
}
template<typename Type,typename Arg,typename Result>
men_fun1_t<Type,Arg,Result> men_fun(Result (Type::*_Pm)(Arg)) {
	return men_fun1_t<Type,Arg,Result>(_Pm);
}
template<typename Type,typename Arg,typename Result>
const_men_fun1_t<Type,Arg,Result> men_fun(Result (Type::*_Pm)(Arg) const) {
	return const_men_fun1_t<Type,Arg,Result>(_Pm);
}
// men_fun_ref for reference.
template<typename Type,typename Result>
men_fun_ref_t<Type,Result> men_fun_ref(Result (Type::*_Pm)()) {
	return men_fun_ref_t<Type,Result>(_Pm);
}
template<typename Type,typename Result>
const_men_fun_ref_t<Type,Result> men_fun_ref(Result (Type::*_Pm)() const) {
	return const_men_fun_ref_t<Type,Result>(_Pm);
}
template<typename Type,typename Arg,typename Result>
men_fun1_ref_t<Type,Arg,Result> men_fun_ref(Result (Type::*_Pm)(Arg)) {
	return men_fun1_ref_t<Type,Arg,Result>(_Pm);
}
template<typename Type,typename Arg,typename Result>
const_men_fun1_ref_t<Type,Arg,Result> men_fun_ref(Result (Type::*_Pm)(Arg) const) {
	return const_men_fun1_ref_t<Type,Arg,Result>(_Pm);
}
//////////////////////////////   FO   //////////////////////////////////////////// 
//binder1st //binder2nd 
template<typename Operation>
class binder1st : public unary_function<typename Operation::second_argument_type,
										typename Operation::result_type> 
{
	public: 
		typedef typename Operation::second_argument_type argument_type;
		typedef typename Operation::result_type result_type;
		explicit binder1st(const Operation& _Func,
			const typename Operation::first_argument_type& _Left ) {
			_op = _Func;
			_value = _Left;
		}
		result_type operator()(const argument_type& _Right) const {
			return _op(_value,_Right);
		}
	private:
		Operation _op;
		typename Operation::first_argument_type _value;
};
template<typename Operation>
class binder2nd : public unary_function<typename Operation::first_argument_type,
										typename Operation::result_type>
{
	public:
		typedef typename Operation::first_argument_type argument_type;
		typedef typename Operation::result_type result_type;
		explicit binder2nd(const Operation& _Func,
			const typename Operation::second_argument_type& _Right) {
			_op = _Func;
			_value = _Right;
		}
		result_type operator()(const argument_type& _Left) const {
			return _op(_Left,_value);
		}
	private:
		Operation _op;
		typename Operation::second_argument_type _value;
};
template<typename Operation>
inline binder1st<Operation> bind1st(const Operation& Func,const typename Operation::first_argument_type& Left) {
	return binder1st<Operation>(Func,Left);
}
template<typename Operation>
inline binder2nd<Operation> bind2nd(const Operation& Func,const typename Operation::second_argument_type& Right) {
	return binder2nd<Operation>(Func,Right);
}
// compose
template<typename Unary_Operation1, typename Unary_Operation2>
class unary_compose : unary_function<typename Unary_Operation2::argument_type,
																	typename Unary_Operation1::result_type> {
	Unary_Operation1 Op1;
	Unary_Operation2 Op2;
public:
	typedef typename Unary_Operation1::result_type result_type;
	typedef typename Unary_Operation2::argument_type argument_type;
public:
	explicit unary_compose( const Unary_Operation1& x, const Unary_Operation2& y ) : Op1( x ), Op2( y ) { }
	result_type operator( )( const argument_type& x ) { return Op1( Op2( x ) ); }
};
template<typename Binary_Operation1, typename Unary_Operation1, typename Unary_Operation2>
class binary_compose : unary_function<typename Unary_Operation1::argument_type,
																	typename Binary_Operation1::result_type> {
	Binary_Operation1 Op1;
	Unary_Operation1 Op2;
	Unary_Operation2 Op3;
public:
	typedef typename Unary_Operation1::argument_type argument_type;
	typedef typename Binary_Operation1::result_type result_type;
public:
	explicit binary_compose( const Binary_Operation1& x, const Unary_Operation1& y, const Unary_Operation2& z ) : 
		Op1( x ), Op2( y ), Op3( z ) { }
	result_type operator( )( const argument_type& x ) { return Op1( Op2( x ), Op3( x ) ); }
};
template<typename Unary_Operation1, typename Unary_Operation2>
inline unary_compose<Unary_Operation1, Unary_Operation2> 
	compose1( const Unary_Operation1& x, const Unary_Operation2& y ) {
		return unary_compose<Unary_Operation1, Unary_Operation2>( x, y );
}
template<typename Binary_Operation1, typename Unary_Operation1, typename Unary_Operation2>
inline binary_compose<Binary_Operation1, Unary_Operation1, Unary_Operation2>
	compose2( const Binary_Operation1& x,
						const Unary_Operation1& y,
						const Unary_Operation2& z ) {
		return binary_compose<Binary_Operation1, Unary_Operation1, Unary_Operation2>( x, y, z );
}
//unary_negate //binary_negate
template<typename Predict>
class unary_negate : public unary_function<typename Predict::argument_type,bool> {
	Predict _pre;
public:
	typedef typename Predict::argument_type argument_type;
	explicit unary_negate( const Predict& p ) : _pre( p ) {}
	bool operator()( const argument_type& pm ) {
		return !_pre( pm );
	}
};
template<typename Operation>
class binary_negate : public binary_function<typename Operation::first_argument_type,
																	typename Operation::second_argument_type,
																	bool>
{
	Operation _op;
public:
	typedef typename Operation::first_argument_type first_argument_type;
	typedef typename Operation::second_argument_type second_argument_type;
	explicit binary_negate( const Operation& p ) : _op( p ) {}
	bool operator()( const first_argument_type& Left,
							const second_argument_type& Right ) {
		return !_op( Left,Right );
	}
};
template<typename Predict>
bool not1( const Predict& p ) {
	return unary_negate<Predict>( p );
}
template<typename Operation>
bool not2( const Operation& op ) {
	return binary_negate<Operation>( op );
}
// identity.
template<typename T>
struct identity : public unary_function<T,T> {
	const T& operator( )( const T& x ) { return x; }
};
template<typename Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type> {
	const typename Pair::first_type operator( )( const Pair& x ) const { return x.first; }
};
template<typename Pair>
struct select2nd : public unary_function<Pair, typename Pair::second_type> {
	const typename Pair::second_type operator( )( const Pair& x ) const { return x.second; }
};
template<typename Arg1, typename Arg2>
struct project1st : public binary_function<Arg1, Arg2, Arg1> {
	Arg1 operator( )( const Arg1& x, const Arg2& ) const { return x; }
};
template<typename Arg1, typename Arg2>
struct project2nd : public binary_function<Arg1, Arg2, Arg2> {
	Arg2 operator( )( const Arg1&, const Arg2& x ) const { return x; }
};

} // namespace safe.
#endif // FUNCTIONAL_S_H