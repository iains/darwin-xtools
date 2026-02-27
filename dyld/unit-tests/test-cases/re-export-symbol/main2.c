
#include <stdio.h>  // fprintf(), NULL
#include <stdlib.h> // exit(), EXIT_SUCCESS
#include <dlfcn.h>

#include "test.h" // PASS(), FAIL(), XPASS(), XFAIL()



extern int foo();
extern int mybar();

int (*pmybar)() = &mybar;

int 
main (void)
{
	if ( foo() != 10 )
		FAIL("re-export-symbol: foo() returned wrong value");
	if ( mybar() != 10 )
		FAIL("re-export-symbol: mybar() returned wrong value");
	if ( (*pmybar)() != 10 )
		FAIL("re-export-symbol: (*pmybar)() returned wrong value");
	PASS("re-export-symbol");
	return 0;
}

