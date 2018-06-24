/* mousetest.c  -*- C -*-
 * 
 * To compile:
 * gcc -o mousetest mousetest.c /usr/lib/libXbgi.a -lX11 -lm
 * 
 * By Guido Gonzato, August 2013.
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
  char s[20];
  
  gd = X11;
  gm = X11_1024x768;
  initgraph (&gd, &gm, "");
  setbkcolor (BLACK);
  setcolor (RED);
  cleardevice ();
  settextjustify (CENTER_TEXT, CENTER_TEXT);
  outtextxy (getmaxx() / 2, getmaxy () / 2, 
	     "Click the RIGHT button to begin");
  while (!ismouseclick (WM_RBUTTONDOWN))
    ;
  getmouseclick(WM_RBUTTONDOWN, &x, &y);
  cleardevice ();
  
  rectangle (0, 0, 200, 20);
  settextjustify (LEFT_TEXT, TOP_TEXT);
  outtextxy (12, 4, "Click around (or here to exit)");
  setlinestyle (SOLID_LINE, 0, THICK_WIDTH);
  
  while (!kbhit ()) {
    
    p = mouseclick ();
    if (p > 0)
      if ( (mousex () < 200) && (mousey () < 20))
	break;
    else 
      if (p < WM_MOUSEMOVE) {
	setfillstyle (1 + random (USER_FILL - 1),
		      COLOR (random(255), random(255), random(255)));
	setcolor (COLOR (random(255), random(255), random(255)));
	if (0 == random (2))
	  fillellipse (mousex (), mousey (),
		       5 + p*random(15), 5 + p*random(15));
	else
	  bar (mousex () - p*10, mousey () - p*10,
	       mousex () + p*10, mousey () + p*10);
      }
    else // mouse move
      _putpixel (mousex (), mousey ());
    
  } // while

  closegraph ();
  return 0;
  
}
