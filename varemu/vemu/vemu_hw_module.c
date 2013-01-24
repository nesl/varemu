//
//  vemu_hw_module.c
//  
//
//  Created by Lucas Wanner on 11/21/12.
//  

#ifdef	VEMU

#include <stdio.h>
#include <math.h>
#include "vemu.h"
#include "qemu-common.h"
#include "vemu-cycles.h"
#include "vemu-energy.h"
#include "hw/sysbus.h"

#define	READ_NOT_PENDING	0xFFFFF 
#define VEMU_MOD_SIZE		0x1000
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


typedef struct {
	SysBusDevice busdev;
	qemu_irq irq;
    MemoryRegion iomem;	
} vemu_mod;

static uint64_t last_offset = READ_NOT_PENDING;
static uint64_t last_read = 0;

extern uint64_t vemu_errors_enabled;

static uint64_t vemu_mod_read(void *opaque, target_phys_addr_t offset, unsigned size)
{
	
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
		if ((offset > VEMU_MOD_SIZE) | (offset % 8)) {
			printf("VarMod: Invalid offset: %x\n", (uint32_t)offset);
			return 0;
		}
		last_offset = offset;
	}

	if (offset < ACT_TIME_HI) {
		int idx = (offset - ACT_TIME_LO)/8;
		last_read = vemu_get_act_time(idx);
		//printf("offset: %d, return = %u\n", offset, (uint32_t)last_read);
		return (uint32_t)last_read;
	}

	if (offset < ACT_EN_HI) {
		int idx = (offset - ACT_EN_LO)/8;
		last_read = vemu_get_act_energy(idx);
		//printf("offset: %d, return = %u\n", offset, (uint32_t)last_read);
		return (uint32_t)last_read;
	}
	
	switch (offset) {
		case TOTAL_ACT_TIME : {
			last_read = vemu_get_act_time_all_classes();
		} break;
		case TOTAL_SLP_TIME : {
			last_read = vemu_get_slp_time();
		} break;
		case TOTAL_ACT_ENERGY : {
			last_read = vemu_get_act_energy_all_classes();
		} break;
		case TOTAL_SLP_ENERGY : {
			last_read = vemu_get_slp_energy();
		} break;
		case TOTAL_CYCLES : {
			last_read = vemu_get_cycles_all_classes();
			vemu_debug("total_cycles: %llu\n", last_read);
		} break;		
		case ERRORS_EN : {
			last_read = vemu_errors_enabled;
		} break;
	}
	return (uint32_t)last_read;
	
}
static void vemu_mod_write(void *opaque, target_phys_addr_t offset,
                                 uint64_t val, unsigned size)
{
	switch (offset) {
		case ERRORS_EN : {
            
			if(val != vemu_errors_enabled) {
				vemu_debug("vemu_errors_enabled = %llu\n", (unsigned long long)val);
			}
             
			vemu_errors_enabled = val;
		} break;
        case VEMU_EXIT : {
			printf("Killing QEMU. I hope you unmounted all network filesystems...\n");
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
