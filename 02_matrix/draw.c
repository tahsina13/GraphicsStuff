#include <stdio.h>
#include <stdlib.h>

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
