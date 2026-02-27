extern void b();
extern void bb();

extern void func(void*);


void 
c (void)
{
	func(&b);
	func(&bb);
}