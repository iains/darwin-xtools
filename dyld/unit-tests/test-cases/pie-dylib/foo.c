#include <stdio.h>
#include <stdbool.h>

void 
foo (void)
{
	printf("&foo=%p\n", &foo);
}
