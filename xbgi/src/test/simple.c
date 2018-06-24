/* simple.c  -*- C -*-
 * 
 * To compile:
 * gcc -o simple simple.c /usr/lib/libXbgi.a -lX11 -lm
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
#include "../graphics.h"

/* ----- */

void check ()
{
  if (KEY_ESC == getch ()) {
    closegraph ();
    exit (0);
  }
}

/* ----- */

int main (void)
{

  int i, col, gd, gm, x, y;
  
  gd = X11;
  gm = X11_1024x768;
  // initgraph (&gd, &gm, "");
  initgraph (NULL, NULL, "");
  
  setbkcolor (BLACK);
  cleardevice ();
  x = getmaxx ();
  y = getmaxy ();

  /* circles */
  while (! kbhit ()) {
    setcolor (YELLOW);
    outtextxy (0, 0, "Press a key to continue");
    for (i = 0; i < 500; i++) {
      setcolor (1 + random (15));
      circle (random(x), random(y), random(100));
    }
    sleep (1);
    cleardevice ();
  }
  check ();
  
  /* lines */
  while (! kbhit ()) {
    setcolor (YELLOW);
    outtextxy (0, 0, "Press a key to continue");
    for (i = 0; i < 500; i++) {
      setlinestyle (SOLID_LINE, 0, 1 + random(THICK_WIDTH));
      setcolor (1 + random (15));
      line (random(x), random(y), random(x), random(y));
    }
    sleep (1);
    cleardevice ();
  }
  check ();

  /* pixels */
  while (! kbhit ()) {
    setcolor (YELLOW);
    outtextxy (0, 0, "Press a key to exit");
    srand (2012);
    for (i = 0; i < 1000; i++) {
      col = 1 + random (MAXCOLORS);
      putpixel (random(x), random(y), col);
      if (kbhit ())
	break;
      delay (1);
    }
    sleep (1);
    srand (2012);
    for (i = 0; i < 1000; i++) {
      col = 1 + random (MAXCOLORS); /* keep random () in sync */
      putpixel (random(x), random(y), BLACK);
      if (kbhit ())
	break;
      delay (1);
    }
    sleep (1);
    cleardevice ();
  }
  
  closegraph ();
  return 0;
  
}
