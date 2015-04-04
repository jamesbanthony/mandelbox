/*
   This file is part of the Mandelbox program developed for the course
    CS/SE  Distributed Computer Systems taught by N. Nedialkov in the
    Winter of 2015 at McMaster University.

    Copyright (C) 2015 T. Gwosdz and N. Nedialkov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <stdio.h>
#include "camera.h"
#include "renderer.h"
#include "mandelbox.h"
#include <mpi.h>
#include <math.h>

extern double getTime();
void getParameters(char *filename, CameraParams *camera_params, RenderParams *renderer_params,
		   MandelBoxParams *mandelBox_paramsP);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image, int start, int end, int chunk, int rank);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);
void printTime(double time, int rank);

MandelBoxParams mandelBox_params;

int main(int argc, char** argv)
{
  /* MPI */
  int num_proc, my_rank;
  MPI_Status status;
  MPI_Init(NULL, NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);

  CameraParams    camera_params;
  RenderParams    renderer_params;
  
  getParameters(argv[1], &camera_params, &renderer_params, &mandelBox_params);
  init3D(&camera_params, &renderer_params);
  
  /* Determine how many rows of pixels to render per process */
  int chunk_size  = ((renderer_params.height)/num_proc);
  int local_start = (chunk_size * my_rank) + 1;
  int local_end   = (chunk_size * (my_rank +1));
  int s = (chunk_size * renderer_params.width)*3;
  unsigned char *partial_image = (unsigned char*)malloc(s*sizeof(unsigned char));
  int image_size = renderer_params.width * renderer_params.height;
  unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));  

  double totalTime = getTime();
  double time = getTime();
  renderFractal(camera_params, renderer_params, partial_image, local_start, local_end, chunk_size, my_rank); 
  printf("Fractal Generation > ");
  printTime(getTime()-time,my_rank);

  double time2 = getTime();
  MPI_Gather(partial_image,s,MPI_UNSIGNED_CHAR,image,s,MPI_UNSIGNED_CHAR,0,MPI_COMM_WORLD);
  printf("Send to P0         > ");
  printTime(getTime()-time2,my_rank);

  //Collect partial images and merge into final image
  if(my_rank == 0){
    saveBMP(renderer_params.file_name, image, renderer_params.width, renderer_params.height);
    free(image);
    printf("Total Time         > ");
    printTime(getTime()-totalTime,my_rank);
  }

  MPI_Finalize();
}

void printTime(double time, int rank)
{
  int sec = ceil(time);
  int hr = sec/3600;
  int t = sec%3600;
  int min = t/60;
  sec = t%60;
  printf("p%d took %02d:%02d:%02d.\n",rank,hr,min,sec);
}