
int a = 0;
int 
func_a (void) { return a; }

// add code that will cause stack canary 
extern void fill(char*);
void 
test (void)
{
	char buf[100];
	fill(buf);
}

