/*
 * variability.h
 *
 * VarEMU driver
 * Lucas Wanner, 01/23/13
 *
 */

#ifndef _VARIABILITY_H_
#define _VARIABILITY_H_

#define VARMOD_BASE                 0x101F5000       /* Variability Module */
#define VARMOD_ACT_TIME_LO			0x000
#define VARMOD_ACT_TIME_HI			0x280
#define VARMOD_ACT_EN_LO			0x280
#define VARMOD_ACT_EN_HI			0x500
#define VARMOD_TOTAL_ACT_TIME		0x500
#define VARMOD_TOTAL_SLP_TIME		0x540
#define VARMOD_TOTAL_ACT_ENERGY     0x580
#define VARMOD_TOTAL_SLP_ENERGY     0x5C0
#define VARMOD_TOTAL_CYCLES         0x600
#define VARMOD_ERRORS_EN			0xFC0
#define VARMOD_VEMU_EXIT            0xFD0

#define VARMOD_NUMBER_ICLASSES      10

typedef struct {
    uint64_t vemu_act_time[VARMOD_NUMBER_ICLASSES];
    uint64_t vemu_act_energy[VARMOD_NUMBER_ICLASSES];
    uint64_t vemu_total_act_time;
    uint64_t vemu_total_cycles;
    uint64_t vemu_error_status;
} vemu_status;

extern void vemu_save_error_status(vemu_status * proc);
extern void vemu_restore_error_status(vemu_status * proc);


#endif /* _VARIABILITY_H_ */
