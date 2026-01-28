#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main(int argc, char **argv) {

  screen s;
  struct matrix * edges;
  struct matrix * transform;

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);

  /* print_matrix(make_rotX(M_PI/4)); */
  /* printf("\n"); */
  /* print_matrix(make_rotY(M_PI/4)); */
  /* printf("\n"); */
  /* print_matrix(make_rotZ(M_PI/4)); */
  /* printf("\n"); */

  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );

  
  free_matrix( edges );
  free_matrix( transform );

  // Arch Logo
  color arch_color = { 23, 147, 209 };
  color bg = { 23, 23, 23 };

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);

  parse_file("arch_logo", transform, edges, s);

  reset_color(s, bg);
  draw_lines(edges, s, arch_color);
  display(s);
  save_extension(s, "arch_logo.png");

  free_matrix( edges );
  free_matrix( transform );
}  
