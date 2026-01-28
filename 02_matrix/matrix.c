/*==========================================
  A matrix will be a 4xN array of doubles
  Each column will represent an [x, y, z, 1] point.
  For multiplication purposes, consider the rows like so:
  x0  x1      xn
  y0  y1      yn
  z0  z1  ... zn
  1  1        1
  ==========================================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"


/*-------------- void print_matrix() --------------
Inputs:  struct matrix *m

print the matrix such that it looks like
the template in the top comment
*/
void print_matrix(struct matrix *m) 
{
    for(int i = 0; i < m->rows; ++i)
    {
        for(int j = 0; j < m->lastcol; ++j)
            printf("%6.2f ", m->m[i][j]); 
        printf("\n"); 
    }
}

/*-------------- void ident() --------------
Inputs:  struct matrix *m <-- assumes m is a square matrix

turns m in to an identity matrix
*/
void ident(struct matrix *m) 
{
    for(int i = 0; i < m->rows; ++i)
        for(int j = 0; j < m->cols; ++j)
            m->m[i][j] = i == j; 
    m->lastcol = m->cols; 
}


double dot_product(struct matrix *a, int r, struct matrix *b, int c)
{
    double product = 0; 
    for(int i = 0; i < b->rows; ++i)
        product += a->m[r][i] * b->m[i][c];  
    return product; 
}

/*-------------- void matrix_mult() --------------
Inputs:  struct matrix *a
         struct matrix *b

multiply a by b, modifying b to be the product
a*b -> b
*/
void matrix_mult(struct matrix *a, struct matrix *b) 
{
    struct matrix * tmp = new_matrix(b->rows, b->cols); 
    copy_matrix(b, tmp); 
    
    for(int i = 0; i < b->rows; ++i)
        for(int j = 0; j < b->lastcol; ++j)
            b->m[i][j] = dot_product(a, i, tmp, j); 

    free_matrix(tmp); 
}


/*===============================================
  These Functions do not need to be modified
  ===============================================*/

/*-------------- struct matrix *new_matrix() --------------
Inputs:  int rows
         int cols

Once allocated, access the matrix as follows:
m->m[r][c]=something;
if (m->lastcol)...
*/
struct matrix *new_matrix(int rows, int cols) {
  double **tmp;
  int i;
  struct matrix *m;

  tmp = (double **)malloc(rows * sizeof(double *));
  for (i=0;i<rows;i++) {
      tmp[i]=(double *)malloc(cols * sizeof(double));
    }

  m=(struct matrix *)malloc(sizeof(struct matrix));
  m->m=tmp;
  m->rows = rows;
  m->cols = cols;
  m->lastcol = 0;

  return m;
}


/*-------------- void free_matrix() --------------
Inputs:  struct matrix *m
Returns:

1. free individual rows
2. free array holding row pointers
3. free actual matrix
*/
void free_matrix(struct matrix *m) {

  int i;
  for (i=0;i<m->rows;i++) {
      free(m->m[i]);
    }
  free(m->m);
  free(m);
}


/*======== void grow_matrix() ==========
Inputs:  struct matrix *m
         int newcols 
Returns: 

Reallocates the memory for m->m such that it now has
newcols number of collumns
====================*/
void grow_matrix(struct matrix *m, int newcols) {
  
  int i;
  for (i=0;i<m->rows;i++) {
      m->m[i] = realloc(m->m[i],newcols*sizeof(double));
  }
  m->cols = newcols;
}


/*-------------- void copy_matrix() --------------
Inputs:  struct matrix *a
         struct matrix *b 
Returns: 

copy matrix a to matrix b
*/
void copy_matrix(struct matrix *a, struct matrix *b) {

  int r, c;

  for (r=0; r < a->rows; r++) 
    for (c=0; c < a->cols; c++)  
      b->m[r][c] = a->m[r][c];  
}
