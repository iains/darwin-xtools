extern void bar();
extern int result;

int myresult = 1;

int 
mymain (void)
{
  bar();
  return result;
}

void 
mybar (void)
{

}

