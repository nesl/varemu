#include <asm/io.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <mach/platform.h>
#include <asm/uaccess.h>

#define VARMOD_BASE                    0x101F5000       /* Variability Module */

MODULE_LICENSE("Dual BSD/GPL");

uint8_t *base_pointer;

asmlinkage long sys_var_reg_read(int offset, uint64_t __user *dest) {

	uint32_t lo = 0;
	uint32_t hi = 0;
	uint64_t val = 0;
    
    if ((offset > 0x1000) | (offset % 8)) {
        return -EFAULT;
    }
    
    lo = ioread32((uint32_t*)(base_pointer + offset));
    hi = ioread32((uint32_t*)(base_pointer + offset + 4));
    
    val = hi;
    val = val<<32;
    val += lo;

	//printk("hi %u\t", hi);
	//printk("lo %u\t", lo);
	//printk("val %llu\n", val);
	

	if (copy_to_user(dest, &val, sizeof(val)))
                return -EFAULT;
    //printk("dest %llu", *dest);          
           
	return 0;
}

asmlinkage long sys_var_reg_write(int offset, uint64_t val) {
    
   
    if ((offset > 0x1000) | (offset % 8)) {
        return -EFAULT;
    }
    
    iowrite32((uint32_t)val, (uint32_t*)(base_pointer + offset));
    iowrite32((uint32_t)(val>>32), (uint32_t*)(base_pointer + offset));
    
    
    
	return 0;
    
}

static int __init variability_init(void)
{
    base_pointer = ioremap(VARMOD_BASE, 0x1000);
    printk("Variability module init, Base Pointer=%p\n", base_pointer);
    return 0;
}

static void variability_exit(void)
{

}

module_init(variability_init);
module_exit(variability_exit);

