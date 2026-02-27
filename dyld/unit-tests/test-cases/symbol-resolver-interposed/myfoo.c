
#include <mach-o/dyld-interposing.h>
#include "foo.h"

int 
myfoo (void)
{
	foo();
	return 20;
}

DYLD_INTERPOSE(myfoo, foo)
