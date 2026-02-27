
extern void foo();
extern void bar();
extern void bar_extra();

int 
main (void)
{
	foo();
	bar();
#if BAR_EXTRA
	bar_extra();
#endif
	return 0;
}

