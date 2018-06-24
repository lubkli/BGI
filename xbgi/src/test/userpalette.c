/* userpalette.c  -*- C -*-
 * 
 * To compile:
 * gcc -o userpalette userpalette.c /usr/lib/libXbgi.a -lX11 -lm
 * 
 * By Guido Gonzato, September 2, 2013.
 * 
 * This program shows how to implement a user-defined RGB palette.
 * This may be necesary in case you want more than MAXRGBCOLORS colours.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "../graphics.h"

#define SIZE 8192

struct rgb_colour *my_palette;

void setuserpalette (struct rgb_colour *palette)
{
  // let the global RGB palette point to the
  // user-defined palette
  rgb_palette = palette;
}

/* ----- */

int main (void)
{
  int c, gd, gm;
  
  gd = X11;
  gm = X11_1024x768;
  initgraph (&gd, &gm, "");
  
  // allocate memory for the new palette
  my_palette = (struct rgb_colour *)
    malloc((SIZE + 1) * sizeof(struct rgb_colour));
  if (NULL == my_palette) {
    fprintf (stderr, "Not enough memory, sorry.\n");
    exit (1);
  }
  
  // then use it
  setuserpalette (my_palette);

  // make shades of red
  for (c = 0; c < 255; c++)
    setrgbpalette (c, c, 0, 0);
  
  // and also random shades
  for (c = 255; c < SIZE; c++)
    setrgbpalette (c, random(255), random(255), random(255));
  
  // draw some coloured lines
  for (c = 0; c < getmaxx(); c++) {
    setrgbcolor (c);
    line (c, 0, c, getmaxy ());
  }
  
  getch ();
  closegraph (); // my_palette is freed here
  return 0;
  
}
