
#ifdef __cplusplus
extern "C" {
#endif
#if defined (__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__) \
      && __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ <= 1040

/* Don't try a backtrace on earlier systems.  */

void inline
__assert_rtn(const char* func, const char* file, int line, const char* failedexpr)
{
  fprintf(stderr, "Assertion failed: (%s), function %s, file %s, line %d.\n", failedexpr, func, file, line);
  exit (1);
}
#else

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <cxxabi.h>

#include <execinfo.h>
// implement assert() function to print out a backtrace before aborting
void inline
__assert_rtn(const char* func, const char* file, int line, const char* failedexpr)
{
	fprintf(stderr, "Assertion failed: (%s), function %s, file %s, line %d.\n", failedexpr, func, file, line);

	void* callStack[128];
	int depth = ::backtrace(callStack, 128);
	char* buffer = (char*)malloc(1024);
	for(int i=0; i < depth-1; ++i) {
		Dl_info info;
		dladdr(callStack[i], &info);
		const char* symboName = info.dli_sname;
		if ( (symboName != NULL) && (strncmp(symboName, "_Z", 2) == 0) ) {
			size_t bufLen = 1024;
			int result;
			char* unmangled = abi::__cxa_demangle(symboName, buffer, &bufLen, &result);
			if ( unmangled != NULL )
				symboName = unmangled;
		}
		long offset = (uintptr_t)callStack[i] - (uintptr_t)info.dli_saddr;
		fprintf(stderr, "%d  %p  %s + %ld\n", i, callStack[i], symboName, offset);
	}
	exit(1);
}
#  endif /* 10.4 or earlier.  */
#ifdef __cplusplus
}
#endif
