#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "turtle.h"

#define STARS 50
#define RADIUS (((XRES + YRES) / 2) / STARS)

void draw_star(int x0, int y0, int r, screen *s_ptr, color c); 

int main()
{
    srand(time(0)); 

    screen s; 
    clear_screen(s); 

    color c = {
        c.red = 255,
        c.green = 255,
        c.blue = 255
    }; 

    for(int i = 0; i < STARS; ++i)
    {
        int x = rand() % XRES; 
        int y = rand() % YRES; 
        draw_star(x, y, RADIUS, &s, c); 
    }

    display(s); 
    save_extension(s, "star.png"); 
}

void draw_star(int x0, int y0, int r, screen *s_ptr, color c)
{
    int l = (2*r) / cos(d2r(18)); 

    struct turtle t; 
    t.x = x0 - l/2; 
    t.y = y0 + (l / (2*tan(d2r(72)))); 
    t.angle = 0; 

    t.s_ptr = s_ptr; 
    t.c = c; 

    for(int i = 0; i < 5; ++i)
    {
        step(&t, l); 
        turn(&t, -144); 
    }
}
