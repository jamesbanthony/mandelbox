DFLAGS = -lm
CFLAGS= -pg -O3  -W
CXXFLAGS= -pg -O3  -W
CC=mpicxx
RM=rm

PROGRAM_NAME= mandelbox

$(PROGRAM_NAME): main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o mandelboxde.o raymarching.o renderer.o init3D.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

#$(PROGRAM_NAME2):  main.o print.o timing.o savebmp.o getparams.o 3d.o getcolor.o distance_est.o mandelboxde.o raymarching.o renderer.o init3D.o
#	g++ -o $@ $^ $(CFLAGS) $(LDFLAGS)

#run-serial: $(PROGRAM_NAME2)
#	./$(PROGRAM_NAME2)$(EXEXT) params.dat; viewnior image.bmp

rp: $(PROGRAM_NAME)
	mpirun -np 2 ./$(PROGRAM_NAME)$(EXEXT) params.dat; viewnior image.bmp

c:
	$(RM) *.o $(PROGRAM_NAME)$(EXEEXT) 
