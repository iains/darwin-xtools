#include <stdio.h>
#include <stdlib.h>


extern int foo();

int 
main (void)
{
	int (*func)() = foo;
	if ( func != NULL )
		(*func)();
	return 0;
}
