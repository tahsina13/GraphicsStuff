#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

#define K 9000000000
#define MICRO 0.000001
#define KILO 1000

#define WIDTH 500
#define HEIGHT 500

#define PARTS 10
#define MAX_Q 100
#define MAX_VOLTS (1.2 * MAX_Q)

double dist(double x1, double y1, double x2, double y2); 
double potential(double q, double d); 
void hue2rgb(double hue, int rgb[3]); 

int main()
{
    srand(time(0)); 

    char buffer[100]; 
    int fd = open("potential_bw.ppm", O_CREAT | O_WRONLY, 0644); 
    snprintf(buffer, sizeof(buffer)-1, "P3 %d %d 255\n", WIDTH, HEIGHT); 
    write(fd, buffer, strlen(buffer)); 

    int fd2 = open("potential_color.ppm", O_CREAT | O_WRONLY, 0644); 
    write(fd2, buffer, strlen(buffer)); 

    double xs[PARTS]; 
    double ys[PARTS]; 
    double qs[PARTS]; 
    for(int i = 0; i < PARTS; ++i)
    {
        xs[i] = rand() % WIDTH; 
        ys[i] = rand() % HEIGHT; 
        qs[i] = ((rand() % MAX_Q) + 1) * MICRO;  
    }

    int rgb[3]; 
    for(int y = 0; y < WIDTH; ++y)
    {
        for(int x = 0; x < HEIGHT; ++x)
        {
            double volts = 0; 
            for(int i = 0; i < PARTS; ++i)
            {
                double d = dist(x, y, xs[i], ys[i]); 
                volts += potential(qs[i], d); 
            }
            double ratio = volts / MAX_VOLTS; 
            if(ratio > 1)       ratio = 1; 
            else if(ratio < 0)  ratio = 0; 

            int shade = 255 * ratio; 
            snprintf(buffer, sizeof(buffer)-1, "%d %d %d ", shade, shade, shade); 
            write(fd, buffer, strlen(buffer)); 

            hue2rgb(ratio, rgb); 
            snprintf(buffer, sizeof(buffer)-1, "%d %d %d ", rgb[0], rgb[1], rgb[2]); 
            write(fd2, buffer, strlen(buffer)); 
        }
        write(fd, "\n", 1); 
        write(fd2, "\n", 1); 
    }

    close(fd); 
    close(fd2); 
}

double dist(double x1, double y1, double x2, double y2)
{
    double dx = x2 - x1; 
    double dy = y2 - y1; 
    return sqrt(dx*dx + dy*dy) * KILO; 
}

double potential(double q, double d)
{
    if(d == 0)  
        return MAX_VOLTS; 
    return K * (q / d); 
}

double get_percent(double tmp)
{
    if(6 * tmp < 1)         return 6 * tmp; 
    else if(2 * tmp < 1)    return 1; 
    else if(3 * tmp < 2)    return 6 * (2.0/3.0 - tmp); 
    return 0; 
}

void hue2rgb(double hue, int rgb[3])
{
    double tmp_r = hue + 1.0/3.0;  
    if(tmp_r > 1) tmp_r -= 1; 
    double tmp_b = hue; 
    double tmp_g = hue - 1.0/3.0; 
    if(tmp_g < 0) tmp_g += 1; 

    rgb[0] = 255 * get_percent(tmp_r); 
    rgb[1] = 255 * get_percent(tmp_b); 
    rgb[2] = 255 * get_percent(tmp_g); 
}
