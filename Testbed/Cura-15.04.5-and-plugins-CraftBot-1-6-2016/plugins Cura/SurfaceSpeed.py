#Name: Surface speed control
#Info: Print outer skin with different speed
#Depend: GCode
#Type: postprocess
#Param: skinSpeed(integer:100) Skin speed (%)

__copyright__ = "Copyright (C) 2014 Alexey Kurilov - Released under terms of the AGPLv3 License"
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

eOld = 0.
eCur = 0.
eTmp = 0.
speed = 0
currentSectionType = 'STARTOFFILE'
with open(filename, "w") as f:
	for line in lines:
		if line.startswith(';'):
			if line.startswith(';TYPE:'):
				currentSectionType = line[6:].strip()
			f.write(line)
			continue

		if getValue(line, 'G', None) == 1 or getValue(line, 'G', None) == 0:
			eTmp = getValue(line, 'E', 0)
			speed = int(getValue(line, 'F', 0))
			if speed > 0 and eTmp > 0 and currentSectionType == 'WALL-OUTER':
				eOld = eCur
				eCur = eTmp

				if eOld < eCur:
					fOld = 'F'+str(speed)
					speed = ( speed / 100 ) * int( skinSpeed )
					fNew = 'F'+str(int(speed))
					line = line.replace(fOld, fNew)
		f.write(line)
