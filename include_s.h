/**********************************************************
 * File£º include_s.h
 *
 * Author£º shenglong (shenglong@vip.126.com)
 *
 * Description£ºthis file contains several head file of namespace std,
 *						It should include this file which need std components.
 *	
 *	State : handling.
 * 
 * 
 * 
 */
#ifndef INCLUDE_S_H
#define INCLUDE_S_H
#ifndef UNIX_MAX
#define UNIX_MAX 4096
#endif
#ifndef AUTO_CMD
#define AUTO_CMD
#endif
#include "stdafx.h"

// std headers.
#include <new>				// for ::operator new(), 
									// ::operator delete(), 
									// set_new_handler.
#include <cstddef>		// for malloc and free.
#include <cctype>			// for
#include <typeinfo>		//
#include <cassert>		// for assert().
#include <cstdlib>			// for max and min.
#include <iostream>		// for cout and endl.
#include <exception>		// for std::exception.
#include <stdexcept>		// 
#include <numeric>		// 
#include <algorithm>	// for copy and transform.
#include <string>			// for string.

#endif // INCLUDE_S_H