
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

double 
getpi (void) { return 3.1415926535; }

void 
foo (void) 
{
}

void 
bar (void) 
{
}

extern void* __dso_handle;
void* x = &__dso_handle;

int abc = 10;

int def = 20;

int ghi = 30;

int com;

int com3;
int com4;
int com5;

