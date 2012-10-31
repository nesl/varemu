#include "qdict.h"
#include "qemu-variability.h"
#include "qint.h"
#include "qlist.h"
#include "qobject.h"

static uint8_t count_exec = 0;

#if defined(TARGET_ARM)
#define BLX_imm 9
#define BLX_reg 10
#define BX 11
#define B 59

// Register Error Types
#define REG_READ_ERROR 1
#define REG_WRITE_ERROR 2
#define STUCK_AT_ZERO 1

static const char* regnames[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", 
	"r8", "r9", "r10", "r11", "r12", "r13", "r14", "pc"}; 

static bool isinsnbranch(uint16_t insn)
{
	if(insn == BLX_imm || insn == BLX_reg || insn == BX || insn == B)
		return true;
	return false;
}
#endif



int error_model(CPUState * env, TranslationBlock* tb)
{
	int i;
	count_exec ++;
	int32_t input_regs[4] = {-1}, output_reg = -1;
	bool branchinsn = false;
	/*
		if(count_exec == 10)
		{
			env->regs[15] += 2;
			return true;
		}
	*/
	printf("\nINSN : %s", insn_map[tb->insn_under_exec].instruction);
	for(i = 0; i < 8; i += 2) {
		if(tb->args[i] == 0)
			printf(" Reg Used : %d", tb->args[i+1]);
		else if(tb->args[i] == 1)
				printf(" Immediate Offset: 0x%08x", tb->args[i+1]);
	}

	branchinsn = isinsnbranch(tb->insn_under_exec);
	
	if(!branchinsn) {
		// TODO (gdrane)
		int i, k = 0;
		if(tb->args[0] == 0)
			output_reg = tb->args[1];
		for( i = 2; i < 8 ; i+=2)
			if(tb->args[i] == 0)
				input_regs[k++] = tb->args[i + 1]; 
	}

	//
	//Register Errors
	//
	if(error_info_regs != NULL && qlist_empty(error_info_regs))
	{
		int i;
		for(i = 0; i < 4; ++i)
		{
			const QListEntry* qlistentry = qlist_first(error_info_regs);
			const char* reg = regnames[input_regs[i]];
			QDict* qdict;
			do {
				QObject* obj = qlistentry->value;
				qdict = qobject_to_qdict(obj);
				if(!qdict_haskey(qdict, "reg_name")) {
					printf("ERROR: String not valid\n");
					exit(1);
				}
				qlistentry = qlist_next(qlistentry);
			}while(strcmp(reg,qdict_get_str(qdict, "reg_name")) != 0 && qlistentry != NULL);
			if(strcmp(reg,qdict_get_str(qdict, "reg_name")) == 0) {
				int64_t error_type = qdict_get_int(qdict, "error_type");
				if(error_type == STUCK_AT_ZERO + REG_READ_ERROR * 10) {
					QList* extinfolist = qdict_get_qlist(qdict, "extra_info");
					const QListEntry* extinfoentry = qlist_first(extinfolist);
					do {
						int64_t bitno;
						if(extinfoentry == NULL)
							break;
						bitno = qint_get_int(qobject_to_qint(extinfoentry->value));
						#if defined(TARGET_ARM)
						env->regs[input_regs[i]] &= ~(1 << bitno);			
						#endif
						extinfoentry = qlist_next(extinfoentry);
					} while(extinfoentry == NULL);
				}
			}
		}
	}
	//
	// Memory Errors
	//
	if(error_info_memory != NULL)
	{

	}
	//
	// Execution Errors
	//
	// Should return the index number of the replaced instruction, in cases 
	// where instruction is replaced. Other wise should return -1
		
	// Errors due to ambient
	
	return -1;
}
