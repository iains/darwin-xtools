
extern int b;
extern void func();

int 
test_bind (void) 
{ 
	func();
	return b; 
}

