extern int foo2(void);
extern int foo3(void);

int 
foo1 (void)
{
	int i = 42;
	if (foo2())
	  i = foo3();
	return i;
}
