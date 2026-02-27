
extern void bar();

int 
main (void)
{
#if	CALL_BAR
	bar();
#endif
	return 0;
}
