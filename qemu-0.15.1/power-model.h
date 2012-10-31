#ifndef POWER_MODEL_H
#define POWER_MODEL_H
#include "qemu-common.h"

#define DEFAULT_FREQUENCY 400

struct energy_counter;

typedef struct {
	void (*freq_change_notify) (uint64_t);	
	uint64_t (*read_active_power) (void *);
	void (*sleep_start_notify) (void);
	void (*sleep_stop_notify) (uint64_t);
	uint64_t (*read_sleep_power) (void *);
	uint64_t (*insn_power) (uint32_t);
	void (*change_parameter) (const char *, const char *);	
}PowerModel;

void power_init(void);

#endif
