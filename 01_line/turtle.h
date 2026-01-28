#include "ml6.h"

#ifndef TURTLE_H
#define TURTLE_H

#define d2r(d) (d * (M_PI / 180))
#define r2d(r) (r * (180 / M_PI))

struct turtle 
{
    int x, y; 
    double angle; 
    screen *s_ptr; 
    color c; 
}; 

void step(struct turtle *t, double length); 
void turn(struct turtle *t, double theta); 

#endif
