/* mousetest.c  -*- C -*-
 * 
 * To compile:
 * gcc -o mousetest mousetest.c /usr/lib/libXbgi.a -lX11 -lm
 * 
 * By Guido Gonzato, October 2013.
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

int main (void)
{

  int gd, gm, c, p, x, y;
  
  gd = X11;
  gm = X11_800x600;
  initgraph (&gd, &gm, "");
  setbkcolor (BLACK);
  setcolor (RED);
  cleardevice ();
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  
  outtextxy (getmaxx() / 2, getmaxy () / 2 - 15, 
	     "Left click to draw a random circle;");
  outtextxy (getmaxx() / 2, getmaxy () / 2, 
	     "right click to fill;");
  outtextxy (getmaxx() / 2, getmaxy () / 2 + 15, 
	     "press a key twice to exit.");
  getevent ();
  cleardevice ();
  setcolor (YELLOW);

  while (!kbhit ()) {
    
    if (p = mouseclick ()) {
      if (WM_LBUTTONDOWN == p) {
	setcolor (YELLOW);
	circle (mousex (), mousey (), random (150));
      }
      else 
	if (WM_RBUTTONDOWN == p) {
	  setcolor (random (MAXCOLORS));
	  // floodfill (mousex (), mousey (), YELLOW);
	  _floodfill (mousex (), mousey ());
	}
  }
    
  } // while

  closegraph ();
  return 0;
  
}
