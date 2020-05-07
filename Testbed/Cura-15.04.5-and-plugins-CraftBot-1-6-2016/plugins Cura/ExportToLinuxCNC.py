#Name: Export to LinuxCNC
#Info: Modify GCode so it is ready to export to Linux CNC based 3D printer
#Depend: GCode
#Type: postprocess
#Last modified Feburary 23, 2014
import re

with open(filename, "r") as f:
	lines = f.readlines()

absolute_pos = True

with open(filename, "w") as f:
	for line in lines:	
		line2 = list(line.partition(";"))
		
	#change axis E to axis A
	
		if "G90 " in line2[0]:
			line = line.replace('E', 'A')	
		if "G91 " in line2[0]:
			line = line.replace('E', 'A')	
		if "G92 " in line2[0]:
			line = line.replace('E', 'A')				
		if "G1 " in line2[0]:
			line = line.replace('E', 'A')
		if "M84" in line2[0]:
			line = ""				
		if "M107" in line2[0]:
			line = ""	
		if "M109" in line2[0]:
			line = ""			
		if "T0" in line2[0]:
			line = ""			

		#line = line.replace('M107', '')	
		#line = line.replace('M84', '')	

		f.write(line)