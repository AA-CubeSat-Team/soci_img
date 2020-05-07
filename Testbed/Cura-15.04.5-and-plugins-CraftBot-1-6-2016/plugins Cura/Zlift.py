#Name: Z lift by David Bue Pedersen
#Info: Lift the nozzle on fast moves
#Depend: GCode
#Type: postprocess
#Param: zlift(float:1) Z lift (mm)
#Param: zliftspeed(float:200) Z lift feedrate (mm/s)
#Param: minhoplength(float:1) Min length (XY) before lifting (mm)

import re
import math

from itertools import tee, izip
def pairwise(iterable):
    "s -> (s0,s1), (s1,s2), (s2, s3), ..."
    a, b = tee(iterable)
    next(b, None)
    return izip(a, b)
    

def getValue(line, key, default = None):
	if not key in line or (';' in line and line.find(key) > line.find(';')):
		return default
	subPart = line[line.find(key) + 1:]
	#m = re.search('^[0-9]+\.?[0-9]*', subPart)
	m = re.search('[-+]?\d*\.\d+|\d+', subPart)
	
	if m == None:
		return default
	try:
		return float(m.group(0))
	except:
		return default
		

with open(filename, "r") as f:
	lines = f.readlines()

absolute_pos = True
#E = 0
#Enew = 0
Z = 0
X = 0
Y = 0
Xold = 0
Yold = 0
DeltaL = 0
starthopping = False

with open(filename, "w") as f:
    for line, next_line in pairwise(lines):

	
		if "G90" in line:
			absolute_pos = True
		if "G91" in line:
			absolute_pos = False
		if "LAYER" in line:  #New layer started. Begin hopping
			starthopping = True	
		if "TYPE:WALL-INNER" in line:  #New wall started. Begin hopping
			starthopping = True	
		if "TYPE:WALL-OUTER" in line:  #New wall started. Begin hopping
			starthopping = True			
		if "TYPE:FILL" in line:  #Infill of an island started. Don't hop
			starthopping = False	
		if "LAYER" in next_line:  #First line after infill
			starthopping = True
		if "TYPE:WALL-OUTER" in next_line:  #First line after infill
			starthopping = True
		if "TYPE:WALL-INNER" in next_line: #First line after infill
			starthopping = True
												
		if "G1 " in line and absolute_pos:
			Xold = X
			Yold = Y
			X = getValue(line, 'X', X)		
			Y = getValue(line, 'Y', Y)	
			Z = getValue(line, 'Z', Z)
							
		if "G0 " in line and absolute_pos:
			Z = getValue(line, 'Z', Z)
			Xold = X
			Yold = Y			
			X = getValue(line, 'X', X)		
			Y = getValue(line, 'Y', Y)	
			if starthopping:			
				DeltaL = math.sqrt( (X-Xold)*(X-Xold) + (Y-Yold)*(Y-Yold) )   # x**2 means x^2
			
				if DeltaL > minhoplength: 			
					f.write("G1 Z%f F%f;Hopping up DeltaL%f\n" % (Z+zlift,zliftspeed*60,DeltaL))
					f.write(line)
					f.write("G1 Z%f F%f;Hopping down\n" % (Z,zliftspeed*60))
				else:
					f.write(line)
			else:
				f.write(line)				
		else:
			f.write(line)
			
			