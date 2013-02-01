#include <stdio.h>
 
volatile int x;

int main() {
	int i;
	while (1) {
		for (i = 0; i < 0xFFFFF; i++) {
			x += i;	
		}
	}
	printf("Hello World!\n");
	return 0;
}

