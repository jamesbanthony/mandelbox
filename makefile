LDFLAGS = -lm -fopenmp -std=c++11
CFLAGS= -pg -O3 -Wall -fopenmp -std=c++11
CXXFLAGS= -pg -O3 -Wall -fopenmp -std=c++11
CC=g++
RM=rm

PROGRAM_NAME= mandelbox

$(PROGRAM_NAME): main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

rs: $(PROGRAM_NAME)
	./$(PROGRAM_NAME)$(EXEXT) params.dat; pqiv image.bmp -c -i

c:
	$(RM) *.o $(PROGRAM_NAME)$(EXEEXT) 
