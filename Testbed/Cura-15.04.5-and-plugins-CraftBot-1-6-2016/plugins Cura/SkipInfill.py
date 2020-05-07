#Name: Skip infill
#Info: Skip some infill layer for better speed and bridge
#Depend: GCode
#Type: postprocess
#Param: iStartFrom(integer:1) Start from layer, first layer is 1
#Param: iEndAt(integer:10000) End At layer
#Param: iMagnifier(integer:2) Infill only each this layer

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
		return float( m.group(0) )
	except:
		return default

with open(filename, "r") as f:
	lines = f.readlines()

ExtLast = 0.
ExtTemp = 0.
ExtNew = 0.
SectionType = 'STARTOFFILE'
lastjump = ''
Layer = 1
Retraction = 0
sExtOld = ''
sExtNew = ''
iMag = int( iMagnifier )
iMin = int( iStartFrom )
iMax = int( iEndAt )
bPrint = 0

with open( filename, "w" ) as f:
	for line in lines:
		bWrite = 1

		if line.startswith( ';' ) :
			if line.startswith( ';LAYER:' ) :
				Layer = int( line[7:].strip() ) + 1
				SectionType = 'NEWLAYER'

			if line.startswith( ';TYPE:' ) :
				SectionType = line[6:].strip()

			f.write( line )
			continue

		ExtTemp = getValue( line, 'E', -1 )
		if ExtTemp >= 0 :
			ExtRel = ExtTemp - ExtLast
			ExtLast = ExtLast + ExtRel
		else :
			ExtRel = 0

		sCode = getValue( line, 'G', None )

		if sCode == 92 :
			if ExtTemp >= 0 :
				ExtLast = ExtTemp
				ExtNew = ExtTemp

		if sCode == 1 or sCode == 0 :
			if ( SectionType == 'FILL' or SectionType == 'SUPPORT' ) and Layer >= iMin :
				if Layer >= iMin and Layer < iMax and ( Layer + 1 - iMin ) % iMag == 0 :
					bPrint = 1
					if ExtRel < 0 :
						Retraction = 1
						ExtNew = ExtNew + ExtRel
					elif ExtRel > 0 :
						if Retraction == 0 :
							ExtNew = ExtNew + ( ExtRel * iMag )
						else :
							Retraction = 0
							ExtNew = ExtNew + ExtRel
				elif Layer < iMax :
					bPrint = 0
					if ExtRel > 0 :
						bWrite = 0
					elif ExtRel < 0 :
						ExtNew = ExtNew + ExtRel
						bWrite = 1
						Retraction = 1
					else :
						bWrite = 0
						lastjump = line
				else :
					ExtNew = ExtNew + ExtRel

			elif ExtRel != 0 and bWrite == 1 and ExtTemp >= 0 :
				ExtNew = ExtNew + ExtRel
				if Retraction == 1 :
					Retraction = 0
					if bPrint == 0 :
						f.write( lastjump )

			if ExtRel != 0 and bWrite == 1 and ExtTemp >= 0 :
				if ExtTemp == int( ExtTemp ) :
					sExtOld = 'E'+str( int( ExtTemp ) )
				else :
					sExtOld = 'E'+str( ExtTemp )

				if ExtNew == int( ExtNew ) :
					sExtNew = 'E'+str( int( ExtNew ) )
				else :
					sExtNew = 'E'+str( ExtNew )

				line = line.replace( sExtOld, sExtNew )

		if bWrite == 1 :
			f.write( line )
