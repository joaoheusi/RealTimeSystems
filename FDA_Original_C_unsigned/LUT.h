void diff2d2 
     (float    ht,        /* time step size */
      float    lambda,    /* contrast parameter */
      long     nx,        /* image dimension in x direction */ 
      long     ny,        /* image dimension in y direction */ 
      float    **f,      /* input: original image ;  output: smoothed */
      float * lut_vector);
