#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pgmfiles.h"
#include "diff2d.h"
#include <x86intrin.h>


#pragma intrinsic(__rdtsc)
//gcc -o fda pgmtolist.c pgmfiles.c diff2d.c main.c -lm

void main (int argc, char **argv) {

  unsigned __int64 maintime;
  char   row[80];
  float  **matrix;
  int i, j;
  FILE   *inimage, *outimage;
  long   imax;
  float  lambda;
  int result;
  eightBitPGMImage *PGMImage;



  /* ---- read image name  ---- */
  
  
  PGMImage = (eightBitPGMImage *) malloc(sizeof(eightBitPGMImage));
  
  

  if (!argv[1])
  {
	  printf("name of input PGM image file (with extender): ");
    scanf("%s", PGMImage->fileName);
  }
  else
  {
    strcpy(PGMImage->fileName, argv[1]);
  }

  result = read8bitPGM(PGMImage);

  if(result < 0) 
    {
      printPGMFileError(result);
      exit(result);
    }

  
  
  /* ---- allocate storage for matrix ---- */
  
  matrix = (float **) malloc (PGMImage->x * sizeof(float *));
  if (matrix == NULL)
    { 
      printf("not enough storage available\n");
      exit(1);
    } 
  for (i=0; i<PGMImage->x; i++)
    {
      matrix[i] = (float *) malloc (PGMImage->y * sizeof(float));
      if (matrix[i] == NULL)
        { 
	  printf("not enough storage available\n");
	  exit(1);
        }
    }
    
  /* ---- read image data into matrix ---- */
 
  
 for (i=0; i<PGMImage->x; i++)
    for (j=0; j<PGMImage->y; j++)
      matrix[i][j] = (float) *(PGMImage->imageData + (i*PGMImage->y) + j); 

  
  /* ---- process image ---- */
  
  
  //~ gets(row);  sscanf(row, "%f", &lambda);
  lambda = 3;
  //scanf("%f", &lambda);
  //printf("number of iterations: ");
  //~ gets(row);  sscanf(row, "%ld", &imax);
  imax = 8;
  //scanf("%ld", &imax);  
  for (i=1; i<=imax; i++)
    {
      //printf("iteration number: %3ld \n", i);
      diff2d (0.5, lambda, PGMImage->x, PGMImage->y, matrix); 
    }
    
  /* ----- copy the Result Image to PGM Image/File structure ----- */
  
  for (i=0; i<PGMImage->x; i++)
    for (j=0; j<PGMImage->y; j++)
      *(PGMImage->imageData + i*PGMImage->y + j) = (char) matrix[i][j];
  
  /* ---- write image ---- */
  
  if (!argv[2])
  {
    printf("name of output PGM image file (with extender): ");
    scanf("%s", PGMImage->fileName);
  }
  else
  {
    strcpy(PGMImage->fileName, argv[2]);
  }

  write8bitPGM(PGMImage);

  
  /* ---- disallocate storage ---- */
  maintime = __rdtsc();
  for (i=0; i<PGMImage->x; i++)
  free(matrix[i]);
  free(matrix);

  free(PGMImage->imageData);
  free(PGMImage);
  

  printf("%I64d ticks3\n", __rdtsc() - maintime);
  //printf("Tempo de execucao: %lf", ((double)maintime)); //conversão para double(maintime);
}


