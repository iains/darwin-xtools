

// put test code in a dylib so that is slides
extern void realmain();

int 
main (void)
{
	realmain();
	return 0;
}


