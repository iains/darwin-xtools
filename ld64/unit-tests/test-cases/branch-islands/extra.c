#include <stdio.h>

extern void back();

void 
foo (void)
{
	fprintf(stdout, "foo\n");
  back();
}

