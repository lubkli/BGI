//
// Project Name: Homework 5 - Bouncing Lines
//
// Name: 
//
// Date  : November 2005

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "graphics2.h"

using namespace std;

#define ESC 0x1b

void main() 
{
	srand((unsigned int)time(NULL));  // Seed the random number generator
	int GraphDriver=0,GraphMode=0;
	initgraph( &GraphDriver, &GraphMode, "", 800, 600 ); // Start Window
	char tempstring[80]="unused";

	//Variable Declarations

	bool KeepGoing=true;
	bool TrailsOn=false;
	char KeyPressed;

	double ballX1, ballY1;
	double prevX1, prevY1;
	double ballX2, ballY2;
	double prevX2, prevY2;
	double XVel1, YVel1;
	double XVel2, YVel2;

	int mouseX, mouseY;

	// Initial Position
	ballX1 = 540.0;
	ballY1 = 150.0;
	ballX2 = 440.0;
	ballY2 = 350.0;

	// Initial Velocity
	XVel1 = 5.8;
	YVel1 = 6.1;
	XVel2 = -5.8;
	YVel2 = 4.7;

	//Main Loop
	while ( KeepGoing ) {
		delay( 25 );

		// -------------------- Move and Control the Ball -------------------------

		// Remember previous position
		prevX1 = ballX1;
		prevY1 = ballY1;
		prevX2 = ballX2;
		prevY2 = ballY2;

		// Move the circle (Add Velocity to Position)
		ballX1 += XVel1;
		ballY1 += YVel1;
		ballX2 += XVel2;
		ballY2 += YVel2;

		// Erase old circles
		setcolor(BLACK);
		circle( (int) prevX1, (int) prevY1, 20);
		circle( (int) prevX2, (int) prevY2, 20);

		// Draw new circles
		setcolor(WHITE);
		circle( (int) ballX1, (int) ballY1, 20);
		circle( (int) ballX2, (int) ballY2, 20);

		// Bounch off left/right walls
		if ( ballX1 > 770.0  || ballX1 < 30.0) {
			XVel1 *= -1.0;
		}
		if ( ballX2 > 770.0  || ballX2 < 30.0) {
			XVel2 *= -1.0;
		}
		// Bounch off top/bottom walls
		if ( ballY1 > 570.0  || ballY1 < 30.0) {
			YVel1 *= -1.0;
		}
		if ( ballY2 > 570.0  || ballY2 < 30.0) {
			YVel2 *= -1.0;
		}


		// -------------- Check to see if a key has been pressed ---------------------
		if (kbhit()) {
			KeyPressed = getch();

			// KEY: Q or ESC pressed
			if (KeyPressed == 'q' || KeyPressed=='Q' || KeyPressed==ESC) {  // q - quit
				KeepGoing = false;
			}//end if (Q)

			//Toggle Trails with T key
			if (KeyPressed == 't' || KeyPressed=='T') {  // q - quit
				if (TrailsOn){
					TrailsOn=false;
				} else {
					TrailsOn=true;
				}
			}//end if (G)

		}//end if kbhit()

		// ------------------ Check to see Mouse was clicked ------------------------

		if (mousedown()) {
			mouseX = mouseclickx();
			mouseY = mouseclicky();

			// ------- Respond to mouse click ---------


		}//end if mousedown()
	} // end while kbhit

} //end of main()
