#include <stdio.h>

extern void my_weak();
extern int my_tent;

int 
main (void)
{
	my_tent = 0;
	my_weak();
	return 0;
}

