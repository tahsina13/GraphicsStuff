#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gmath.h"
#include "matrix.h"
#include "ml6.h"


//vector functions
//normalize vetor, should modify the parameter
void normalize( double *vector ) {
    double mag = sqrt(vector[0]*vector[0] + vector[1]*vector[1] + vector[2]*vector[2]);
    vector[0] /= mag;
    vector[1] /= mag;
    vector[2] /= mag;
}

//Return the dot porduct of a . b
double dot_product( double *a, double *b ) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

//Calculate the surface normal for the triangle whose first
//point is located at index i in polygons
double *calculate_normal(struct matrix *polygons, int i) {
    double ax = polygons->m[0][i+1] - polygons->m[0][i];
    double ay = polygons->m[1][i+1] - polygons->m[1][i];
    double az = polygons->m[2][i+1] - polygons->m[2][i];

    double bx = polygons->m[0][i+2] - polygons->m[0][i];
    double by = polygons->m[1][i+2] - polygons->m[1][i];
    double bz = polygons->m[2][i+2] - polygons->m[2][i];

    double * v = (double *) malloc(3 * sizeof(double));
    v[0] = ay*bz - az*by;
    v[1] = az*bx - ax*bz;
    v[2] = ax*by - ay*bx;

    return v;
}

//lighting functions
color get_lighting( double *normal, double *view, color alight, double light[2][3], double *areflect, double *dreflect, double *sreflect) {
    color i;
    color a, d, s;

    a = calculate_ambient(alight, areflect);
    d = calculate_diffuse(light, dreflect, normal);
    s = calculate_specular(light, sreflect, view, normal);

    i.red   = a.red   + d.red   + s.red;
    i.green = a.green + d.green + s.green;
    i.blue  = a.blue  + d.blue  + s.blue;

    limit_color(&i);
    return i;
}

color calculate_ambient(color alight, double *areflect ) {
    color a;

    a.red   = alight.red    * areflect[RED];
    a.green = alight.green  * areflect[GREEN];
    a.blue  = alight.blue   * areflect[BLUE];

    limit_color(&a);
    return a;
}

color calculate_diffuse(double light[2][3], double *dreflect, double *normal ) {
    color d;
    double dp;

    dp = dot_product(light[LOCATION], normal);
    d.red   = light[COLOR][RED]     * dreflect[RED]     * dp; 
    d.green = light[COLOR][GREEN]   * dreflect[GREEN]   * dp;
    d.blue  = light[COLOR][BLUE]    * dreflect[BLUE]    * dp;

    limit_color(&d);
    return d;
}

color calculate_specular(double light[2][3], double *sreflect, double *view, double *normal ) {
    color s;
    double T[3];
    double S[3];
    double R[3];
    double dp;
    int sig;

    dp = dot_product(light[LOCATION], normal);
    T[0] = normal[0] * dp;
    T[1] = normal[1] * dp;
    T[2] = normal[2] * dp;

    S[0] = T[0] - light[LOCATION][0]; 
    S[1] = T[1] - light[LOCATION][1];
    S[2] = T[2] - light[LOCATION][2];

    R[0] = T[0] + S[0];
    R[1] = T[1] + S[1];
    R[2] = T[2] + S[2];

    dp = dot_product(R, view);
    sig = dp >= 0 ? 1 : -1;
    dp = sig * fabs(pow(dp, 8));

    s.red   = light[COLOR][RED]     * sreflect[RED]     * dp;
    s.green = light[COLOR][GREEN]   * sreflect[GREEN]   * dp;
    s.blue  = light[COLOR][BLUE]    * sreflect[BLUE]    * dp;

    limit_color(&s);
    return s;
}

//limit each component of c to a max of 255
void limit_color( color * c ) {
    if( c->red < 0 )
        c->red = 0;
    if( c->red > MAX_COLOR )
        c->red = MAX_COLOR;

    if( c->green < 0 )
        c->green = 0;
    if( c->green > MAX_COLOR )
        c->green = MAX_COLOR;

    if( c->blue < 0 )
        c->blue = 0;
    if( c->blue > MAX_COLOR )
        c->blue = MAX_COLOR;
}
