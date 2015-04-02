import sys
start = sys.argv[1]
end = sys.argv[2]

x = 5		# camera starting points
y = 20
z = 40

scale = 1.20	# mandelbox starting points
rMin = 1.0
rFixed = 5.50

for i in range(int(start),int(end)+1):
	f = open("params"+str(i)+".dat", 'w')
	s = str(x)+' '+str(y)+' '+str(z)+'\n'				# camera pos
	s += '0 0 0\n'										# camera pointing
	s += '0 1 0\n'										# up vector
	s += '1.2\n'										# fov
	s += '500 500\n'									# resolution
	s += '-2.00\n'										# detail level
	s += str(scale)+' '+str(rMin)+' '+str(rFixed)+'\n'	# mandelbox (scale,rMin,rFixed)
	s += '15 100\n'										# max iter, escape
	s += '1\n'											# color
	s += '1.2\n'										# brightness
	s += '0\n'											# super aliasing 
	s += 'image'+str(i)+'.bmp'							# image file
	f.write(s)
	f.close()
	
	x = x-0.15										# change in camera pos (per frame)
	y = y-0.15
	z = z-0.15
	
	scale = scale+0 								# change in mandelbox (per frame)
	rMin = rMin+0
	rFixed = rFixed+0