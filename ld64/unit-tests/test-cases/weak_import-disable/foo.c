


void 
foo (void) {}


#define WEAK_IMPORT_FOR_10_7(sym) \
                 extern const char sym##_tmp __asm("$ld$weak$os10.7$_" #sym ); const char sym##_tmp = 0;

void 
bar (void) {}
WEAK_IMPORT_FOR_10_7(bar)

