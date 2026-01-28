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
                 double r, double step )
{
    double x, y, px, py; 
    px = r + cx; 
    py = cy; 
    for(double t = step; t <= 1; t += step)
    {
        x = r * cos(2 * M_PI * t) + cx; 
        y = r * sin(2 * M_PI * t) + cy; 
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
                double step, int type )
{
    double ax, bx, cx, dx;
    double ay, by, cy, dy;
    int x, y, px, py;

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

    for(double t = step; t <= 1; t += step)
    {
        x = ax*(t*t*t) + bx*(t*t) + cx*(t) + dx;
        y = ay*(t*t*t) + by*(t*t) + cy*(t) + dy;
        add_edge(points, px, py, 0, x, y, 0);
        px = x; 
        py = y;
    }
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
