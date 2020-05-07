#Name: Filament change 
#Info: Causes the printer to eject filament at a certain height (using M600)
#Depend: GCode
#Type: postprocess
#Param: pauseLevel(float:10.0) height (mm)
#Param: parkX(float:1) Head park X (mm)
#Param: parkY(float:1) Head park Y (mm)
#Param: parkZ(float:10.0) Additional safety height(mm)
#Param: retractIntitial(float:5) Initial retract amount (mm)
#Param: retractFinal(float:200) Final retract amount (mm)

import re

def getValue(line, key, default = None):
	if not key in line or (';' in line and line.find(key) > line.find(';')):
		return default
	subPart = line[line.find(key) + 1:]
	m = re.search('^[0-9]+\.?[0-9]*', subPart)
	if m == None:
		return default
	try:
		return float(m.group(0))
	except:
		return default

with open(filename, "r") as f:
	lines = f.readlines()

z = 0
x = 0
y = 0
pauseState = 0
with open(filename, "w") as f:
	for line in lines:
		if getValue(line, 'G', None) == 1:
			newZ = getValue(line, 'Z', z)
			x = getValue(line, 'X', x)
			y = getValue(line, 'Y', y)
			if newZ != z:
				z = newZ
				if z < pauseLevel and pauseState == 0:
					pauseState = 1
				if z >= pauseLevel and pauseState == 1:
					pauseState = 2
					#Retract
					f.write("M600 X%f Y%f Z%f E-%f L-%f \n" % (parkX, parkY,parkZ, retractIntitial, retractFinal))
					
		f.write(line)
