#include <math.h>

#include "draw.h"
#include "turtle.h"

void step(struct turtle *t, double length)
{
    int dx = length * cos(d2r(t->angle)); 
    int dy = length * sin(d2r(t->angle)); 
    draw_line(t->x, t->y, t->x + dx, t->y + dy, *(t->s_ptr), t->c);
    t->x += dx; 
    t->y += dy; 
}

void turn(struct turtle *t, double theta)
{
    t->angle += theta; 
    if(t->angle >= 360)
        t->angle -= 360; 
    else if(t->angle < 0)
        t->angle += 360; 
}
