
int foo()
{
	return 0;
}

			
#if __arm__ 
	#define INSTALL_NAME_4_0(sym) \
                 extern const char install_name_4_0 __asm("$ld$install_name$os4.0$" #sym ); const char install_name_4_0 = 0;
				
	INSTALL_NAME_4_0(/usr/lib/libfoo.dylib)
#elif defined (__clang__)
/* Clang adds its own quoting and takes the user's original quotes and renders them
   as _22_ pre/ap-ended to the actual symbol which breaks the test.  */
	#define INSTALL_NAME_10_4(sym) \
                 extern const char install_name_10_4 __asm("$ld$install_name$os10.4$" #sym ); const char install_name_10_4 = 0;
				
	INSTALL_NAME_10_4(/usr/lib/libfoo.dylib)
#else
	#define INSTALL_NAME_10_4(sym) \
                 extern const char install_name_10_4 __asm("\"$ld$install_name$os10.4$" #sym "\""); const char install_name_10_4 = 0;
				
	INSTALL_NAME_10_4(/usr/lib/libfoo.dylib)
#endif
