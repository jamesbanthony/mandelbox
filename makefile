LDFLAGS = -lm
CFLAGS= -g -O3 -Wall -fopenmp
CXXFLAGS= -g -O3 -Wall -fopenmp
CC=g++
RM=rm

PROGRAM_NAME= mandelbox

$(PROGRAM_NAME): main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

rs: $(PROGRAM_NAME)
	./$(PROGRAM_NAME) 8 $(EXEXT) params.dat; gthumb image.bmp

c:
	$(RM) *.o $(PROGRAM_NAME)$(EXEEXT) 
