
extern const char* mystring;

const char** myp = &mystring;

int com;

const char *
inc (void) {
  ++com;
  return "";
}


