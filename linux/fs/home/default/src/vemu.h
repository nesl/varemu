#ifndef errors_h
#define errors_h

#include <unistd.h>


#define VAR_SYSCALL_READ 	378
#define VAR_SYSCALL_WRITE 	379

#define ACT_TIME_LO			0x000
#define ACT_TIME_HI			0x280
#define ACT_EN_LO			0x280
#define ACT_EN_HI			0x500
#define TOTAL_ACT_TIME		0x500
#define TOTAL_SLP_TIME		0x540
#define TOTAL_ACT_ENERGY	0x580
#define TOTAL_SLP_ENERGY	0x5C0
#define TOTAL_CYCLES		0x600
#define ERRORS_EN			0xFC0
#define VEMU_EXIT           0xFD0

unsigned long long vemu_read(unsigned int reg) {
	unsigned long long tempv = 0;
	syscall(VAR_SYSCALL_READ, reg, &tempv);
	return tempv;
}

void vemu_write(unsigned int reg, unsigned long long val) {
	unsigned long long int tempv;
	tempv = val;
	syscall(VAR_SYSCALL_WRITE, reg, &tempv);
}

unsigned long long vemu_read_offset(unsigned int base, unsigned int offset) {
	unsigned int off = offset;
	return vemu_read(off + base);
}

unsigned long long vemu_get_act_time(unsigned int class) {
	return vemu_read_offset(ACT_TIME_LO, class);
}

unsigned long long vemu_get_act_energy(unsigned int class) {
	return vemu_read_offset(ACT_EN_LO, class);
}

unsigned long long vemu_get_total_cycles() {
	return vemu_read(TOTAL_CYCLES);
}

unsigned long long vemu_get_total_act_time() {
	return vemu_read(TOTAL_ACT_TIME);
}

unsigned long long vemu_get_total_slp_time() {
	return vemu_read(TOTAL_SLP_TIME);
}

unsigned long long vemu_get_total_act_energy() {
	return vemu_read(TOTAL_ACT_ENERGY);
}

unsigned long long vemu_get_total_slp_energy() {
	return vemu_read(TOTAL_SLP_ENERGY);
}

void vemu_enable_errors(unsigned long long idx) {
	vemu_write(ERRORS_EN, idx);
}

void vemu_disable_errors() {
	vemu_write(ERRORS_EN, 0);
}

void vemu_kill() {
	vemu_write(VEMU_EXIT, 1);
}

#endif
