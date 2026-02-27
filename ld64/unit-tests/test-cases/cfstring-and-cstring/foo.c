#include <CoreFoundation/CFString.h>

extern void bar();

int 
main (void) 
{
	CFStringGetLength(CFSTR("stuff"));
	bar();
	return 0; 
}


