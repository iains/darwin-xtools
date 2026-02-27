#include <stdio.h>

extern int myglobal;
extern void myfunction(int);

int 
main (void)
{
	myfunction(myglobal);
	return 0;
}

