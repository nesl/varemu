//
//  vemu.h
//  
//
//  Created by Lucas Wanner on 11/15/12.
//
//

#ifndef _vemu_h
#define _vemu_h

#ifdef 	VEMU

#define VEMU_DEBUG	

#ifdef VEMU_DEBUG
    #define vemu_debug(...) fprintf(stdout, __VA_ARGS__)
#else
    #define vemu_debug(...) 
#endif

#include "qemu-common.h"
#include "qjson.h"
#include "qint.h"
#include "qdict.h"

#define VEMU_ERRORS_NONE        0x0
#define VEMU_ERRORS_REPLACE     0x1
#define VEMU_ERRORS_PRE         0x2
#define VEMU_ERRORS_POST        0x4

typedef struct 
{
	uint32_t opcode;
	const char * name;
	uint8_t class;	
	uint8_t cycles;
	bool errors;
} vemu_instr_info;

typedef struct
{
	uint32_t instr_word;
    vemu_instr_info instr_info;
} vemu_tb_info;


void vemu_init(const char *fname);
int vemu_do_change_model_param(Monitor *mon, const QDict *qdict, QObject **ret_data);
int vemu_do_info_time(Monitor *mon, const QDict *qdict, QObject **ret_data);
int vemu_do_info_energy(Monitor *mon, const QDict *qdict, QObject **ret_data);
int vemu_do_info_all(Monitor *mon, const QDict *qdict, QObject **ret_data);


#endif // VEMU
#endif // _vemu_h
