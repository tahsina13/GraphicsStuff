#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"

int main() {

  screen s;
  color c;

  c.red = 0;
  c.green = 255;
  c.blue = 0;

  clear_screen(s);
  //octants 1 and 5
  draw_line(0, 0, XRES-1, YRES-1, s, c);
  draw_line(0, 0, XRES-1, YRES / 2, s, c);
  draw_line(XRES-1, YRES-1, 0, YRES / 2, s, c);

  //octants 8 and 4
  c.blue = 255;
  draw_line(0, YRES-1, XRES-1, 0, s, c);
  draw_line(0, YRES-1, XRES-1, YRES/2, s, c);
  draw_line(XRES-1, 0, 0, YRES/2, s, c);

  //octants 2 and 6
  c.red = 255;
  c.green = 0;
  c.blue = 0;
  draw_line(0, 0, XRES/2, YRES-1, s, c);
  draw_line(XRES-1, YRES-1, XRES/2, 0, s, c);

  //octants 7 and 3
  c.blue = 255;
  draw_line(0, YRES-1, XRES/2, 0, s, c);
  draw_line(XRES-1, 0, XRES/2, YRES-1, s, c);

  //horizontal and vertical
  c.blue = 0;
  c.green = 255;
  draw_line(0, YRES/2, XRES-1, YRES/2, s, c);
  draw_line(XRES/2, 0, XRES/2, YRES-1, s, c);

  display(s);
  save_extension(s, "lines.png");
}
