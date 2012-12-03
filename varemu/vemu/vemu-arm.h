//
//  vemu-arm.h
//  
//
//  Created by Lucas Wanner on 11/16/12.
//
//

#ifndef _vemu_arm_h
#define _vemu_arm_h

#ifdef 	VEMU


#include "qemu-common.h"
#include "vemu.h" 

vemu_instr_info* vemu_target_get_all_instr_info(bool thumb);
int vemu_target_get_all_instr_size(bool thumb);
void vemu_target_decode_instr(uint32_t instr, bool thumb, vemu_tb_info * info);

#endif // VEMU
#endif // _vemu_arm_h
