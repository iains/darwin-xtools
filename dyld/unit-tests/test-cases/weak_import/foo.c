

#include "foo.h"

void 
func1 (void) {}
void 
func3 (void) {}
int data1 = 0;
int data3;
int data5 = 0;

#if ALL_SYMBOLS
void 
func2 (void) {}
void 
func4 (void) {}

int data2 = 0;	// weak_import initialized
int data4;		// weak_import uninitialized
int data6 = 0;	// weak_import 
#endif

