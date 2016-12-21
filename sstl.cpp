// sstl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "include_s.h"
#ifdef AUTO_CMD
	// print name and result.
	#define D(A) std::cout<< "execute ----- " << #A << " ----- result ----- "<< ( A ) << std::endl;
#else
	#include <afxwin.h>
	// print result.
	#define D(A) std::cout<< A << std::endl; 
#endif
// customized head file
#include "iterator_s.h"
#include "memory_s.h"
#include "algorithm_s.h"
#include "functional_s.h"
#include "utility_s.h"

using namespace safe;

int main(int argc, char* argv[]) {

	return 0;
}