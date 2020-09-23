#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "LUT.h"


/*--------------------------------------------------------------------------*/

void diff2d2

     (float    ht,        /* time step size, >0, e.g. 0.5 */
      float    lambda,    /* contrast parameter */
      long     nx,        /* image dimension in x direction */
      long     ny,        /* image dimension in y direction */
      float    **f,
      float * lut_vector)       /* input: original image ;  output: smoothed */


/*--------------------------------------------------------------------------*/
/*                                                                          */
/*             NONLINEAR TWO DIMENSIONAL DIFFUSION FILTERING                */
/*                                                                          */
/*                       (Joachim Weickert, 7/1994)                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/


/* Explicit scheme with 9-point stencil and exponential stabilization.      */
/* Conservative, conditionally consistent to the discrete integration       */
/* model, unconditionally stable, preserves maximum-minimum principle.      */


{

long    i, j;                                     /* loop variables */
float   qC, qN, qNE, qE, qSE, qS, qSW, qW, qNW;   /* weights */
float   **g;                                      /* work copy of f */


/* ---- allocate storage for g ---- */

g = (float **) malloc ((nx+2) * sizeof(float *));

for (i=0; i<=nx+1; i++)
    {
      g[i] = (float *) malloc ((ny+2) * sizeof(float));

    }


/* ---- copy f into g ---- */

for (i=1; i<=nx; i++)
 for (j=1; j<=ny; j++)
     g[i][j] = f[i-1][j-1];


/* ---- create dummy boundaries ---- */

for (i=1; i<=nx; i++)
    {
     g[i][0]    = g[i][1];
     g[i][ny+1] = g[i][ny];
    }

for (j=0; j<=ny+1; j++)
    {
     g[0][j]    = g[1][j];
     g[nx+1][j] = g[nx][j];
    }


/* ---- diffusive averaging ---- */
int a;
for (i=1; i<=nx; i++)
 for (j=1; j<=ny; j++)

     {

       /* calculate weights */
       a = (int)fabs(g[i][j]- g[i  ][j+1]);
       qN  = lut_vector[a]; 
       a = (int)fabs(g[i][j]- g[i+1][j+1]);
       qNE = lut_vector[a];
       a = (int)fabs(g[i][j]- g[i+1][j  ]);
       qE  = lut_vector[a];
       a = (int)fabs(g[i][j]- g[i+1][j-1]);
       qSE =  lut_vector[a];
       a = (int)fabs(g[i][j]- g[i  ][j-1]);
       qS  = lut_vector[a];
       a = (int)fabs(g[i][j]- g[i-1][j-1]);
       qSW = lut_vector[a];
       a = (int)fabs(g[i][j]- g[i-1][j  ]);
       qW  = lut_vector[a];
       a = (int)fabs(g[i][j]- g[i-1][j+1]);
       qNW = lut_vector[a];
       qC  = 1.0 - qN - qNE - qE - qSE - qS - qSW - qW - qNW;
       /* weighted averaging */

       f[i-1][j-1] = qNW * g[i-1][j+1] + qN * g[i][j+1] + qNE * g[i+1][j+1] +
                     qW  * g[i-1][j  ] + qC * g[i][j  ] + qE  * g[i+1][j  ] +
                     qSW * g[i-1][j-1] + qS * g[i][j-1] + qSE * g[i+1][j-1];

     }  /* for */


/* ---- disallocate storage for g ---- */

for (i=0; i<=nx+1; i++)
    free(g[i]);
free(g);

return;

} /* diff */


/*--------------------------------------------------------------------------*/

