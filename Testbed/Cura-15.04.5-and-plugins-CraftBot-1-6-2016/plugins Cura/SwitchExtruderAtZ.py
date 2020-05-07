#Name: Switch extruder at height
#Info: Switch the extruder at a certain heights
#Depend: GCode
#Type: postprocess
#Param: firstHeight(float:2) First change height (mm)
#Param: changeHeight(float:4) After that, change height at x (mm)
#Param: firstExtruder(int:0) First extruder number
#Param: secondExtruder(int:1) Second extruder number


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

"""
filename= "Cone.gcode"
firstHeight = 2
changeHeight = 4
firstExtruder = 0
secondExtruder = 1
"""

with open(filename, "r") as f:
	lines = f.readlines()

z = 0
changeState = 0
modExtruder = 0
currentExtruder = -1
currentSectionType = ""
myValue = None

"""
filename= "Cone.gcode.txt"
"""

with open(filename, "w") as f:
	for line in lines:
		if line.startswith(';'):
			if line.startswith(';TYPE:'):
				currentSectionType = line[6:].strip()
			f.write(line)
			continue
		myValue = getValue(line, 'G', None)
		if  myValue != None:
			newZ = getValue(line, 'Z', z)
			if newZ != z and currentSectionType != 'CUSTOM':
				z = newZ
				modExtruder = int((z-firstHeight) /  changeHeight) % 2		
				if currentExtruder == -1 :
					f.write(";TYPE:CUSTOM\n")
					f.write(";ModExtruder="+ str(modExtruder)+ " \n")
					f.write(";M117 Starting with Extruder #"  + str(firstExtruder) + "\n")
					f.write("T" + str(firstExtruder) + "\n")
					currentExtruder = firstExtruder
				elif modExtruder == 0 :		
						f.write(";TYPE:CUSTOM\n")
						f.write(";M117 Changing to Extruder #"  + str(firstExtruder) + "\n")
						f.write("T" + str(firstExtruder) + "\n")
						currentExtruder = firstExtruder
				else:
						f.write(";TYPE:CUSTOM\n")
						f.write(";M117 Changing to Extruder #"  + str(secondExtruder) + "\n")
						f.write("T" + str(secondExtruder) + "\n")
						currentExtruder = secondExtruder			
		f.write(line)
