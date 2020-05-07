#Name: Wood
#Info: Vary the print temperature troughout the print to create wood rings with some printing material such as the LayWoo. The higher the temperature, the darker the print.
#Depend: GCode
#Type: postprocess
#Param: minTemp(float:180) Mininmum print temperature (degree C)
#Param: maxTemp(float:230) Maximun print temperature (degree C)
#Param: grainSize(float:3.0) Average "wood grain" size (mm)
#Param: firstTemp(float:0) Starting temperature (degree C, zero to disable)
#Param: spikinessPower(float:1.0) Relative thickness of light bands (power, >1 to make dark bands sparser)
#Param: maxUpward(float:0) Instant temperature increase limit, as required by some firmwares (C)
#Param: zOffset(float:0) Vertical shift of the variations, as shown at the end of the gcode file (mm)

import re
import random
import math
import time
import datetime

__author__ = 'Jeremie Francois (jeremie.francois@gmail.com)'
__date__ = '$Date: 2014/01/10 13:31:47 $'
__license__ = 'GNU Affero General Public License http://www.gnu.org/licenses/agpl.html'

############ BEGIN CURA PLUGIN STAND-ALONIFICATION ############
# This part is an "adapter" to Daid's version of my original Cura/Skeinforge plugin that
# he upgraded to the lastest & simpler Cura plugin system. It enables commmand-line
# postprocessing of a gcode file, so as to insert the temperature commands at each layer.
#
# Note that it should still be viewed by Cura as a regular plugin by the way!
#
# To run it you need Python, then simply run it like
#   wood_standalone.py --min minTemp --max maxTemp --grain grainSize --file gcodeFile
# It will "patch" your gcode file with the appropriate M104 temperature change.
#
import inspect
import sys
import getopt

def plugin_standalone_usage(myName):
	print("Usage:")
	print("  "+myName+" --file gcodeFile (--min minTemp) (--max maxTemp) (--first-temp startTemp) (--grain grainSize) (--max-upward deltaTemp) (--random-seed integer) (--spikiness-power exponentFactor) (--z-offset zOffset)")
	print("  "+myName+" -f gcodeFile (-i minTemp) (-a maxTemp) (-t startTemp) (-g grainSize) (-u deltaTemp) (-r randomSeed) (-s spikinessFactor) (-z zOffset)")
	print("Licensed under CC-BY-NC "+__date__[7:26]+" by Jeremie.Francois@gmail.com (www.tridimake.com)")
	sys.exit()

try:
	filename
except NameError:
	# Then we are called from the command line (not from cura)
	# trying len(inspect.stack()) > 2 would be less secure btw
	opts, extraparams = getopt.getopt(sys.argv[1:],'i:a:t:g:u:r:s:z:f:h',['min=','max=','first-temp=','grain=','max-upward=','random-seed=','spikiness-power=','z-offset=','file=','help']) 
	minTemp=190
	maxTemp=240
	firstTemp=0
	grainSize=3
	maxUpward=0
	zOffset=0
	spikinessPower=1.0
	filename=""
	for o,p in opts:
		if o in ['-f','--file']:
			filename = p
		elif o in ['-i','--min']:
			minTemp = float(p)
		elif o in ['-a','--max']:
			maxTemp = float(p)
		elif o in ['-t','--first-temp']:
			firstTemp = float(p)
		elif o in ['-g','--grain']:
			grainSize = float(p)
		elif o in ['-u','--max-upward']:
			maxUpward = float(p)
		elif o in ['-z','--z-offset']:
			random.seed(0)
			zOffset = float(p)
		elif o in ['-r','--random-seed']:
			if p!= 0:
				random.seed(p)
		elif o in ['-s','--spikiness-power']:
			spikinessPower = float(p)
			if spikinessPower<=0:
				spikinessPower=1.0
	if not filename:
		plugin_standalone_usage(inspect.stack()[0][1])
#
############ END CURA PLUGIN STAND-ALONIFICATION ############


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

def getZ(line, default = None):
	# new 20130727: now support G0 in addition to G1 
	if getValue(line, 'G') == 0 or getValue(line, 'G') == 1:
		return getValue(line, 'Z', default)
	else:
		return default


class Perlin:
	# Perlin noise: http://mrl.nyu.edu/~perlin/noise/

	def __init__(self, tiledim=256):
		self.tiledim= tiledim
		self.perm = [None]*2*tiledim

		permutation = []
		for value in xrange(tiledim): permutation.append(value)
		random.shuffle(permutation)

		for i in xrange(tiledim):
			self.perm[i] = permutation[i]
			self.perm[tiledim+i] = self.perm[i]

	def fade(self, t):
		return t * t * t * (t * (t * 6 - 15) + 10)

	def lerp(self, t, a, b):
		return a + t * (b - a)

	def grad(self, hash, x, y, z):
		#CONVERT LO 4 BITS OF HASH CODE INTO 12 GRADIENT DIRECTIONS.
		h = hash & 15
		if h < 8: u = x
		else:     u = y
		if h < 4: v = y
		else:
			if h == 12 or h == 14: v = x
			else:                  v = z
		if h&1 == 0: first = u
		else:        first = -u
		if h&2 == 0: second = v
		else:        second = -v
		return first + second

	def noise(self, x,y,z):
		#FIND UNIT CUBE THAT CONTAINS POINT.
		X = int(x)&(self.tiledim-1)
		Y = int(y)&(self.tiledim-1)
		Z = int(z)&(self.tiledim-1)
		#FIND RELATIVE X,Y,Z OF POINT IN CUBE.
		x -= int(x)
		y -= int(y)
		z -= int(z)
		#COMPUTE FADE CURVES FOR EACH OF X,Y,Z.
		u = self.fade(x)
		v = self.fade(y)
		w = self.fade(z)
		#HASH COORDINATES OF THE 8 CUBE CORNERS
		A = self.perm[X  ]+Y; AA = self.perm[A]+Z; AB = self.perm[A+1]+Z
		B = self.perm[X+1]+Y; BA = self.perm[B]+Z; BB = self.perm[B+1]+Z
		#AND ADD BLENDED RESULTS FROM 8 CORNERS OF CUBE
		return self.lerp(w,self.lerp(v,
				self.lerp(u,self.grad(self.perm[AA  ],x  ,y  ,z  ), self.grad(self.perm[BA  ],x-1,y  ,z  )),
				self.lerp(u,self.grad(self.perm[AB  ],x  ,y-1,z  ), self.grad(self.perm[BB  ],x-1,y-1,z  ))),
			self.lerp(v,
				self.lerp(u,self.grad(self.perm[AA+1],x  ,y  ,z-1), self.grad(self.perm[BA+1],x-1,y  ,z-1)),
				self.lerp(u,self.grad(self.perm[AB+1],x  ,y-1,z-1), self.grad(self.perm[BB+1],x-1,y-1,z-1))))

	def fractal(self, octaves, persistence, x,y,z, frequency=1):
		value = 0.0
		amplitude = 1.0
		totalAmplitude= 0.0
		for octave in xrange(octaves):
			n= self.noise(x*frequency,y*frequency,z*frequency)
			value += amplitude * n
			totalAmplitude += amplitude
			amplitude *= persistence
			frequency *= 2
		return value / totalAmplitude


with open(filename, "r") as f:
	lines = f.readlines()


# new 20130727: limit the number of changes for helicoidal/Joris slicing method
minimumChangeZ=0.1

#Find the total height of the object
maxZ = None
thisZ = 0
eol = "#"
for line in lines:
	thisZ = getZ(line)
	if thisZ != None and maxZ < thisZ:
		maxZ = thisZ
	if eol == "#" and len(line)>=2: # new 20130727:btw detect existing EOL to stay consistent when we'll be adding our own lines
		if(line[-2]=="\r"): # windows...
			eol = "\r\n"
if eol == "#":
	eol="\n" # uh oh empty file?


"First pass generates the noise curve, that we normalize afterwards because the user expects to reach the min & max temperatures"
perlin = Perlin()

def perlinToNormalizedWood(z):
	banding = 3
	octaves = 2
	persistence = 0.7
	noise = banding * perlin.fractal(octaves, persistence, 0,0, (z+zOffset)/(grainSize*2));
	noise = (noise - math.floor(noise)) # normalized to [0,1]
	noise= math.pow(noise, spikinessPower)
	return noise

# Generate normalized noises, and then temperatures (will be indexed by Z value)
noises = {}
# first value is hard encoded since some slicers do not write a Z0 at the first layer!
# TODO: keep only Z changes that are followed by real extrusion (ie. discard non-printing head movements!)
noises[0] = perlinToNormalizedWood(0)
pendingNoise = None
formerZ = -1
for line in lines:
	thisZ = getZ(line, formerZ)
	if thisZ > 2 + formerZ:
		formerZ = thisZ
		#noises = {} # some damn slicers include a big negative Z shift at the beginning, which impacts the min/max range
	elif abs ( thisZ - formerZ ) > minimumChangeZ:
		formerZ = thisZ
		noises[thisZ] = perlinToNormalizedWood(thisZ);
lastPatchZ = thisZ; # record when to stop patching M104, to leave the last one switch the temperature off

# normalize built noises
noisesMax = noises[max(noises, key = noises.get )]
noisesMin = noises[min(noises, key = noises.get )]
for z,v in noises.items():
	noises[z]= (noises[z]-noisesMin)/(noisesMax-noisesMin)


def noiseToTemp(noise):
	return minTemp + noise * (maxTemp - minTemp)

#
# Now save the file with the patched M104 temperature settings
#
fout= open(filename, "w")
with fout as f:
	# Prepare a transposed temperature graph for the end of the file

	#
	# new 20130727: header and first (blocking) temperature change
	#
	f.write(";woodified gcode, see graph at the end - jeremie.francois@gmail.com - generated on " + datetime.datetime.now().strftime("%Y%m%d-%H%M") + eol)
	warmingTempCommands="M230 S0" + eol # enable wait for temp on the first change
	if firstTemp == 0:
		warmingTempCommands+= ("M104 S%i" + eol) % noiseToTemp(0)
	else:
		warmingTempCommands+= ("M104 S%i" + eol) % firstTemp
	# The two following commands depends on the firmware:
	warmingTempCommands+= "M230 S1" + eol # now disable wait for temp on the first change
	warmingTempCommands+= "M116" + eol # wait for the temperature to reach the setting (M109 is obsolete)
	f.write(warmingTempCommands);

	graphStr=";WoodGraph: Wood temperature graph (from "+str(minTemp)+"C to "+str(maxTemp)+"C, grain size "+str(grainSize)+"mm, z-offset "+str(zOffset)+")"
	if maxUpward:
		graphStr+=", temperature increases capped at "+str(maxUpward)
	graphStr+=":"
	graphStr+=eol
		
	thisZ = -1
	formerZ = -1
	warned = 0
	
	postponedTempDelta=0 # only when maxUpward is used
	postponedTempLast=None # only when maxUpward is used
	skiplines=0
	for line in lines:
		if "; set extruder " in line.lower(): # special fix for BFB
			f.write(line);
			f.write(warmingTempCommands);
			warmingTempCommands=""
		elif "; M104_M109" in line:
			f.write(line); # don't lose this remark!
		elif skiplines > 0:
			skiplines= skiplines-1;
		elif ";woodified" in line.lower():
			skiplines=4 # skip 4 more lines after our comment
		elif not ";woodgraph" in line.lower(): # forget optional former temp graph lines in the file
			if thisZ == lastPatchZ:
				f.write(line) # no more patch, keep the important end scripts unchanged
			elif not "m104" in line.lower(): # forget any previous temp in the file
				thisZ = getZ(line, formerZ)
				if thisZ != formerZ and thisZ in noises:
					
					if firstTemp != 0 and thisZ<=0.5: # if specifed, keep the first temp for the first 0.5mm
						temp= firstTemp
					else:
						temp= noiseToTemp(noises[thisZ])
						
						# possibly cap temperature change upward
						temp += postponedTempDelta;
						#print("ppdelta=%f\n" % postponedTempDelta)
						postponedTempDelta = 0
						if postponedTempLast!= None and maxUpward > 0 and temp > postponedTempLast + maxUpward:
							postponedTempDelta = temp - (postponedTempLast + maxUpward)
							temp= postponedTempLast + maxUpward
						if temp > maxTemp:
							postponedTempDelta= 0
							temp= maxTemp
						postponedTempLast = temp
						
						f.write( ("M104 S%i" + eol) % temp)

					formerZ = thisZ
					
					# Build the corresponding graph line
					#t = int(19 * noises[thisZ])
					t= int(19 * (temp-minTemp)/(maxTemp-minTemp))
					str = ";WoodGraph: Z %03f " % thisZ
					str += "@%3iC | " % temp
					for i in xrange(0,t):
						str += "#"
					for i in xrange(t+1,20):
						str += "."
					graphStr += str + eol
					
				f.write(line)
	f.write(graphStr+ eol)