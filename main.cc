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
#include <time.h>
#include <sys/time.h>
#include "camera.h"
#include "renderer.h"
#include "mandelbox.h"
#include <omp.h>

void getParameters(char *filename, CameraParams *camera_params, RenderParams *renderer_params,
		   MandelBoxParams *mandelBox_paramsP);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);
void Hello(void);

MandelBoxParams mandelBox_params;

int thread_count;

int main(int argc, char** argv)
{
  double time;
  struct timeval start, end;
  gettimeofday(&start, NULL);

  thread_count = strtol(argv[1], NULL, 10);

  CameraParams    camera_params;
  RenderParams    renderer_params;
  
  getParameters(argv[2], &camera_params, &renderer_params, &mandelBox_params);

  int image_size = renderer_params.width * renderer_params.height;
  unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));

  init3D(&camera_params, &renderer_params);

  renderFractal(camera_params, renderer_params, image);
  saveBMP(renderer_params.file_name, image, renderer_params.width, renderer_params.height);
  
  free(image);

  gettimeofday(&end, NULL);
  time = end.tv_sec-start.tv_sec+(end.tv_usec - start.tv_usec)*1.e-6;
  printf("Total time time: %f\n", time);

  return 0;
}
