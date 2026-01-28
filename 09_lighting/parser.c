#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"
#include "stack.h"

/*======== void parse_file () ==========
Inputs:   char * filename
          struct stack * csystems
          struct matrix * edges,
          struct matrix * polygons,
          screen s, zbuffer zb
          double *view, color ambient, double light[2][3],
          double *areflect, double *sreflect, double *dreflect);
Returns:

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:

     push: push a copy of the curent top of the coordinate system stack to the stack

     pop: remove the current top of the coordinate system stack

     All the shape commands work as follows:
        1) Add the shape to a temporary matrix
        2) Multiply that matrix by the current top of the coordinate system stack
        3) Draw the shape to the screen
        4) Clear the temporary matrix

         sphere: add a sphere to the POLYGON matrix -
                 takes 4 arguemnts (cx, cy, cz, r)
         torus: add a torus to the POLYGON matrix -
                takes 5 arguemnts (cx, cy, cz, r1, r2)
         box: add a rectangular prism to the POLYGON matrix -
              takes 6 arguemnts (x, y, z, width, height, depth)
         clear: clears the edge and POLYGON matrices

         circle: add a circle to the edge matrix -
                 takes 4 arguments (cx, cy, cz, r)
         hermite: add a hermite curve to the edge matrix -
                  takes 8 arguments (x0, y0, x1, y1, rx0, ry0, rx1, ry1)
         bezier: add a bezier curve to the edge matrix -
                 takes 8 arguments (x0, y0, x1, y1, x2, y2, x3, y3)
         line: add a line to the edge matrix -
               takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
         ident: set the transform matrix to the identity matrix -
         scale: create a scale matrix,
                then multiply the transform matrix by the scale matrix -
                takes 3 arguments (sx, sy, sz)
         move: create a translation matrix,
               then multiply the transform matrix by the translation matrix -
               takes 3 arguments (tx, ty, tz)
         rotate: create a rotation matrix,
                 then multiply the transform matrix by the rotation matrix -
                 takes 2 arguments (axis, theta) axis should be x y or z
         apply: apply the current transformation matrix to the edge and
                POLYGON matrices
         display: clear the screen, then
                  draw the lines of the edge and POLYGON matrices to the screen
                  display the screen
         save: clear the screen, then
               draw the lines of the edge and POLYGON matrices to the screen
               save the screen to a file -
               takes 1 argument (file name)
         quit: end parsing

See the file script for an example of the file format

IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename,
                  struct stack * csystems,
                  struct matrix * edges,
                  struct matrix * polygons,
                  screen s, zbuffer zb,
                  double *view, color ambient, double light[2][3],
                  double *areflect, double *sreflect, double *dreflect)
{
  color c = { 255, 0, 0 }; 
  int steps = 100;
  int steps_3d = 15;

  FILE *f;
  char line[256];

  struct matrix * tmp;
  struct matrix * tmp_3d;

  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
  
  clear_screen(s);
  clear_zbuffer(zb);

  tmp = new_matrix(4, 4);
  tmp_3d = new_matrix(4, 4);

  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);

    /*
    tmp = new_matrix(4, 4);
    tmp_3d = new_matrix(4, 4);
    */
    
    if ( !strcmp(line, "line") )
    {
        fgets(line, 255, f);  
        printf("%s\n", line); 
        double x0, y0, z0, x1, y1, z1; 
        sscanf(line, "%lf %lf %lf %lf %lf %lf", &x0, &y0, &z0, &x1, &y1, &z1);  
        add_edge(tmp, x0, y0, z0, x1, y1, z1); 
    }
    else if ( !strcmp(line, "circle") )
    {
        fgets(line, 255, f);
        double cx, cy, cz, r; 
        sscanf(line, "%lf %lf %lf %lf", &cx, &cy, &cz, &r);
        add_circle(tmp, cx, cy, cz, r, steps);
    }
    else if ( !strcmp(line, "hermite") )
    {
        fgets(line, 255, f); 
        double x0, y0, x1, y1, rx0, ry0, rx1, ry1;
        sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", 
                &x0, &y0, &x1, &y1, &rx0, &ry0, &rx1, &ry1);
        add_curve(tmp, x0, y0, x1, y1, rx0, ry0, rx1, ry1, steps, TYPE_HERMITE);
    }
    else if ( !strcmp(line, "bezier") )
    {
        fgets(line, 255, f); 
        double x0, y0, x1, y1, x2, y2, x3, y3; 
        sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf", 
                &x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3);
        add_curve(tmp, x0, y0, x1, y1, x2, y2, x3, y3, steps, TYPE_BEZIER);
    }
    else if ( !strcmp(line, "sphere") )
    {
        fgets(line, 255, f); 
        double cx, cy, cz, r;
        sscanf(line, "%lf %lf %lf %lf", &cx, &cy, &cz, &r);
        add_sphere(tmp_3d, cx, cy, cz, r, steps_3d);
    }
    else if ( !strcmp(line, "torus") )
    {
        fgets(line, 255, f);
        double cx, cy, cz, r1, r2;
        sscanf(line, "%lf %lf %lf %lf %lf", &cx, &cy, &cz, &r1, &r2);
        add_torus(tmp_3d, cx, cy, cz, r1, r2, steps_3d);
    }
    else if ( !strcmp(line, "box") )
    {
        fgets(line, 255, f); 
        double x, y, z, width, height, depth;
        sscanf(line, "%lf %lf %lf %lf %lf %lf", &x, &y, &z, &width, &height, &depth);
        add_box(tmp_3d, x, y, z, width, height, depth);
    }
    else if ( !strcmp(line, "ident") )
    {
        ident(peek(csystems));  
    }
    else if ( !strcmp(line, "scale") )
    {
         fgets(line, 255, f); 
         double x, y, z; 
         sscanf(line, "%lf %lf %lf", &x, &y, &z); 
         struct matrix * d = make_scale(x, y, z); 
         matrix_mult(peek(csystems), d);
         copy_matrix(d, peek(csystems));
         free_matrix(d); 
    }
    else if ( !strcmp(line, "move") )
    {
        fgets(line, 255, f); 
        double x, y, z; 
        sscanf(line, "%lf %lf %lf", &x, &y, &z); 
        struct matrix * t = make_translate(x, y, z); 
        matrix_mult(peek(csystems), t);
        copy_matrix(t, peek(csystems));
        free_matrix(t); 
    }
    else if ( !strcmp(line, "rotate") )
    {
        fgets(line, 255, f); 
        char axis; 
        double theta; 
        sscanf(line, "%c %lf", &axis, &theta); 
        struct matrix * r; 
        if(axis == 'x')
            r = make_rotX(D2R(theta)); 
        else if(axis == 'y')
            r = make_rotY(D2R(theta)); 
        else 
            r = make_rotZ(D2R(theta)); 
        matrix_mult(peek(csystems), r);
        copy_matrix(r, peek(csystems));
        free_matrix(r); 
    }
    else if ( !strcmp(line, "push") )
    {
       push(csystems);
    }
    else if ( !strcmp(line, "pop") )
    {
        if(csystems->size > 1)
            pop(csystems);
    }
    else if ( !strcmp(line, "apply") )
    {
        matrix_mult(peek(csystems), edges); 
        matrix_mult(peek(csystems), polygons);
    }
    else if ( !strcmp(line, "clear") )
    {
        free_matrix(edges); 
        edges = new_matrix(4, 4);
        free_matrix(polygons);
        polygons = new_matrix(4, 4);
        clear_zbuffer(zb); 
    }
    else if ( !strcmp(line, "display") )
    {
        clear_screen(s); 
        draw_lines(edges, s, zb, c); 
        draw_polygons(polygons, s, zb, c, view, light, ambient, areflect, dreflect, sreflect);
        display(s); 
    }
    else if ( !strcmp(line, "save") )
    {
        fgets(line, 255, f); 
        clear_screen(s); 
        draw_lines(edges, s, zb, c); 
        draw_polygons(polygons, s, zb, c, view, light, ambient, areflect, dreflect, sreflect);
        save_extension(s, line); 
    }
    else if ( !strcmp(line, "quit") )
    {
        free_matrix(tmp);
        free_matrix(tmp_3d);
        break; 
    }

    matrix_mult(peek(csystems), tmp);
    for(int i = 0; i < tmp->lastcol; ++i)
        add_point(edges, tmp->m[0][i], tmp->m[1][i], tmp->m[2][i]);
    tmp->lastcol = 0;
    // free_matrix(tmp);

    matrix_mult(peek(csystems), tmp_3d);
    for(int i = 0; i < tmp_3d->lastcol; ++i)
        add_point(polygons, tmp_3d->m[0][i], tmp_3d->m[1][i], tmp_3d->m[2][i]);
    tmp_3d->lastcol = 0;
    // free_matrix(tmp_3d);
  }

  free_matrix(tmp);
  free_matrix(tmp_3d);

  fclose(f); 
}
  
