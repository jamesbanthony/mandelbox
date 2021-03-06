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
#include <cmath>
#include <cstdio>
#include <algorithm>
#include <assert.h>
#include "color.h"
#include "mandelbox.h"
#include <omp.h>

#define COMP_FOLD(x) { (x) = fabs(x) <= 1? (x) : copysign(2,(x))-(x);}

using namespace std;

inline double SQR( double x) { return x*x; }

static void boxFold(vec3 &v)
{
  COMP_FOLD(v.x);
  COMP_FOLD(v.y);
  COMP_FOLD(v.z);
}

static void sphereFold(vec3 &v, double &dfactor, double r2, double rMin2, double rFixed2)
{
  
  if (r2 < rMin2){
    v = v*(rFixed2/rMin2);
    dfactor *= (rFixed2/rMin2);
  }
  else{
    if (r2 < rFixed2){
      v = v*(rFixed2/r2);
      dfactor *= (rFixed2/r2);
    }
  }
}


double MandelBoxDE(const vec3 &p0, const MandelBoxParams &params)
{
  vec3 p = p0;
  
  double rMin2   = SQR(params.rMin);
  double rFixed2 = SQR(params.rFixed);
  double escape  = SQR(params.escape_time);
  double scale   = params.scale;
  double dfactor = 1; 
  double r2=-1;
  
  double c1 = fabs(scale - 1.0);
  double c2 = pow( fabs(scale), 1 - params.num_iter);

  for (int i=0; i < params.num_iter; i++) 
    {
      if (r2 <= escape)
      {
        // box fold
        boxFold(p);
        r2 = p.Dot(p);      

        // sphere fold
        sphereFold(p,dfactor,r2,rMin2,rFixed2);
        
        // scale
        p = p*scale+p0;
        
        dfactor *= fabs(scale);
        assert(dfactor>0);
      }
      //if ( r2 > escape ) break;		
    }
  r2 = p.Magnitude();
  assert(r2>=0);
  double d = (r2 - c1) / dfactor - c2;
  return d;
}


