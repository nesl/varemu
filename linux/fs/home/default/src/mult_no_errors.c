#include <stdio.h>
#include "vemu.h"

volatile int x = 3;
volatile int y = 9;
volatile int z = 1;

int main() {
	//vemu_enable_errors(1);
	volatile int i;
	int sum = 0;
	for (i = 0; i < 100; i++) {
		z = x * y;
		sum = sum + z;
	}

	//vemu_disable_errors();
	printf("%d\n", sum);
		
	return 0;
}
