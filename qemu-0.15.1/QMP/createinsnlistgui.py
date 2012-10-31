import easygui as eg
import os
import sys

insns = {}
msg = 'Enter Name of the architecture:'
arch_name = eg.enterbox(msg)
if arch_name == None:
	sys.exit(0)
fh = open(arch_name + '_instruction_list.txt', 'a')
while 1:
	msg = 'Enter Instruction Info'
	fieldNames = ["Instruction Name(ADD_reg or ADD_imm)", "Instruction Name(ADD_reg or ADD_imm)", "Instruction Name(ADD_reg or ADD_imm)", "Instruction Name(ADD_reg or ADD_imm)", "Instruction Name(ADD_reg or ADD_imm)"]
	fieldValues = eg.multenterbox(msg, '', fieldNames)
	if fieldValues == None:
		break
	for i in range(len(fieldNames)):
		if fieldValues[i].strip() != None:
			fh.write(fieldValues[i].strip())
			fh.write('\n')
fh.close()
