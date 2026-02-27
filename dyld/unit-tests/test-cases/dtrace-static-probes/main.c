
#include <stdio.h>

#include "foo.h"


int 
main (void) {
	for (int i=0; i < 5; ++i) {
		FOO_COUNT(i);
	}

	return 0;
}
