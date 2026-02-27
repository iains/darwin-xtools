#include <stdio.h>

extern void foo();

int 
main (void)
{
	fprintf(stdout, "hello\n");
  foo();
	return 0;
}

void 
back (void)
{
}