#include <stdio.h>
#include "errors.h"

volatile int x = 3;
volatile int y = 9;
volatile int z = 1;

int main() {
//	enable_errors();
	volatile int i;
	int sum = 0;
	for (i = 0; i < 100; i++) {
		z = x * y;
		sum = sum + z;
	}
	
		printf("%d\n", sum);
}


