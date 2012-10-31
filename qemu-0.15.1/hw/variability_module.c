#include "qemu-variability.h"
#include "sysbus.h"

#define N_CLASSES			10

#define	READ_NOT_PENDING	0xFFFFF 
#define VARMOD_SIZE			0x1000
#define CYCLES_LO			0x000
#define CYCLES_HI			0x280
#define DYN_EN_LO			0x280
#define DYN_EN_HI			0x500
#define TOTAL_CYCLES		0x500
#define TOTAL_ACT_ENERGY	0x540
#define TOTAL_SLP_ENERGY	0x580
#define ACT_ERRORS			0xFC0



typedef struct {
	SysBusDevice busdev;
	qemu_irq irq;
} varmod;

static void varmod_reset(varmod *s)
{
	reset_all_cycle_counters();
}

static uint64_t last_offset = READ_NOT_PENDING;
static uint64_t last_read = 0;

static uint32_t varmod_read(void* opaque, target_phys_addr_t offset)
{
	struct cycle_counter cy;
	struct energy_counter active, sleep;
	calculate_active_energy(&active);
	calculate_sleep_energy(&sleep);
	read_all_cycle_counters(&cy);
	
	if (last_offset != READ_NOT_PENDING) {
		if ((offset - 4) != last_offset) {
			printf("VarMod: Can't read this register, read pending for offset: %x\n", (uint32_t)(last_offset+4));
			return 0;
		} else {
			// we'll complete the read here, clear read pending
			last_offset = READ_NOT_PENDING;	
			return (uint32_t)(last_read >> 32);
		}
	} else {
		if ((offset > VARMOD_SIZE) | (offset % 8)) {
			printf("VarMod: Invalid offset: %x\n", (uint32_t)offset);
			return 0;
		}
		last_offset = offset;
	}

	if (offset < CYCLES_HI) {
		int idx = (offset - CYCLES_LO)/8;
		last_read = cy.cycle_count[idx];
		//printf("offset: %d, return = %u\n", offset, (uint32_t)last_read);
		return (uint32_t)last_read;
	}

	if (offset < DYN_EN_HI) {
		int idx = (offset - DYN_EN_LO)/8;
		last_read = active.insn_energy[idx];
		//printf("offset: %d, return = %u\n", offset, (uint32_t)last_read);
		return (uint32_t)last_read;
	}
	
	switch (offset) {
		case TOTAL_CYCLES : {
			int i;
			last_read = 0;
			for (i = 0; i < N_CLASSES; i++) {
				last_read += cy.cycle_count[i];
			}		
		} break;
		case TOTAL_ACT_ENERGY : {
			int i;
			last_read = 0;
			for (i = 0; i < N_CLASSES; i++) {
				last_read += active.insn_energy[i];
			}			
		} break;
		case TOTAL_SLP_ENERGY : {
			last_read = sleep.sleep_energy;
		} break;
		case ACT_ERRORS : {
			last_read = errors_activated;
		} break;
	}

	return (uint32_t)last_read;
	
}

static void varmod_write(void *opaque, target_phys_addr_t offset, uint32_t value)
{
	switch(offset)
	{
		case 0x70: 
					errors_activated = (value & 1);
					break;
	}
}

static CPUReadMemoryFunc* const varmod_readfn[] = {
	varmod_read,
	varmod_read,
	varmod_read,
};

static CPUWriteMemoryFunc* const varmod_writefn[] = {
	varmod_write,
	varmod_write,
	varmod_write,
};

static int varmod_init(SysBusDevice *dev)
{
	int iomemtype;
	varmod* s = (varmod*) qemu_mallocz(sizeof(varmod));
	sysbus_init_irq(dev, &s->irq);
	varmod_reset(s);
	iomemtype = cpu_register_io_memory(varmod_readfn, varmod_writefn, s,
									DEVICE_NATIVE_ENDIAN);
	sysbus_init_mmio(dev, VARMOD_SIZE, iomemtype);
	power_init();
	return 0;
}

static SysBusDeviceInfo varmod_info = {
	.init = varmod_init,
	.qdev.name = "varmod",
	.qdev.size = sizeof(varmod),
};

static void varmod_register_devices(void)
{
	sysbus_register_withprop(&varmod_info);
}

device_init(varmod_register_devices);
