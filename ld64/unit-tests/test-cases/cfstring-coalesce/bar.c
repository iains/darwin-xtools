#include <CoreFoundation/CFString.h>

CFStringRef OtherCFString = CFSTR("other");

void 
bar (void)
{
	CFStringGetLength(CFSTR("live"));
}
