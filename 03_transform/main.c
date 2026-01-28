#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

struct matrix * get_grid(); 
struct matrix * make_lorentz(double beta); 
void gen_img(char * pathname, double beta); 

int main(int argc, char **argv) {

  screen s;
  struct matrix * edges;
  struct matrix * transform;

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);

  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );

  free_matrix( edges );
  free_matrix( transform );
  
  printf("\nThis may take a while Mr. DW :)\n"); 

  char buf[256]; 
  double step = (3*M_E)/100; 
  for(int i = 0; i < 100; ++i)
  {
    printf("Generating %02d.gif\n", i); 
    getcwd(buf, sizeof(buf)-1); 
    sprintf(buf, "%s/%02d.gif", buf, i); 
    double beta = 1 / exp(i*step) - 1; 
    gen_img(buf, beta); 
  }

  FILE * f; 

  printf("Combing gifs\n"); 
  f = popen("convert -dispose previous -delay 10 -loop 0 *.gif lorentz.gif\n", "r"); 
  pclose(f); 

  printf("Deleting gifs\n"); 
  f = popen("rm [0-9][0-9].gif\n", "r"); 
  pclose(f); 

  printf("Showing gif\n"); 
  f = popen("animate lorentz.gif\n", "r"); 
  pclose(f); 
}

struct matrix * get_grid()
{
    struct matrix * grid = new_matrix(4, 4); 
    double delta = 10.0; 
    for(int i = 0; i <= XRES; i += delta)
        add_edge(grid, i, 0, 0, i , YRES, 0);  
    for(int j = 0; j <= YRES; j += delta)
        add_edge(grid, 0, j, 0, XRES, j, 0); 
    return grid; 
}

double lorentz_factor(double beta)
{
    return 1 / sqrt(1 - beta*beta); 
}

struct matrix * make_lorentz(double beta)
{
    double gamma = lorentz_factor(beta); 
    struct matrix * lorentz = new_matrix(4, 4); 
    ident(lorentz); 
    lorentz->m[0][0] = gamma; 
    lorentz->m[0][1] = -gamma * beta; 
    lorentz->m[1][0] = -gamma * beta; 
    lorentz->m[1][1] = gamma;  
    return lorentz; 
}

void gen_img(char * pathname, double beta)
{
    screen s; 
    color c = { 255, 255, 0 }; 
    struct matrix * grid = get_grid(); 
    struct matrix * transform; 

    // translate middle to origin 
    transform = make_translate(-XRES/2.0, -YRES/2.0, 0); 
    matrix_mult(transform, grid); 

    // lorentz transformation
    transform = make_lorentz(beta); 
    matrix_mult(transform, grid); 

    // zoom in 
    transform = make_scale(4, 4, 1); 
    matrix_mult(transform, grid); 

    // translate back
    transform = make_translate(XRES/2.0, YRES/2.0, 0); 
    matrix_mult(transform, grid); 

    clear_screen(s); 
    draw_lines(grid, s, c); 
    save_extension(s, pathname); 

    free_matrix(grid); 
    free_matrix(transform); 
}
