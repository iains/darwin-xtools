
void 
foo (void) {}

void 
mid (void) {}

static void 
bar (void) { foo(); }

int 
main (void) { bar(); return 0; }


#if __STATIC__
void 
myexit (void) {}
#endif

