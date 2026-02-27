
int a;
int b = 5;
int* pa = &a;
int* pb = &b;

int 
foo (void)
{
	*pa = 4;
	return a+b;
}


int 
entry (void)
{
	return foo();
}


