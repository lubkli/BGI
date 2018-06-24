/* hopalong.c  -*- C -*-
 * 
 * To compile:
 * gcc -o hopalong hopalong.c /usr/lib/libXbgi.a -lX11 -lm
 * 
 * By Guido Gonzato, March 2013.
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
#include <math.h>

#include "../graphics.h"

/* ----- */

int sgn (float n)
{
  return (n < 0) ? -1: 1;
}

/* ----- */

int main (int argc, char **argv)
{
  int gd, gm;
  int col;
  unsigned int seed;
  long int counter;
  float j, k, x, y, xx, xp, yp, r, xoffs, yoffs;
  
  if (argc == 2)
    seed = atoi (argv [1]); /* no checks! */
  else {  
    printf ("Seed: ");
    j = scanf ("%d", &seed);
  }
  srand (seed);
  // random each time:
  // srand (time(NULL));
  
  gd = X11;
  gm = getmaxmode ();
  initgraph(&gd, &gm, "");
  
  setbkcolor (BLACK);
  cleardevice ();
  setcolor (YELLOW);
  outtextxy (0, 0, "Press a key or click to exit: ");
  
  xoffs = getmaxx () / 2;
  yoffs = getmaxy () / 3;
  j = random (100);
  k = random (100);
  x = y = xx = xp = yp = r = 0.0;
  col = 1; /* colours codes run from 0 (black) to 15 (white) */
  counter = 0;
  setcolor (col);

  while (!kbhit () && !mouseclick ()) {
    xx = y - sgn (x) * sqrt (abs (k * x - 1));
    y = j - x;
    x = xx;
    xp = x * 2 + xoffs;
    yp = y * 2 + yoffs;
    _putpixel (xp, yp);
    if (++counter == 20000) {
      counter = 0;
      col++;
      if (col == 16)
	col = 1;
    }
    setcolor (col);
  }
   
  closegraph ();
  return 0;
}
