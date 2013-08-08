import random
import math
import numpy as np
import matplotlib.pyplot as plt

def gen_instance() :

	T = 21
	Vdd = 1.0

	x1 = random.normalvariate(0,0.6804)
	x2 = random.normalvariate(0,0.3666)
	x3 = random.normalvariate(0,0.0188)
	x4 = random.normalvariate(0,0.4062)

	dynamic_power_multiplier = 0.0001
	Nd = dynamic_power_multiplier * (1 + 0.01*(0.7273*x1-0.6864*x2))
	
	Bd = 2.088 + 0.6864*x1 + 0.7273*x2
	Cd = 0.6386 + x3
	leakage_power_multiplier = 0.02
	Nl = leakage_power_multiplier * math.exp(x4)
	
	Al = -9210
	Bl = 890
	Vnl = 0.2
	Vpl = 0.232

	delta_vtp = 0.0
	mode = 1.0
	Vdd_scalar = 1.0
	
	return [T, Vdd, Nd, Bd, Cd, Nl, Al, Bl, Vnl, Vpl, delta_vtp, mode, Vdd_scalar]
	
def slp_power(t, instance):
	t = t+273
	v = instance[1]
	Nd = instance[2]
	Bd = instance[3]
	Cd = instance[4]
	Nl = instance[5]
	Al = instance[6]
	Bl = instance[7]
	Vnl = instance[8]
	Vpl = instance[9]
	delta_vtp = instance[10]
	power = Nl * v * t*t * ( math.exp(Al*Vnl/t) + math.exp(Al*(Vpl+delta_vtp)/t) ) * math.exp(Bl*v/t);
	
	return power/1000000;
	
def act_power(t, instance):
	v = instance[1]
	Bd = instance[3]
	Cd = instance[4]
	delta_vtp = instance[10]
	Nd = instance[2]
	frequency = 8
	
	power = v*v + Bd * math.pow(( v - Cd - delta_vtp ),3);
	power = power * Nd * frequency;
	return power + slp_power(t, instance)
	
def power_trace(T, instance, fun) :
	pp = []
	for t in T:	
		pp.append(fun(t, instance))
	return pp
	
def print_instance(i) :
	msg = "{\"0\": {\"class_name\": \"all\", \"power_param\" : [  "
	for x in i:
		msg += "\b\"%f\", " % x	
	msg = msg[0:len(msg)-2]+"], \"idx\": 0}}\n{\"all\": []}"
	print msg
	
instances = []	
sp = []

ni = 10000

for i in range(1,ni) :
	instance = gen_instance()
	T = range(0,70)
	instances.append(instance)
	sp.append(slp_power(21,instance))


p, i = zip(*sorted(zip(sp, instances)))

print p[0], p[len(p)/2], p[len(p)-1]

selectedi = i[0], i[len(i)/2], i[len(i)-1]
selectedi = i[100], i[1500], i[3000], i[4500], i[6000], i[7500], i[9000], i[9970]
#selectedi = i

for instance in selectedi :
	print_instance(instance)
	
	T = range(0,100)
	sp = power_trace(T, instance, slp_power)
	ap = power_trace(T, instance, act_power)
	plt.plot(T, sp)
	plt.hold(True)


plt.show()
