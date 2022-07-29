#include <stdio.h>

extern int back();

int foo()
{
  fprintf(stdout, "foo\n");
  return back();
}

