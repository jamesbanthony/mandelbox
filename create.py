import random
import sys
start = 1
end = 120
if (len(sys.argv) > 1):
	end = sys.argv[1]

x = 14.0		# camera starting points
y = 14.0
z = 14.0

scale = 1.5		# mandelbox starting points
rMin = 1.0
rFixed = 2.0

for i in range(int(start),int(end)+1):
	f = open("params"+str(i)+".dat", 'w')
	s = str(x)+' '+str(y)+' '+str(z)+'\n'				# camera pos
	s += '0 0 0\n'										# camera pointing
	s += '0 1 0\n'										# up vector
	s += '1.2\n'										# fov
	s += '500 500\n'									# resolution
	s += '-3.5\n'										# detail level
	s += str(scale)+' '+str(rMin)+' '+str(rFixed)+'\n'	# mandelbox (scale,rMin,rFixed)
	s += '15 100\n'										# max iter, escape
	s += '1\n'											# color
	s += '1.2\n'										# brightness
	s += '0\n'											# super aliasing 
	s += 'image'+str(i)+'.bmp'							# image file
	f.write(s)
	f.close()
	
	x = x*0.97				# change in camera pos (per frame)
	y = y*0.98
	z = z*0.99
	
	scale = scale+0 		# change in mandelbox (per frame)
	rMin = rMin+0
	rFixed = rFixed+0
