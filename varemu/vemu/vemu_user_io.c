//
//  vemu_user_io.c
//  
//
//  Created by Lucas Wanner on 11/15/12.
//
//

#include <stdio.h>
#include <string.h>
#include "vemu.h"
#include "vemu-arm.h" 
#include "vemu-cycles.h" 
#include "vemu-energy.h" 
#include "qapi/qmp/qjson.h"
#include "qapi/qmp/qint.h"
#include "qapi/qmp/qdict.h"

#ifdef 	VEMU


void vemu_parse_input_file(const char *fname);
void vemu_parse_pm_parameters(QDict *class, const char* class_idx);
void vemu_parse_instr_class(QDict* class_info, const char* class_idx, const char* insn);

void vemu_parse_pm_parameters(QDict *class, const char* class_idx) 
{
	const QDictEntry* classentry;

	//memset ( &pm_parameters, 0, sizeof(tvlsi_pm) );
	
	uint8_t class_idx_int = atoi(class_idx);
	
	
	classentry = qdict_first(class);
	do
	{
		QObject* temp = qdict_entry_value(classentry);		
		QDict* pdict = qobject_to_qdict(temp);
		QObject* params = qdict_get(pdict,"power_param");
		if (params != NULL) {
			const QListEntry *list_entry;
			QList* plist = qobject_to_qlist(params);
			list_entry = qlist_first(plist);
			int idx = 0;
			while(list_entry != NULL)
			{
				const char* entry = qstring_get_str(qobject_to_qstring(list_entry->value));
				//printf("class %d, idx %d, value %f\n", class_idx_int, idx, atof(entry));
				vemu_pm_change_parameter(class_idx_int, idx, atof(entry));				
				idx++;
				list_entry = qlist_next(list_entry);
			}
			
			//print_model_parameters();
		} else {
			printf("Could not find parameters for Power Model\n");
		}
		classentry = qdict_next(class, classentry);
		
	} while(classentry != NULL);
	
	vemu_pm_print_parameters();	
	
	
}

void vemu_parse_instr_class(QDict* class_info, const char* class_name, const char* instr)
{

	const QDictEntry* pentry;
	int idx= -1;
	assert(class_info != NULL);
	
	

	pentry = qdict_first(class_info);
	do
	{
		QObject* temp = qdict_entry_value(pentry);
		QDict* pdict = qobject_to_qdict(temp);
		idx = qdict_get_int(pdict, "idx");
		const char * str = qdict_get_str(pdict, "class_name");
		if(strcmp(str, class_name) == 0)
			break;
		idx = -1;
		pentry = qdict_next(class_info, pentry);
				
	} while(pentry != NULL);
	
	if (idx != -1) {
        int thumb;
        for (thumb = 0; thumb <= 1; thumb++) {
            vemu_instr_info* all_instr = vemu_target_get_all_instr_info(thumb);
            int max_instr = vemu_target_get_all_instr_size(thumb);
            int i;
            for(i = 0; i < max_instr; i++) {
                if(strcmp(all_instr[i].name,instr) == 0) {
                    all_instr[i].class = idx;
                    //vemu_debug("Setting instruction %s (%x) class to %s (%d)\n", instr, i, class_name, idx);
                }
            }
        }
	} else {
		vemu_debug("Could not find an index for class %s\n", class_name);
	}
		
}


void vemu_parse_input_file(const char *fname)
{
	FILE *fp;
	char line[2000];
	QObject *qobj;
	QDict *qdict;
	fp = fopen(fname, "r");
	if(fp == NULL)
	{
		perror("Could not open variability file");
		exit(0);
	}
	
	fgets(line, 2000,fp);
	//printf("%s\n", line);
	
	
	qobj = qobject_from_json(line);
    
	
	assert(qobject_type(qobj) == QTYPE_QDICT);
	qdict = qobject_to_qdict(qobj);
	//printf("Size of the dict %u\n", qdict_size(qdict));
	assert(qdict_size(qdict) > 0);
    
    QDict* class_info = qdict;
    
	//class_info_init(qdict);
	while(!feof(fp))
	{
		const QDictEntry *entry;
		int i;
		fgets(line, 2000, fp);
		for(i = 0;i < 2000; ++i)
			if(line[i] == '\n') {
				//printf("%s \n", line);
				break;
			}
		qobj = qobject_from_json(line);
        
		if(qobj == NULL) {
			//printf("QOBJ is null\n");
			continue;
		}
		qdict = qobject_to_qdict(qobj);
		if(qdict_haskey(qdict, "y")){
			// Code to add instructions as errorneous or not
			entry = qdict_first(qdict);
			do
			{
				const QListEntry *list_entry;
				QList* insn_list = qobject_to_qlist(qdict_entry_value(entry));
				list_entry = qlist_first(insn_list);
				while(list_entry != NULL)
				{
					//const char* insn = qstring_get_str(qobject_to_qstring(list_entry->value));
					//update_insn_error_info(qdict_entry_key(entry), insn);
					list_entry = qlist_next(list_entry);
				}
				entry = qdict_next(qdict, entry);
			} while(entry != NULL);
		} else if(qdict_haskey(qdict, "start_pc") && qdict_haskey(qdict, "end_pc")) {
            //error_init_pc(qdict_get_int(qdict, "start_pc"),
            //              qdict_get_int(qdict, "end_pc"));
        } else if(qdict_haskey(qdict, "start_icount") && qdict_haskey(qdict, "end_icount")) {
            //error_init_icount(qdict_get_int(qdict, "start_icount"),
            //                  qdict_get_int(qdict, "end_icount"));
        } else if(qdict_haskey(qdict, "error_regs")) {
            entry = qdict_first(qdict);
            // error_regs_init(qobject_to_qlist(qdict_entry_value(entry)));
        } else if(qdict_haskey(qdict, "mem_errors")) {
            entry = qdict_first(qdict);
            // mem_error_init(qobject_to_qlist(qdict_entry_value(entry)));
        } else {

			
            // Code to assign instruction to a particular class
            entry = qdict_first(qdict);
            
			vemu_parse_pm_parameters(class_info, qdict_entry_key(entry));
            
            
            do
            {
                const QListEntry * list_entry;
                QList* insn_list = qobject_to_qlist(qdict_entry_value(entry));
                list_entry = qlist_first(insn_list);
                while(list_entry != NULL)
                {
                    const char* insn = qstring_get_str(qobject_to_qstring(list_entry->value));
                    vemu_parse_instr_class(class_info, qdict_entry_key(entry), insn);
                    list_entry = qlist_next(list_entry);
                }
                entry = qdict_next(qdict, entry);	
            } while(entry != NULL);
        }
	}
	fclose(fp);		
}

int vemu_do_change_model_param(Monitor *mon, const QDict *qdict, QObject **ret_data)
{

	const char *c = qdict_get_str(qdict, "c");
	const char *p = qdict_get_str(qdict, "p");
	const char *v = qdict_get_str(qdict, "v");
	
	char cc[4096];
	memcpy(cc, c+1, strlen(c)-2);
	cc[strlen(c)-2]  = '\0';
	
	char pp[4096];
	memcpy(pp, p+1, strlen(p)-2);
	pp[strlen(p)-2]  = '\0';
	
	char vv[4096];
	memcpy(vv, v+1, strlen(v)-2);
	vv[strlen(v)-2]  = '\0';	
	
	printf("before conversion: %s %s %s\n", cc, pp, vv);


	uint8_t ccc = strtol((const char *)cc,NULL, 10);		
	uint8_t ppp = strtol(pp,NULL, 10);
	double vvv = strtod((char *)vv, NULL);
	
	printf("after conversion: %d %d %f\n", ccc, ppp, vvv);
	
	
	vemu_energy_change_parameter(ccc,ppp,vvv);	
	
	return 0;
}

int vemu_do_info_time(Monitor *mon, const QDict *qdict, QObject **ret_data)
{
	QDict *qdict1;
	QObject *obj;
	
	uint64_t act_time = vemu_get_act_time_all_classes();
	uint64_t slp_time = vemu_get_slp_time();
	
	qdict1 = qdict_new();
	obj = qobject_from_jsonf(" { 'TIME_TYPE' : 'ACT' , "
								"'value': %" PRId64 " }" , act_time);
	qdict_put_obj(qdict1, "1", obj);
	obj = qobject_from_jsonf(" { 'TIME_TYPE' : 'SLP' , "
								"'value': %" PRId64 " }" , slp_time);
	qdict_put_obj(qdict1, "2", obj);
	*ret_data = QOBJECT(qdict1);	
	
	return 0;}

int vemu_do_info_energy(Monitor *mon, const QDict *qdict, QObject **ret_data)
{
	QDict *qdict1;
	QObject *obj;
	
	uint64_t act_energy = vemu_get_act_energy_all_classes();
	uint64_t slp_energy = vemu_get_slp_energy();
	
	qdict1 = qdict_new();
	obj = qobject_from_jsonf(" { 'ENERGY_TYPE' : 'ACT' , "
								"'value': %" PRId64 " }" , act_energy);
	qdict_put_obj(qdict1, "1", obj);
	obj = qobject_from_jsonf(" { 'ENERGY_TYPE' : 'SLP' , "
								"'value': %" PRId64 " }" , slp_energy);
	qdict_put_obj(qdict1, "2", obj);
	*ret_data = QOBJECT(qdict1);	
	
	return 0;
	
}

int vemu_do_info_all(Monitor *mon, const QDict *qdict, QObject **ret_data)
{
	QDict *qdict1;
	QObject *obj;
	
	uint64_t act_energy = vemu_get_act_energy_all_classes();
	uint64_t slp_energy = vemu_get_slp_energy();
	
	qdict1 = qdict_new();
	obj = qobject_from_jsonf(" { 'ENERGY_TYPE' : 'ACT' , "
								"'value': %" PRId64 " }" , act_energy);
	qdict_put_obj(qdict1, "1", obj);
	obj = qobject_from_jsonf(" { 'ENERGY_TYPE' : 'SLP' , "
								"'value': %" PRId64 " }" , slp_energy);
	qdict_put_obj(qdict1, "2", obj);
	
	
	uint64_t act_time = vemu_get_act_time_all_classes();
	uint64_t slp_time = vemu_get_slp_time();
	
	obj = qobject_from_jsonf(" { 'TIME_TYPE' : 'ACT' , "
								"'value': %" PRId64 " }" , act_time);
	qdict_put_obj(qdict1, "3", obj);
	obj = qobject_from_jsonf(" { 'TIME_TYPE' : 'SLP' , "
								"'value': %" PRId64 " }" , slp_time);
	qdict_put_obj(qdict1, "4", obj);
	
	*ret_data = QOBJECT(qdict1);	
	
	return 0;
	
}


void vemu_init(const char *fname) {
    vemu_debug("filename: %s\n", fname);
    vemu_parse_input_file(fname);
    vemu_init_cycles();
}

#endif
