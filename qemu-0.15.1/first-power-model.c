#include "qemu-variability.h"
#include "power-model.h"
#include <math.h>

static uint64_t current_frequency = 4; //DEFAULT_FREQUENCY; // 4000000
void freq_has_changed(uint64_t);
uint64_t serve_active_power(void *opaque);
void sleep_start_event(void);
void sleep_stop_event(uint64_t);
uint64_t serve_sleep_power(void*);
uint64_t serve_insn_power(uint32_t);
double sleep_power(void);
double active_power(void);
uint64_t active_energy(uint64_t cycles);
uint64_t sleep_energy(uint64_t time);
void print_model_parameters(void);
void change_model_parameter(const char * p, const char * v);

typedef struct {
	double T;
	double at1;
	double at2;
	double at3;
	double ak1;
	double ak2;
	double ak3;
	double aalpha;
	double abeta;
	double apt1;
	double apt2;
	double apt3;
	double sA;
	double sB;
	double sC;
	double act_leak_alpha;	
} tvlsi_pm; 

static union {
	tvlsi_pm parameters;
	double pm_parray[16];
} pm_parameters;


uint64_t serve_insn_power(uint32_t insn)
{
	return insn_map[insn].cycle_count;
}

void freq_has_changed(uint64_t new_freq)
{
	current_frequency = new_freq;
	printf("new frequency %lu\n", current_frequency);
}

double sleep_power(void) {
	tvlsi_pm *p = &pm_parameters.parameters;
	double t = p->T + 273;
	double power = p->sA*t*t/exp(p->sB/t) + p->sC;
	power *= 1.8;
	return power;
	/*
	 * 		t = temperature + 273;
		curr = self.A*t*t/np.exp(self.B/t) + self.C*self.ratio;
	*/
	
}

double active_power(void) {
	tvlsi_pm *p = &pm_parameters.parameters;
	double power = 0;
		
	if(p->T < p->at2) {
		power = p->apt1 + p->ak1*0.001 * pow(p->T - p->at1, p->aalpha);
	} else if (p->T < p->at3) {
		power = p->apt2 + p->ak2*0.001 * (p->T - p->at2);
	} else {
		power = p->apt3 + p->ak3*0.001 * pow(p->T - p->at3, p->abeta);
	}
	return power*1000000 + sleep_power()*p->act_leak_alpha;
	/*
			t = temperature
		if (t < self.ActModel_t2) :
			power = self.ActModel_pt1 + self.ActModel_k1*pow((t-self.ActModel_t1),self.ActModel_alpha)
		elif (t < self.ActModel_t3) :
			power = self.ActModel_pt2 + self.ActModel_k2*(t-self.ActModel_t2)
		else :
			power = self.ActModel_pt3 + self.ActModel_k3*pow((t-self.ActModel_t3),self.ActModel_beta)
		return power*1000000 + self.getSleepPower(temperature)*self.ActLeakAlpha
		#return self.getActiveCurrent(temperature)*1.8;
		* 
		*
		* */
}

uint64_t active_energy(uint64_t cycles) {
	double elapsed_time = cycles/((double)current_frequency*1000000);
	//printf("time: %f, energy: %f\n", elapsed_time, active_power()*elapsed_time);
	return active_power()*elapsed_time;
}

uint64_t sleep_energy(uint64_t time) {
	double elapsed_time = time/1000000000.0;
	//printf("time: %f, energy: %f\n", elapsed_time, sleep_power()*elapsed_time);
	// Too small for small intervals, divide later
	return (sleep_power() * elapsed_time)*1000;
}


uint64_t serve_active_power(void *opaque)
{
	int  i;
	struct cycle_counter cycle_count;
	struct cycle_counter cycle_count_chkpt;
	struct energy_counter *s = (struct energy_counter*) opaque;
	read_all_cycle_counters(&cycle_count);
	read_all_cycle_counter_chkpts(&cycle_count_chkpt);
	
	for(i = 0;i < MAX_INSN_CLASSES; ++i)
		s->insn_energy[i] = active_energy(cycle_count.cycle_count[i] - cycle_count_chkpt.cycle_count[i]);
	return 0;
}

void sleep_start_event(void)
{
}

void sleep_stop_event(uint64_t interval)
{
	update_sleep_energy(sleep_energy(interval));
}

uint64_t serve_sleep_power(void* s)
{
	struct energy_counter* temp = (struct energy_counter*) s;
	temp->sleep_energy = read_sleep_energy();	
	return 0;
}

void change_model_parameter(const char * p, const char * v) {
	


	int pp = strtol(p,NULL, 10);
	double vv = strtod((char *)v, NULL);
	
	printf("Change model parameter %dto %f \n",  pp, vv);

	
	if (pp > 15) {
		printf("No parameter # %d\n", pp);
	} else {
		pm_parameters.pm_parray[pp] = 	vv;
	}

	
	print_model_parameters();


}

static PowerModel first_pwr_model = 
{
	.freq_change_notify = freq_has_changed,
	.read_active_power = serve_active_power,
	.sleep_start_notify = sleep_start_event,
	.sleep_stop_notify = sleep_stop_event,
	.read_sleep_power = serve_sleep_power,
	.insn_power = serve_insn_power,
	.change_parameter = change_model_parameter
};




void print_model_parameters(void) {
	printf("Power model parameters: T=%f, at1=%f, at2=%f, at3=%f, ak1=%f, ak2=%f, ak3=%f, aalpha=%f, abeta=%f, apt1=%f, apt2=%f, apt3=%f, sA=%f, sB=%f, sC=%f, act_leak_alpha=%f\n", pm_parameters.parameters.T, pm_parameters.parameters.at1, pm_parameters.parameters.at2, pm_parameters.parameters.at3, pm_parameters.parameters.ak1, pm_parameters.parameters.ak2, pm_parameters.parameters.ak3, pm_parameters.parameters.aalpha, pm_parameters.parameters.abeta, pm_parameters.parameters.apt1, pm_parameters.parameters.apt2, pm_parameters.parameters.apt3, pm_parameters.parameters.sA, pm_parameters.parameters.sB, pm_parameters.parameters.sC, pm_parameters.parameters.act_leak_alpha);
	printf("At t=%f, Active Power = %f uW, Sleep Power = %f uW\n", pm_parameters.parameters.T, active_power(), sleep_power());
}

void power_init(void) 
{
	QDict *class;
	const QDictEntry* classentry;
	
	
	class = get_class_info_ptr();
	
	memset ( &pm_parameters, 0, sizeof(tvlsi_pm) );
	
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
				pm_parameters.pm_parray[idx] = 	atof(entry);
				idx++;
				//const char* insn = qstring_get_str(qobject_to_qstring(list_entry->value));
				list_entry = qlist_next(list_entry);
			}
			
			print_model_parameters();
		} else {
			printf("Could not find parameters for Power Model\n");
		}
		classentry = qdict_next(class_info, classentry);
		
	} while(classentry != NULL);
	
	//printf("frequency= %lu\n", current_frequency);
	
	power_model_init(&first_pwr_model);	
	
}
