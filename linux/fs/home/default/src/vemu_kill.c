#include <stdio.h>
#include "vemu.h"

volatile int x = 3;
volatile int y = 9;
volatile int z = 1;

int main() {
	vemu_kill();
	return 0;
}
