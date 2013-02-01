all:
	arm-none-eabi-gcc -mthumb -march=armv7 -mfix-cortex-m3-ldrd -T lm3s6965.ld main.c reset.S syscalls.c -o ../bin/main
	arm-none-eabi-objcopy -O binary ../bin/main ../bin/main.bin

