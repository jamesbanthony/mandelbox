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

void getParameters(char *filename, CameraParams *camera_params, RenderParams *renderer_params,
		   MandelBoxParams *mandelBox_paramsP);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image, int start, int end, int chunk, int rank);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);
void mergeImages  (unsigned char* image, unsigned char* partial_image, int chunk_size, int my_rank, int width);

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
  unsigned char *partial_image = (unsigned char*)malloc(3*(renderer_params.width*chunk_size)*sizeof(unsigned char));

  renderFractal(camera_params, renderer_params, partial_image, local_start, local_end, chunk_size, my_rank); 
  
  printf("process %d has completed rendering...", my_rank);
  
  //Collect partial images and merge into final image
  if(my_rank == 0){
    printf("process 0 receiving partial images...");

    int image_size = renderer_params.width * renderer_params.height;
    unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));

    for(int r = 0; r < num_proc; r++){
      unsigned char *temp_image = (unsigned char*)malloc(3*(renderer_params.width*chunk_size)*sizeof(unsigned char));
      MPI_Recv(temp_image, (chunk_size*renderer_params.width), MPI_UNSIGNED_CHAR, r, 0 ,MPI_COMM_WORLD, &status);
      mergeImages(image, temp_image, chunk_size, r, renderer_params.width);
    }
    saveBMP(renderer_params.file_name, image, renderer_params.width, renderer_params.height);
    free(image);
  }

  MPI_Finalize();
}

void mergeImages(unsigned char* image, unsigned char* partial_image, int chunk_size, int my_rank, int width){
  int r = my_rank;
  int c = chunk_size;
  int w = width;
  int total_chunk = (c*w);

  for(int i = 0; i < total_chunk; i++){
    image[(r*total_chunk)+i] = partial_image[i];
  }
}