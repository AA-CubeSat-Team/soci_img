#Name: Hacky Dual Extrusion
#Info: Unretract at final location.
#Depend: GCode
#Type: postprocess

import re


with open(filename, "r") as f:
	lines = f.readlines()

delaystate = 0
with open(filename, "w") as f:
	for line in lines:
		if line[0]=='T' :
			delaystate=1;
			delayedtext="";
			f.write(line);
		else:
			if delaystate==1 :
				if line[:4]=="G1 X" :
					f.write(line);
					#f.write("here\n");
					f.write(delayedtext);
					delaystate=0;
					delayedtext="";
				else:
					#delayedtext+=";delayed\n";
					delayedtext+=line;
			else:
				f.write(line);
		
