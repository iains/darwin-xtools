extern int foo();
extern int bar();
extern int bar_weak();

int 
main (void)
{
	foo();
	bar();
	bar_weak();
	return 0;
}
