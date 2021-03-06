//
//  vemu_hw_module.c
//  
//
//  Created by Lucas Wanner on 11/21/12.
//  

#ifdef	VEMU

#include <stdio.h>
#include <math.h>
#include "qemu-common.h"
#include "vemu-cycles.h"
#include "vemu-energy.h"
#include "hw/sysbus.h"
#include "vemu.h"


#define	READ_NOT_PENDING	0xFFFFF 
#define VEMU_MOD_SIZE		0x1000


#define SET_FAULTS				(0x000)
#define SET_FREQUENCY			(SET_FAULTS+8)	
#define SET_VOLTAGE				(SET_FREQUENCY+8)
#define READ_CMD       			(0xD00)
#define EXIT_CMD           		(0xF00)




typedef struct {
	SysBusDevice busdev;
	qemu_irq irq;
    MemoryRegion iomem;	
} vemu_mod;

typedef struct {
	uint64_t act_time[MAX_INSTR_CLASSES];
	uint64_t act_energy[MAX_INSTR_CLASSES];
	uint64_t cycles[MAX_INSTR_CLASSES];    
	uint64_t total_act_time;
	uint64_t total_act_energy;
	uint64_t total_cycles;  
	uint64_t slp_time;
	uint64_t slp_energy;  
	uint64_t fault_status;
	uint64_t frequency;
	uint64_t voltage;
} vemu_regs;

#define VEMU_STATE_N_VARS	(MAX_INSTR_CLASSES*3+8)


typedef union {
	vemu_regs variables;
	uint8_t  array8[VEMU_STATE_N_VARS*8];
	uint32_t array32[VEMU_STATE_N_VARS*2];
	uint64_t array64[VEMU_STATE_N_VARS];
} vemu_state;

vemu_state last_state;

extern uint64_t vemu_errors_enabled;

static uint64_t vemu_mod_read(void *opaque, hwaddr offset, unsigned size)
{
	uint64_t rv;
	//vemu_debug("offset: %x, size: %x, val: \n", (int)offset, (int)size);
	assert(offset <= VEMU_STATE_N_VARS * 8);
	memcpy(&rv, &(last_state.array8[offset]), size);
	return rv;	
}

static void vemu_mod_write(void *opaque, hwaddr offset,
                                 uint64_t val, unsigned size)
{
	switch (offset) {
		case SET_FAULTS : {   
			vemu_errors_enabled = val;
		} break;
		case SET_FREQUENCY : {
			vemu_pm_change_parameter(0,0,(double)val);
		}  break;
		case SET_VOLTAGE : {
			vemu_pm_change_parameter(0,1,((double)val)/1000.0);
		}  break;		
		case READ_CMD : {
			int i;
			for (i = 0; i < MAX_INSTR_CLASSES; i++) {
				last_state.variables.act_time[i] = vemu_get_act_time(i);
				last_state.variables.act_energy[i] = vemu_get_act_energy(i);
				last_state.variables.cycles[i] = vemu_get_cycles(i);
			}		
			last_state.variables.total_act_time = vemu_get_act_time_all_classes();
			last_state.variables.total_act_energy = vemu_get_act_energy_all_classes();
			last_state.variables.total_cycles = vemu_get_cycles_all_classes();
			last_state.variables.slp_time = vemu_get_slp_time();
			last_state.variables.slp_energy =	vemu_get_slp_energy();
			last_state.variables.fault_status = vemu_errors_enabled;
			last_state.variables.frequency = vemu_pm_get_parameter(0,0);
			last_state.variables.voltage = vemu_pm_get_parameter(0,1)*1000;
		} break;		
        case EXIT_CMD : {
			printf("Killing QEMU.\n");
            exit(-1);
        } break;		
	}
}

static const MemoryRegionOps vemu_mod_ops = {
    .read = vemu_mod_read,
    .write = vemu_mod_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};


static int vemu_mod_init(SysBusDevice *dev)
{
    vemu_mod *s = FROM_SYSBUS(vemu_mod, dev);
    memory_region_init_io(&s->iomem, &vemu_mod_ops, s, "vemu", 0x1000);
    sysbus_init_mmio(dev, &s->iomem);	
	sysbus_init_irq(dev, &s->irq);	
	return 0;
}



static void vemu_mod_class_init(ObjectClass *klass, void *data)
{
	
    DeviceClass *dc = DEVICE_CLASS(klass);
    SysBusDeviceClass *k = SYS_BUS_DEVICE_CLASS(klass);

    k->init = vemu_mod_init;
    dc->no_user = 1;	

}

static TypeInfo vemu_mod_info = {
    .name          = "vemu_mod",
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(vemu_mod),
    .class_init    = vemu_mod_class_init,
};

static void  vemu_mod_register_types(void)
{
    type_register_static(&vemu_mod_info);
}


type_init(vemu_mod_register_types)


#endif
