
extern int foo (int);
extern int bar (int);

int main (int ac, char *av[])
{
  int x = foo (ac);
  return bar (x);
}