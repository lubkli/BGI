/* cellular.c  -*- C -*-
 * 
 * To compile:
 * gcc -o cellular cellular.c /usr/lib/libXbgi.a -lX11 -lm
 * 
 * Simple cellular automata, as described at
 * http://mathworld.wolfram.com/ElementaryCellularAutomaton.html
 * 
 * By Guido Gonzato, September 2013.
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
#include <math.h>
#include "../graphics.h"

/* ----- */

void run_rule (int rule)
{
  int i, x, y, maxx, maxy, pixel;
  maxx = getmaxx ();
  maxy = getmaxy ();
  
  for (y = 20; y < 20 + maxx / 2; y++)
    for (x = 1; x < maxx - 1; x++) {
      pixel = 0;
      if (RED == getpixel (x - 1, y - 1))
	pixel += 4;
      if (RED == getpixel (x, y - 1))
	pixel += 2;
      if (RED == getpixel (x + 1, y - 1))
	pixel ++;
      
      for (i = 0; i < 8; i++)
	if ( (rule << i) & 0x80)
	  if (pixel == 7 - i)
	    putpixel (x, y, RED);
	  
    } /* for x */
}

/* ----- */

int main (int argc, char **argv)
{
  
  int rule;
  char s[20];
  
  initwindow (1024, 1024 / 2 + 40);
  setbkcolor (WHITE);
  setcolor (RED);
  cleardevice ();

  while (!kbhit () && !mouseclick ()) {
    if (2 == argc) {
      rule = atoi (argv [1]);
      argc = 0; // only first time!
      srand (rule);
    }
    else
      rule = random (255);
    sprintf (s, "Rule: %d", rule);
    outtextxy (0, 0, s);
    putpixel (getmaxx () / 2, 20, RED);
    run_rule (rule);
    sleep (2);
    cleardevice ();
  }

  closegraph ();
  return 0;
}
