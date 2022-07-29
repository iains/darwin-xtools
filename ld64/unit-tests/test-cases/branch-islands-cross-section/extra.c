#include <stdio.h>

extern void back();

__attribute__((section("__TEXT,__cold,regular,pure_instructions")))
void foo()
{
	fprintf(stdout, "foo\n");
  back();
}

