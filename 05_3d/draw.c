#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"

void exch(int *a, int *b)
{
    int tmp = *a; 
    *a = *b; 
    *b = tmp; 
}

void draw_line_oct1(int x0, int y0, int x1, int y1, screen s, color c)
{
    int A = 2 * (y1 - y0); 
    int B = 2 * (x0 - x1); 

    int d = A + B/2; 
    int x = x0, y = y0; 
    while(x <= x1)
    {
        plot(s, c, x, y); 
        if(d > 0)
        {
            y++; 
            d += B; 
        }
        x++; 
        d += A; 
    }
}

void draw_line_oct8(int x0, int y0, int x1, int y1, screen s, color c)
{
    int A = 2 * (y1 - y0); 
    int B = 2 * (x0 - x1); 

    int d = A - B/2; 
    int x = x0, y = y0; 
    while(x <= x1)
    {
        plot(s, c, x, y); 
        if(d < 0)
        {
            y--;
            d -= B; 
        }
        x++; 
        d += A; 
    }
}

void draw_line_oct2(int x0, int y0, int x1, int y1, screen s, color c)
{
    int A = 2 * (y1 - y0); 
    int B = 2 * (x0 - x1); 

    int d = B + A/2; 
    int x = x0, y = y0; 
    while(y <= y1)
    {
        plot(s, c, x, y); 
        if(d < 0)
        {
            x++;
            d += A; 
        }
        y++; 
        d += B; 
    }
}

void draw_line_oct7(int x0, int y0, int x1, int y1, screen s, color c)
{
    int A = 2 * (y1 - y0); 
    int B = 2 * (x0 - x1); 

    int d = -B - A/2; 
    int x = x0, y = y0; 
    while(y >= y1)
    {
        plot(s, c, x, y); 
        if(d > 0)
        {
            x++; 
            d += A; 
        }
        y--; 
        d -= B; 
    }
}

void draw_line(int x0, int y0, int x1, int y1, screen s, color c) 
{
    if(x1 < x0)
    {
        exch(&x0, &x1); 
        exch(&y0, &y1); 
    }

    int A = 2 * (y1 - y0); 
    int B = 2 * (x0 - x1); 
    
    if(abs(B) >= abs(A))
    {
        if(A >= 0)
            draw_line_oct1(x0, y0, x1, y1, s, c); 
        else 
            draw_line_oct8(x0, y0, x1, y1, s, c); 
    }
    else 
    {
        if(A >= 0)
            draw_line_oct2(x0, y0, x1, y1, s, c); 
        else 
            draw_line_oct7(x0, y0, x1, y1, s, c); 
    }
}

void draw_lines( struct matrix * points, screen s, color c)
{
    for(int j = 0; j < points->lastcol; j += 2)
    {
        int x0 = points->m[0][j]; 
        int y0 = points->m[1][j]; 

        int x1 = points->m[0][j+1]; 
        int y1 = points->m[1][j+1]; 

        draw_line(x0, y0, x1, y1, s, c); 
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

void add_edge( struct matrix * points, 
	       double x0, double y0, double z0, 
	       double x1, double y1, double z1)
{
    add_point(points, x0, y0, z0); 
    add_point(points, x1, y1, z1); 
}

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

void add_box( struct matrix * edges,
              double x, double y, double z,
              double width, double height, double depth )
{
    add_edge(edges, x, y, z, x + width, y, z); 
    add_edge(edges, x + width, y, z, x + width, y - height, z); 
    add_edge(edges, x + width, y - height, z, x, y - height, z); 
    add_edge(edges, x, y - height, z, x, y, z);

    add_edge(edges, x, y, z - depth, x + width, y, z - depth); 
    add_edge(edges, x + width, y, z - depth, x + width, y - height, z - depth); 
    add_edge(edges, x + width, y - height, z - depth, x, y - height, z - depth); 
    add_edge(edges, x, y - height, z - depth, x, y, z - depth);

    add_edge(edges, x, y, z, x, y, z - depth); 
    add_edge(edges, x, y - height, z, x, y - height, z - depth); 

    add_edge(edges, x + width, y, z, x + width, y, z - depth); 
    add_edge(edges, x + width, y - height, z, x + width, y - height, z - depth);
}

void add_sphere( struct matrix * edges,
                 double cx, double cy, double cz,
                 double r, int steps )
{
    double phi, theta;
    double x, y, z;
    for(int i = 0; i <= steps; ++i)
    {
        phi = 2 * M_PI * ((double) i / steps);
        for(int j = 0; j <= steps; ++j)
        {
            theta = M_PI * ((double) j / steps); 
            x = r * cos(theta) + cx;
            y = r * cos(phi) * sin(theta) + cy;
            z = r * sin(phi) * sin(theta) + cz;
            add_edge(edges, x, y, z, x, y, z);
        }
    }
}

struct matrix * generate_sphere(double cx, double cy, double cz,
                                double r, int steps )
{
    struct matrix * edges = new_matrix(4, 4);
    add_sphere(edges, cx, cy, cz, r, steps);
    return edges;
}

void add_torus( struct matrix * edges,
                double cx, double cy, double cz,
                double r1, double r2, int steps )
{
    double phi, theta;
    double x, y, z;
    for(double i = 0; i <= steps; ++i)
    {
        phi = 2 * M_PI * ((double) i / steps);
        for(double j = 0; j <= steps; ++j)
        {
            theta = 2 * M_PI * ((double) j / steps);
            x = cos(phi) * (r1 * cos(theta) + r2) + cx;
            y = r1 * sin(theta) + cy;
            z = -sin(phi) * (r1 * cos(theta) + r2) + cz;
            add_edge(edges, x, y, z, x, y, z);
        }
    }
}

struct matrix * generate_torus( double cx, double cy, double cz,
                                double r1, double r2, int steps )
{
    struct matrix * edges = new_matrix(4, 4);
    add_torus(edges, cx, cy, cz, r1, r2, steps);
    return edges;
}
