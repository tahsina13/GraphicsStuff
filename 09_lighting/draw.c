#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "gmath.h"

void draw_line_oct1(int x0, int y0, double z0,
                    int x1, int y1, double z1,
                    screen s, zbuffer zb, color c)
{
    int A = 2 * (y1 - y0); 
    int B = 2 * (x0 - x1); 

    int d = A + B/2; 
    int x = x0, y = y0;
    double m = (z1 - z0) / (x1 - x0 + 1);
    double z = z0;
    while(x <= x1)
    {
        plot(s, zb, c, x, y, z); 
        if(d > 0)
        {
            y++; 
            d += B; 
        }
        x++; 
        d += A; 
        z += m;
    }
}

void draw_line_oct8(int x0, int y0, double z0,
                    int x1, int y1, double z1,
                    screen s, zbuffer zb, color c)
{
    int A = 2 * (y1 - y0); 
    int B = 2 * (x0 - x1); 

    int d = A - B/2; 
    int x = x0, y = y0; 
    double m = (z1 - z0) / (x1 - x0 + 1);
    double z = z0;
    while(x <= x1)
    {
        plot(s, zb, c, x, y, z); 
        if(d < 0)
        {
            y--;
            d -= B; 
        }
        x++; 
        d += A; 
        z += m;
    }
}

void draw_line_oct2(int x0, int y0, double z0,
                    int x1, int y1, double z1,
                    screen s, zbuffer zb, color c)
{
    int A = 2 * (y1 - y0); 
    int B = 2 * (x0 - x1); 

    int d = B + A/2; 
    int x = x0, y = y0; 
    double m = (z1 - z0) / (y1 - y0 + 1);
    double z = z0;
    while(y <= y1)
    {
        plot(s, zb, c, x, y, z); 
        if(d < 0)
        {
            x++;
            d += A; 
        }
        y++; 
        d += B; 
        z += m;
    }
}

void draw_line_oct7(int x0, int y0, double z0,
                    int x1, int y1, double z1,
                    screen s, zbuffer zb, color c)
{
    int A = 2 * (y1 - y0); 
    int B = 2 * (x0 - x1); 

    int d = -B - A/2; 
    int x = x0, y = y0; 
    double m = (z1 - z0) / (y0 - y1 + 1);
    double z = z0;
    while(y >= y1)
    {
        plot(s, zb, c, x, y, z); 
        if(d > 0)
        {
            x++; 
            d += A; 
        }
        y--; 
        d -= B; 
        z += m;
    }
}

void draw_line(int x0, int y0, double z0,
               int x1, int y1, double z1,
               screen s, zbuffer zb, color c)
{
    int xt, yt;
    double zt;
    if(x1 < x0)
    {
        xt = x0;
        x0 = x1;
        x1 = xt;
        yt = y0;
        y0 = y1;
        y1 = yt;
        zt = z0;
        z0 = z1;
        z1 = zt;
    }

    int A = 2 * (y1 - y0); 
    int B = 2 * (x0 - x1); 
    
    if(abs(B) >= abs(A))
    {
        if(A >= 0)
            draw_line_oct1(x0, y0, z0, x1, y1, z1, s, zb, c); 
        else 
            draw_line_oct8(x0, y0, z0, x1, y1, z1, s, zb, c); 
    }
    else 
    {
        if(A >= 0)
            draw_line_oct2(x0, y0, z0, x1, y1, z1, s, zb, c); 
        else 
            draw_line_oct7(x0, y0, z0, x1, y1, z1, s, zb, c); 
    }
}

/*======== void draw_lines() ==========
Inputs:   struct matrix * points
         screen s
         color c 
Returns: 
Go through points 2 at a time and call draw_line to add that line
to the screen
====================*/
void draw_lines( struct matrix * points, screen s, zbuffer zb, color c)
{
    for(int i = 0; i < points->lastcol - 1; i += 2)
    {
        int x0 = points->m[0][i]; 
        int y0 = points->m[1][i]; 
        double z0 = points->m[2][i];

        int x1 = points->m[0][i+1]; 
        int y1 = points->m[1][i+1]; 
        double z1 = points->m[2][i+1];

        draw_line(x0, y0, z0, x1, y1, z1, s, zb, c); 
    }
}

void add_point( struct matrix * points, double x, double y, double z)
{
    if( points->lastcol == points->cols )
        grow_matrix(points, points->cols * 2); 

    points->m[0][points->lastcol] = x; 
    points->m[1][points->lastcol] = y;
    points->m[2][points->lastcol] = z; 
    points->m[3][points->lastcol] = 1; 
    points->lastcol++; 
}

/*======== void add_edge() ==========
Inputs:   struct matrix * points
          int x0, int y0, int z0, int x1, int y1, int z1
Returns: 
add the line connecting (x0, y0, z0) to (x1, y1, z1) to points
should use add_point
====================*/
void add_edge( struct matrix * points, 
	       double x0, double y0, double z0, 
	       double x1, double y1, double z1)
{
    add_point(points, x0, y0, z0); 
    add_point(points, x1, y1, z1); 
}

/*======== void add_circle() ==========
  Inputs:   struct matrix * edges
            double cx
            double cy
            double r
            double step

  Adds the circle at (cx, cy) with radius r to edges
  ====================*/
void add_circle( struct matrix * points,
                 double cx, double cy, double cz,
                 double r, int steps )
{
    double theta;
    double x, y, px, py; 
    px = r + cx; 
    py = cy; 
    for(int i = 1; i <= steps; ++i)
    {
        theta = 2 * M_PI * ((double) i / steps); 
        x = r * cos(theta) + cx; 
        y = r * sin(theta) + cy; 
        add_edge(points, px, py, cz, x, y, cz);  
        px = x; 
        py = y;
    }
}

/*======== void add_curve() ==========
Inputs:   struct matrix *edges
         double x0
         double y0
         double x1
         double y1
         double x2
         double y2
         double x3
         double y3
         double step
         int type

Adds the curve bounded by the 4 points passsed as parameters
of type specified in type (see matrix.h for curve type constants)
to the matrix edges
====================*/

void add_curve( struct matrix *points,
                double x0, double y0,
                double x1, double y1,
                double x2, double y2,
                double x3, double y3,
                int steps, int type )
{
    double t;
    double ax, bx, cx, dx;
    double ay, by, cy, dy;
    double x, y, px, py;

    if(type == TYPE_HERMITE)
    {
        ax = 2*x0 - 2*x1 + x2 + x3;
        bx = -3*x0 + 3*x1 - 2*x2 - x3;
        cx = x2;
        dx = x0;

        ay = 2*y0 - 2*y1 + y2 + y3;
        by = -3*y0 + 3*y1 - 2*y2 - y3;
        cy = y2;
        dy = y0;
    }
    else
    {
        ax = -x0 + 3*x1 - 3*x2 + x3;
        bx = 3*x0 - 6*x1 + 3*x2;
        cx = -3*x0 + 3*x1;
        dx = x0;

        ay = -y0 + 3*y1 - 3*y2 + y3;
        by = 3*y0 - 6*y1 + 3*y2;
        cy = -3*y0 + 3*y1;
        dy = y0;
    }
    px = x0; 
    py = y0;

    for(int i = 1; i <= steps; ++i)
    {
        t = ((double) i / steps);
        x = ax*(t*t*t) + bx*(t*t) + cx*(t) + dx;
        y = ay*(t*t*t) + by*(t*t) + cy*(t) + dy;
        add_edge(points, px, py, 0, x, y, 0);
        px = x; 
        py = y;
    }
}

/*======== void add_polygon() ==========
  Inputs:   struct matrix *polygons
            double x0
            double y0
            double z0
            double x1
            double y1
            double z1
            double x2
            double y2
            double z2
  Returns:
  Adds the vertices (x0, y0, z0), (x1, y1, z1)
  and (x2, y2, z2) to the polygon matrix. They
  define a single triangle surface.
  ====================*/
void add_polygon( struct matrix * polys,
                   double x0, double y0, double z0,
                   double x1, double y1, double z1,
                   double x2, double y2, double z2)
{
    add_point(polys, x0, y0, z0);
    add_point(polys, x1, y1, z1);
    add_point(polys, x2, y2, z2);
}

/*======== void draw_polygons() ==========
  Inputs:   struct matrix *polygons
            screen s
            color c
  Returns:
  Goes through polygons 3 points at a time, drawing
  lines connecting each points to create bounding triangles
  ====================*/
void draw_polygons( struct matrix * polys, screen s, zbuffer zb, color c,
                    double *view, double light[2][3], color ambient,
                    double *areflect, double *dreflect, double *sreflect)
{
    double * normal;
    color il;

    for(int i = 0; i < polys->lastcol - 2; i += 3)
    {
        normal = calculate_normal(polys, i);
        normalize(normal);
        if(dot_product(view, normal) > 0)
        {
            /*
            int x0 = polys->m[0][i];
            int y0 = polys->m[1][i];
            double z0 = polys->m[2][i];

            int x1 = polys->m[0][i+1];
            int y1 = polys->m[1][i+1];
            double z1 = polys->m[2][i+1];

            int x2 = polys->m[0][i+2];
            int y2 = polys->m[1][i+2];
            double z2 = polys->m[2][i+2];

            draw_line(x0, y0, z0, x1, y1, z1, s, zb, c); 
            draw_line(x1, y1, z1, x2, y2, z2, s, zb, c);
            draw_line(x2, y2, z2, x0, y0, z0, s, zb, c);
            */

            il = get_lighting(normal, view, ambient, light, areflect, dreflect, sreflect);
            scanline_convert(polys, i, s, zb, il);
        }
        free(normal);
    }
}

/*======== void add_box() ==========
  Inputs:   struct matrix * edges
            double x
            double y
            double z
            double width
            double height
            double depth

  add the points for a rectagular prism whose
  upper-left-front corner is (x, y, z) with width,
  height and depth dimensions.
  ====================*/
void add_box( struct matrix * edges,
              double x, double y, double z,
              double w, double h, double d )
{
    // front
    add_polygon(edges, x, y - h, z, x + w, y, z, x, y, z);
    add_polygon(edges, x, y - h, z, x + w, y - h, z, x + w, y, z);

    // right
    add_polygon(edges, x + w, y - h, z, x + w, y - h, z - d, x + w, y, z - d);
    add_polygon(edges, x + w, y - h, z, x + w, y, z - d, x + w, y, z);

    // back
    add_polygon(edges, x + w, y - h, z - d, x, y - h, z - d, x, y, z - d);
    add_polygon(edges, x + w, y - h, z - d, x, y, z - d, x + w, y, z - d);

    // right
    add_polygon(edges, x, y - h, z - d, x, y - h, z, x, y, z);
    add_polygon(edges, x, y - h, z - d, x, y, z, x, y, z - d);

    // top
    add_polygon(edges, x, y, z, x + w, y, z, x + w, y, z - d);
    add_polygon(edges, x, y, z, x + w, y, z - d, x, y, z - d);

    // bottom
    add_polygon(edges, x, y - h, z - d, x + w, y - h, z - d, x + w, y - h, z);
    add_polygon(edges, x, y - h, z - d, x + w, y - h, z, x, y - h, z);
}

/*======== void generate_sphere() ==========
  Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r
            int step
  Returns: Generates all the points along the surface
           of a sphere with center (cx, cy, cz) and
           radius r using step points per circle/semicircle.
           Returns a matrix of those points
  ====================*/
struct matrix * generate_sphere(double cx, double cy, double cz,
                                double r, int steps )
{
    struct matrix * sphere = new_matrix(4, 4);

    double phi, theta;
    double x, y, z;

    int rot_start = 0; 
    int rot_end = steps;
    int circ_start = 0; 
    int circ_end = steps;

    for(int i = rot_start; i <= rot_end; ++i)
    {
        phi = 2.0 * M_PI * ((double) i / steps);
        for(int j = circ_start; j <= circ_end; ++j)
        {
            theta = M_PI * ((double) j / steps); 
            x = r * cos(theta) + cx;
            y = r * cos(phi) * sin(theta) + cy;
            z = r * sin(phi) * sin(theta) + cz;
            add_point(sphere, x, y, z);
        }
    }

    return sphere;
}

/*======== void add_sphere() ==========
  Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r
            int step

  adds all the points for a sphere with center (cx, cy, cz)
  and radius r using step points per circle/semicircle.

  Since edges are drawn using 2 points, add each point twice,
  or add each point and then another point 1 pixel away.

  should call generate_sphere to create the necessary points
  ====================*/
void add_sphere( struct matrix * edges,
                 double cx, double cy, double cz,
                 double r, int steps )
{
    struct matrix * sphere = generate_sphere(cx, cy, cz, r, steps);
    
    int lat_start = 0; 
    int lat_end = steps;
    int long_start = 0; 
    int long_end = steps;
    
    for(int i = lat_start; i < lat_end; ++i)
    {
        for(int j = long_start; j < long_end; ++j)
        {
            int k = i * (steps+1) + j;       
            if(j != steps-1)
            {
                add_polygon(edges, 
                        sphere->m[0][k],            sphere->m[1][k],            sphere->m[2][k],
                        sphere->m[0][k+1],          sphere->m[1][k+1],          sphere->m[2][k+1],
                        sphere->m[0][k+steps+2],    sphere->m[1][k+steps+2],    sphere->m[2][k+steps+2]
                        );
            }
            if(j != 0)
            {
                add_polygon(edges, 
                        sphere->m[0][k],            sphere->m[1][k],            sphere->m[2][k],
                        sphere->m[0][k+steps+2],    sphere->m[1][k+steps+2],    sphere->m[2][k+steps+2],
                        sphere->m[0][k+steps+1],    sphere->m[1][k+steps+1],    sphere->m[2][k+steps+1]
                        );
            }
        }
    }

    free_matrix(sphere);
}

/*======== void generate_torus() ==========
  Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r
            int step
  Returns: Generates all the points along the surface
           of a torus with center (cx, cy, cz),
           circle radius r1 and torus radius r2 using
           step points per circle.
           Returns a matrix of those points
  ====================*/
struct matrix * generate_torus( double cx, double cy, double cz,
                                double r1, double r2, int steps )
{
    struct matrix * torus = new_matrix(4, 4);

    double phi, theta;
    double x, y, z;

    int rot_start = 0; 
    int rot_end = steps;
    int circ_start = 0; 
    int circ_end = steps;

    for(double i = rot_start; i <= rot_end; ++i)
    {
        phi = 2.0 * M_PI * ((double) i / steps);
        for(double j = circ_start; j <= circ_end; ++j)
        {
            theta = 2.0 * M_PI * ((double) j / steps);
            x = cos(phi) * (r1 * cos(theta) + r2) + cx;
            y = r1 * sin(theta) + cy;
            z = -sin(phi) * (r1 * cos(theta) + r2) + cz;
            add_point(torus, x, y, z);
        }
    }

    return torus;
}

/*======== void add_torus() ==========
  Inputs:   struct matrix * points
            double cx
            double cy
            double cz
            double r1
            double r2
            double step
  Returns:

  adds all the points required for a torus with center (cx, cy, cz),
  circle radius r1 and torus radius r2 using step points per circle.

  should call generate_torus to create the necessary points
  ====================*/
void add_torus( struct matrix * edges,
                double cx, double cy, double cz,
                double r1, double r2, int steps )
{
    struct matrix * torus = generate_torus(cx, cy, cz, r1, r2, steps);

    int lat_start = 0; 
    int lat_end = steps;
    int long_start = 0; 
    int long_end = steps;

    for(int i = lat_start; i < lat_end; ++i)
    {
        for(int j = long_start; j < long_end; ++j)
        {
            int k = i * (steps+1) + j;
            add_polygon(edges, 
                    torus->m[0][k],         torus->m[1][k],         torus->m[2][k],
                    torus->m[0][k+steps+1], torus->m[1][k+steps+1], torus->m[2][k+steps+1],
                    torus->m[0][k+steps+2], torus->m[1][k+steps+2], torus->m[2][k+steps+2]
                    );
            add_polygon(edges, 
                    torus->m[0][k],         torus->m[1][k],         torus->m[2][k],
                    torus->m[0][k+steps+2], torus->m[1][k+steps+2], torus->m[2][k+steps+2],
                    torus->m[0][k+1],       torus->m[1][k+1],       torus->m[2][k+1]
                    );
        }
    }

    free_matrix(torus);
}

/*======== void draw_scanline() ==========
  Inputs: struct matrix *points
          int i
          screen s
          zbuffer zb
          color c

  Line algorithm specifically for horizontal scanlines
  ====================*/
void draw_scanline(int x0, double z0, int x1, double z1, int y, screen s, zbuffer zb, color c)
{
    int xt;
    double zt;
    if(x0 > x1)
    {
        xt = x0;
        x0 = x1;
        x1 = xt;
        zt = z0;
        z0 = z1;
        z1 = zt;
    }

    int x = x0; 
    double m = (z1 - z0) / (x1 - x0 + 1);
    double z = z0;

    while(x <= x1)
    {
        plot(s, zb, c, x, y, z);
        x++;
        z += m;
    }
}

/*======== void scanline_convert() ==========
  Inputs: struct matrix *points
          int i
          screen s
          zbuffer zb
          color il

  Fills in polygon i by drawing consecutive horizontal (or vertical) lines.
  ====================*/
void scanline_convert( struct matrix *points, int i, screen s, zbuffer zb, color il )
{
    // color c;
    int bot, mid, top;
    double dx0, dz0;
    double dx1, dz1;
    double x0, x1, z0, z1;
    int dy0, dy1;
    int y;

    /*
    c.red   = (((i / 3) + 1) % 6) & 1 ? 255 : 0; 
    c.green = (((i / 3) + 1) % 6) & 2 ? 255 : 0; 
    c.blue  = (((i / 3) + 1) % 6) & 4 ? 255 : 0; 
    */

    if( points->m[1][i] <= points->m[1][i+1] &&
        points->m[1][i] <= points->m[1][i+2] )
        bot = i;
    else if( points->m[1][i+1] <= points->m[1][i] &&
             points->m[1][i+1] <= points->m[1][i+2] )
        bot = i+1;
    else
        bot = i+2;

    if( points->m[1][i] >= points->m[1][i+1] &&
        points->m[1][i] >= points->m[1][i+2] )
        top = i;
    else if( points->m[1][i+1] >= points->m[1][i] &&
             points->m[1][i+1] >= points->m[1][i+2] )
        top = i+1;
    else
        top = i+2;    

    if( bot != i && top != i )
        mid = i;
    else if( bot != i+1 && top != i+1 )
        mid = i+1;
    else
        mid = i+2;

    dx0 = points->m[0][top] - points->m[0][bot];
    dz0 = points->m[2][top] - points->m[2][bot];

    dx1 = points->m[0][mid] - points->m[0][bot];
    dz1 = points->m[2][mid] - points->m[2][bot];

    x0  = x1 = points->m[0][bot];
    z0  = z1 = points->m[2][bot];

    dy0 = (int)points->m[1][top] - (int)points->m[1][bot] + 1;
    dy1 = (int)points->m[1][mid] - (int)points->m[1][bot] + 1;

    y   = (int)points->m[1][bot];
    while(y < (int)points->m[1][mid])
    {
        // draw_line(x0, y, z0, x1, y, z1, s, zb, c);
        draw_scanline(x0, z0, x1, z1, y, s, zb, il);
        if(dy0)
        {
            x0 += dx0 / dy0; 
            z0 += dz0 / dy0; 
        }
        if(dy1)
        {
            x1 += dx1 / dy1;
            z1 += dz1 / dy1;
        }
        y++;
    }
    
    dx1 = points->m[0][top] - points->m[0][mid];
    dz1 = points->m[2][top] - points->m[2][mid];
    
    x1  = points->m[0][mid];
    z1  = points->m[2][mid];

    dy1 = (int)points->m[1][top] - (int)points->m[1][mid] + 1;

    y   = (int)points->m[1][mid];
    while(y <= (int)points->m[1][top])
    {
        // draw_line(x0, y, z0, x1, y, z1, s, zb, c);
        draw_scanline(x0, z0, x1, z1, y, s, zb, il);
        if(dy0)
        {
            x0 += dx0 / dy0; 
            z0 += dz0 / dy0; 
        }
        if(dy1)
        {
            x1 += dx1 / dy1;
            z1 += dz1 / dy1;
        } 
        y++;
    }
}
