extern void* otherget();

void 
mm (void) 
{
}

static void 
s1 (void) {
  mm();
}

static void 
s2 (void) {
  mm();
}

int 
main (void)
{
  s1();
  s2();
	return 0;
}

const char *
version (void) { return "1.0"; }

static int 
mylocal (void)
{
  return 0;
}

void *
mainget (void) { return mylocal; }


int def = 20;

int ghi = 30;

int com;

