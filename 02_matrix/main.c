#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"

#define SCALE 10

void draw_route(screen s, color c); 

int main() {

    /* Matrix Test */

    struct matrix *A = new_matrix(4, 4); 
    for(int j = 0; j < 4; ++j)
    {
        for(int i = 0; i < 4; ++i)
            A->m[i][j] = (i+1) * (j+1); 
        A->lastcol++; 
    }
    printf("Matrix A:\n"); 
    print_matrix(A); 
    printf("\n"); 

    struct matrix *B = new_matrix(4, 4); 
    ident(B); 
    printf("Identity B:\n"); 
    print_matrix(B); 
    printf("\n"); 

    matrix_mult(A, B);  
    printf("Multiply Identity:\n"); 
    print_matrix(B); 
    printf("\n"); 

    B->lastcol = 0; 
    for(int j = 0; j < 4; ++j)
    {
        for(int i = 0; i < 4; ++i)
            B->m[i][j] = (4-i) * (4-j); 
        B->lastcol++; 
    }
    printf("Matrix B:\n"); 
    print_matrix(B); 
    printf("\n"); 

    matrix_mult(A, B); 
    printf("Multiply A * B:\n"); 
    print_matrix(B); 
    printf("\n"); 

    free_matrix(A); 
    free_matrix(B); 

    /* Edges Test */

    srand(time(0)); 

    color bg_color = from_hex("#ebdbb2"); 
    color colors[] = {
        from_hex("#cc241d"),    // red
        from_hex("#98971a"),    // green
        from_hex("#d79921"),    // yellow
        from_hex("#458588"),    // blue
        from_hex("#b16286"),    // purple     
        from_hex("#d65d0e"),    // orange
    };

    screen s;
    reset_color(s, bg_color); 

    for(int i = 0; i < 4; ++i)
        draw_route(s, colors[rand()%6]); 

    display(s); 
    save_extension(s, "metro.png"); 

}  

void add_route_edge(struct matrix * points, 
        double x0, double y0, 
        double x1, double y1)
{
    double mx, my; 

    if(fabs(y1-y0) > fabs(x1-x0))
    {
        // longer than wide
        if(y1 > y0)
        {
            mx = x1; 
            my = y0 + fabs(x1-x0); 
        }
        else 
        {
            mx = x0; 
            my = y1 + fabs(x1-x0); 
        }
    }
    else 
    {
        // wider than long
        if(x1 > x0)
        {
            my = y1; 
            mx = x0 + fabs(y1-y0); 
        }
        else 
        {
            my = y0; 
            mx = x1 + fabs(y1-y0); 
        }
    }

    add_edge(points, x0, y0, 0, mx, my, 0); 
    add_edge(points, mx, my, 0, x1, y1, 0); 
}

void draw_route(screen s, color c)
{
    int stops = rand()%4 + 2; 
    int stations[5][2]; 
    
    for(int i = 0; i < stops; ++i)
    {
        stations[i][0] = (rand() % (XRES/SCALE)) * SCALE + (SCALE/2); 
        stations[i][1] = (rand() % (YRES/SCALE)) * SCALE + (SCALE/2); 
        for(int k = 0; k < i; ++k)
        {
            if(stations[k][0] == stations[i][0] ||
                stations[k][1] == stations[i][1])
            {
                i--; 
                break;  
            }
        }
    }

    struct matrix * edges = new_matrix(4, 5); 

    set_stroke_weight(SCALE * 2); 
    for(int i = 0; i < stops-1; ++i)
    {
        add_route_edge(edges, 
                stations[i][0], stations[i][1], 
                stations[i+1][0], stations[i+1][1]); 

        plot(s, c, stations[i][0], stations[i][1]);
    }
    plot(s, c, stations[stops-1][0], stations[stops-1][1]);

    set_stroke_weight(SCALE); 
    draw_lines(edges, s, c); 
    free_matrix(edges); 
}
