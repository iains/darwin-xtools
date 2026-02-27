const char* mystring = "hello";

int var = 10;

const char *
incget (void) {
  ++var;
  return mystring;
}

