#include <stdio.h>

extern int foo();

int main()
{
  fprintf(stdout, "hello\n");
  return foo() - 42;
}

int back()
{
  fprintf(stdout, "back\n");
  return 42;
}