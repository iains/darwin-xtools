#include <CoreFoundation/CFString.h>

extern void bar();

extern CFStringRef OtherCFString;

void 
foo (void)
{
	CFStringGetLength(CFSTR("hello"));
	CFStringGetLength(CFSTR("world"));
	CFStringGetLength(OtherCFString);
}


int 
main (void) 
{
	CFStringGetLength(CFSTR("live"));
	bar();
	return 0; 
}


