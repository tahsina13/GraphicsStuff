#include <stdio.h>
#include <stdlib.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"

void exch(int *a, int *b)
{
    int tmp = *a; 
    *a = *b; 
    *b = tmp; 
}

void swap_points(int *x0_ptr, int *y0_ptr, int *x1_ptr, int *y1_ptr)
{
    exch(x0_ptr, x1_ptr); 
    exch(y0_ptr, y1_ptr); 
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
        swap_points(&x0, &y0, &x1, &y1); 

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
