#Name: Change bed temp at height
#Info: Change the bed temperature at a certain height
#Depend: GCode
#Type: postprocess
#Param: changeHeight(float:0.6) Change height
#Param: newTemp(float:50) New bed temperature

import re

def getValue(line, key, default = None):
	if not key in line or (';' in line and line.find(key) > line.find(';')):
		return default
	subPart = line[line.find(key) + 1:]
	m = re.search('^[0-9]+\.?[0-9]*', subPart)
	if m is None:
		return default
	try:
		return float(m.group(0))
	except:
		return default

with open(filename, "r") as f:
	lines = f.readlines()

z = 0
changeState = 0
with open(filename, "w") as f:
	for line in lines:
		if line.startswith(';'):
			if line.startswith(';TYPE:'):
				currentSectionType = line[6:].strip()
			f.write(line)
			continue
		if getValue(line, 'G', None) == 1:
			newZ = getValue(line, 'Z', z)
			if newZ != z and currentSectionType != 'CUSTOM':
				z = newZ
				if z < changeHeight and changeState == 0:
					changeState = 1
				if z >= changeHeight and changeState == 1:
					changeState = 2
					f.write(";TYPE:CUSTOM\n")
					f.write(";Changing bed temperature to "  + str(newTemp) + " degrees\n")
					f.write("M140 S" + str(newTemp) + "\n")
		f.write(line)
