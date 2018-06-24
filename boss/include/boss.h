#if !defined(__CD_BOSS_H)
#define __CD_BOSS_H

#if defined(WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>

    #if defined(BOSS_DLL)
        #define BOSS_API __declspec(dllexport)
    #elif defined(BOSS_LIB)
        #define BOSS_API
    #else
        #define BOSS_API __declspec(dllimport)
    #endif
#endif

#if !defined(BOSS_API)
    #define BOSS_API
#endif 

/// \mainpage BOSS API Documentation
///
/// \section copyright Copyright Information
///
/// BGI Over SDL Subsystem
///
/// Copyright (C) 2004.  Jeff Schiller (Jeff@CodeDread.com)
///
/// \section revhist Revision History
///
/// <TABLE>
/// <TR>
/// <TD ALIGN="CENTER"><B>Version</B></TD>  <TD><B>Date</B></TD>  <TD><B>Notes</B></TD></TR>
/// <TR><TD ALIGN="CENTER">
/// \ref history "0.0"
/// </TD>
/// <TD>2004-03-15</TD>
/// <TD>
/// \ref history
/// prior to documentation using Doxygen</TD></TR>
/// <TR><TD ALIGN="CENTER">0.1</TD>
/// <TD>2004-03-23</TD>
/// <TD>Initial version, contains support for a good deal of BGI functionality, 
/// some bios.h and conio.h and sound effect support.  Baseline Doxygen documentation in place.</TD></TR>
/// <TR><TD ALIGN="CENTER">0.2</TD>
/// <TD>2004-03-31</TD>
/// <TD>Made a reasonable attempt to correctly handle graphics errors<BR>
/// Added some simple mouse functions<BR>
/// Added fullscreen modes for 320x200 and 320x240<BR>
/// Implemented Music functions
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.3</TD>
/// <TD>2004-04-02</TD>
/// <TD>Fixed GetMouseState() to correctly pump the SDL events<BR>
/// Fixed GetMouseState() to correctly set the button states<BR>
/// Improved putpixel speed by creating an internal color table for VGA colors.
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.31</TD>
/// <TD>2004-04-06</TD>
/// <TD>Updated Sound_Status to optionally check if a particular channel is playing<BR>
/// Fixed implementation of bioskey(0), getch() and bioskey(1).<BR>
/// Added ignoring of mouse events in InitializeMouse as they are captured by SDL_GetMouseState()
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.32</TD>
/// <TD>2004-04-06</TD>
/// <TD>Modified Sound_Load and Music_Load to only accept const char pointers
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.33</TD>
/// <TD>2004-04-25</TD>
/// <TD>Added a RawInitColors() function.  
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.34</TD>
/// <TD>2004-04-26</TD>
/// <TD>Fixed a getch() bug. 
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.35</TD>
/// <TD>2004-04-27</TD>
/// <TD>_sound and _music objects were not being properly constructed 
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.36</TD>
/// <TD>2004-05-01</TD>
/// <TD>Fixed a bug where music functions weren't initializing audio (only Sound_Load() was)
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.37</TD>
/// <TD>2004-05-01</TD>
/// <TD>Added a function to do an inverse lookup to get the VGA color number.
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.40</TD>
/// <TD>2004-05-22</TD>
/// <TD>Added missing USER_FILL to setfillstyle().<BR>
/// Implemented getfillpattern(), getfillsettings() and getmaxcolor().<BR>
/// Updates to code for Linux build.  Thanks to George Mouyios for working on this
/// and letting me use his Linux machine.
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.41</TD>
/// <TD>2004-05-22</TD>
/// <TD>Fixed a minor bug in getch() for special characters.
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.42</TD>
/// <TD>2004-06-03</TD>
/// <TD>Updated font paths for Linux compatibility (forward-slashes in path)<BR>
/// Implemented kbhit() function in Conio.<BR>
/// Moved RawGetAscii helper function outside of Conio namespace.<BR>
/// Added a means of uninitializing the mouse.
/// </TD></TR>
/// <TR><TD ALIGN="CENTER">0.43</TD>
/// <TD>2004-07-01</TD>
/// <TD>Corrected font sizes</TD>
/// <TD>Bug Fix:  bioskey(0) now works for non-ASCII keys</TD>
/// </TR>
/// <TR><TD ALIGN="CENTER">0.44</TD>
/// <TD>2005-08-23</TD>
/// <TD>Added a function: KeyboardHardReset() to allow applications to switch cleanly between Boss and native SDL keyboard handling</TD>
/// </TR>
/// <TR><TD ALIGN="CENTER">0.45</TD>
/// <TD>2006-05-03</TD>
/// <TD>Added optional arguments to Sound_Play() and Music_Play() to adjust the volume</TD>
/// </TR>
/// </TABLE>
///
/// \section license License Stuff
///
/// This library is free software; you can redistribute it and/or
/// modify it under the terms of the GNU General Public
/// License as published by the Free Software Foundation; either
/// version 2 of the License, or (at your option) any later version.
///
/// This library is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
/// Lesser General Public License for more details.
///
/// You should have received a copy of the GNU General Public
/// License along with this library; if not, write to the Free Software
/// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
///
/// \section intro Introduction
///
/// See \ref history page for more details.
/// 
/// BOSS started out with the goal of rewriting the Borland Graphic Interface (BGI) library
/// for use in "modern" Windows systems.  The primary goal would be to port software written
/// for BGI to BOSS as transparently as possible.
///
/// Along the way, BOSS started expanding into other archaic functions such as getch(), 
/// bioskey() and some sound libraries we had used in the past.  Thus, BOSS is more than just
/// graphics, it supports some keyboard, mouse and sound effect functionality.
/// 
/// Unfortunately, I could not find a way to convert Borland's fonts into TrueType fonts, so
/// I am using existing TTF fonts that attempt to mimic the "style" of the Borland stroke
/// fonts and then tried to size them appropriately for BOSS.  This may lead to small text
/// formatting issues with existing code.
///
/// Another caveat is that BGI did not support windowed graphic modes or anything above
/// 640x480 16 color (EGA) mode.  Modern Windows systems support 24-bit and 32-bit color
/// modes by default and also allow non-fullscreen applications (arbitrarily sized windows).
/// As a result, I have added a couple modes into initgraph() to support windowed modes.
/// However, putimage and getimage work only assuming EGA colors, not anything higher (this
/// was done for compatibility sake and should be updated in the future to support any 
/// color depth).
///
/// One other important point:  All graphic operations use a software surface and are not
/// hardware-accelerated.  If you want hardware acceleration, I suggest you use SDL natively.
///
/// DISCLAIMERS:  1) This library is provided AS IS and without any warranty implied or 
/// otherwise.  I am publishing this library for review, education and amusement purposes only.
/// 2) It is advised that you start learning SDL or some other modern API as a great deal of 
/// functionality is missing from BGI, etc.  SDL ain't that hard!
///
/// \section bgi BGI
///
/// <p>Major pieces of functionality that are missing from BOSS' implementation of BGI are:
/// <br>&nbsp;&nbsp; - Line styles (all lines are solid)
/// <br>&nbsp;&nbsp; - Error codes (using graphresult) is not at all consistent, complete or correct
/// <br>&nbsp;&nbsp; - See the Boss::Graphics::BGI::Not_Implemented namespace for a list of all functions not yet implemented</p>
/// <br><br>
/// 
/// For a detailed list of all things implemented, click on "Namespace Members" and then
/// click on "Functions".
///

/// \page history Detailed Pre-History of BOSS
///
///     <p>Here is a detailed pre-history of BOSS development:</p>
///
///     <table border="1">
///     <tr>
///         <td width="20%" bgcolor="#ffffff">Date</td>
///         <td bgcolor="#ffffff">Notes</td>
///     </tr>
///     <tr>
///         <td width="20%">2004-03-01</td>
///         <td>
///         Implemented working versions of the following functions:</p>
///         <br>graphics.h: initgraph() - can only be used for 640x480x32, windowed mode
///         <br>graphics.h: cleardevice() - clears the window/screen
///         <br>graphics.h: graphresult()
///         <br>graphics.h: setbkcolor() - sets current background color
///         <br>graphics.h: setcolor() - sets current drawing color
///         <br>graphics.h: setfillstyle() - sets current fill style
///         <br>graphics.h: bar() - draws a filled rectangle (only EMPTY_FILL or SOLID_FILL at the moment)
///         </td>
///     </tr>
///     <tr>
///         <td width="20%">2004-03-02</td>
///         <td>Implemented working versions of the following functions:
///         <br>conio.h: clrscr()
///         <br>conio.h: delay()
///         <br>conio.h; getch() - for ASCII characters only
///         <br>graphics.h: arc() - simple line drawing 
///         <br>graphics.h: circle() - simple line drawing
///         <br>graphics.h: closegraph()
///         <br>graphics.h: clearviewport() - only to clear the screen
///         <br>graphics.h: ellipse() - simple line drawing
///         <br>graphics.h: fillellipse()
///         <br>graphics.h: fillpoly() - for all polygons with 4 or less sides
///         <br><i>graphics.h: filltriangle() - my own function</i>
///         <br>graphics.h: getmaxx()
///         <br>graphics.h: getmaxy()
///         <br>graphics.h: line() - primitive rasterization for a general line
///         <br>graphics.h: putpixel()
///         <br>graphics.h: rectangle()
///         </td>
///     </tr>
///     <tr>
///         <td width="20%">2004-03-03</td>
///         <td>Implemented working versions of the following functions:
///         <br>graphics.h: outtextxy() - only for horizontal text
///         <br>graphics.h: settextjustify()
///         <br>graphics.h: settextstyle() - used TTF and Windows fonts, sizes are different
///         </td>
///     </tr>
///     <tr>
///         <td width="20%">2004-03-04</td>
///         <td>
///         graphics.h: fixed putimage() for 16-color EGA drawings, only implemented COPY_PUT
///         <br>graphics.h: fixed all implemented functions so they properly update their rectangles of interest
///         <br>graphics.h: fixed outtextxy() for vertical text
///         <br>graphics.h: fixed a problem in my triangle drawing routine
///         </td>
///     </tr>
///     <tr>
///         <td width="20%">2004-03-05</td>
///         <td>
///         Updated the font sizes used by the game
///         <br>arc() - fixed drawing, was drawing arc mirrored in y
///         <br>implemented randomize()
///         <br>implemented random()
///         <br>implemented getpixel()
///         <br>fixed an error in outtextxy (for empty strings an assertion was being tripped)
///         <br>coded getch() to be a reasonably good working function
///         <br>implemented bioskey() - basic version that does bioskey(0) and hack version of bioskey(1)
///         </td>
///     </tr>
///     <tr>
///         <td width="20%">2004-03-06</td>
///         <td>
///         Fixed getch() to be working 100%
///         <br>got bioskey(0) working
///         <br>fixed arc(), circle(), ellipse() and fillellipse() to render smoother curves
///         <br>implemented fill patterns (only EMPTY_FILL, SOLID_FILL and INTERLEAVE_FILL)
///         <br>implemented setfillpattern() for USER_FILL
///         <br>added ability to go fullscreen (software surface)
///         <br>Implemented first cut of sound system:
///         <br>&nbsp;&nbsp; Sound_Load() - implemented for WAV files, not VOC
///         <br>&nbsp;&nbsp; Sound_Unload()
///         <br>&nbsp;&nbsp; Sound_Translate() - not needed now (does nothing)
///         <br>&nbsp;&nbsp; Sound_Play()
///         <br>&nbsp;&nbsp; Sound_Status()
///         <br>&nbsp;&nbsp; Sound_Stop()
///         </td>
///     </tr>
///     <tr>
///         <td width="20%">2004-03-07</td>
///         <td>
///         Fixed Num Keypad to return ASCII values if NumLock is on
///         <br>Fixed Num Keypad to return other key values (Enter, +, -, etc)
///         <br>Fixed bioskey(1) - this causes animation to seem to work
///         </td>
///     </tr>
///     <tr>
///         <td width="20%">2004-03-09</td>
///         <td>
///         Fixed bioskey to work with key-repeats, etc.
///         </td>
///     </tr>
///     <tr>
///         <td width="20%">>2004-03-10</td>
///         <td>
///         Implemented functions:
///         <br>&nbsp;&nbsp; getbkcolor()
///         <br>&nbsp;&nbsp; getcolor()
///         <br>&nbsp;&nbsp; gettextsettings()
///         <br>&nbsp;&nbsp; getx()
///         <br>&nbsp;&nbsp; gety()
///         <br>&nbsp;&nbsp; lineto()
///         <br>&nbsp;&nbsp; linerel()
///         <br>&nbsp;&nbsp; moverel()
///         <br>&nbsp;&nbsp; moveto()
///         <br>&nbsp;&nbsp; outtext()
///         <br>&nbsp;&nbsp; textheight()
///         <br>&nbsp;&nbsp; textwidth()
///         </td>
///     </tr>
///
///     <tr>
///         <td width="20%">2004-03-11</td>
///         <td>
///         Fixed getch() to work with bioskey() by clearing the last key down
///         (this was causing 'q' keystrokes to sometimes get "stuck" in the buffer, I believe
///         this also fixed the alt-tab problem)
///         <br>Gutted audio code to use SDL_mixer instead.  Now sounds can be mixed
///         on top of one another (I have done this for step sounds, but not for any others in COX)
///         </td>
///     </tr>
///
///     <tr>
///         <td width="20%">2004-03-12</td>
///         <td>
///         Implemented drawpoly()
///         <br>Split out BOSS functionality into its own Library/DLL.
///         </td>
///     </tr>
///
///     <tr>
///         <td width="20%">2004-03-13</td>
///         <td>
///         Implemented fillpoly() for n greater than 4
///         <br>Fixed drawpoly(), DrawHLine() for negative widths
///         <br>Implemented getimage()
///         <br>Implemented imagesize()
///         </td>
///     </tr>
///
///     <tr>
///         <td width="20%">2004-03-14</td>
///         <td>
///         Modified Sound_Play() and Sound_Stop() to enable looping and
///         stopping of individual sounds
///         </td>
///     </tr>
///
///     </table>
///
///     <p>Some of the things I know I still have to do (in rough order of priority):</p>
///     <p>
///     <b><u>Known Bugs / Things to Complete:</u></b>
///     <br>&nbsp;&nbsp; Replace all PC speaker sound events with WAV files instead
///     </p>
///     <p>Other things to consider:
///     <br>&nbsp;&nbsp; Writing Doxygen documentation for BOSS.
///     <br>&nbsp;&nbsp; Compiling COX without MSVCRT library.
///     </p>
///     <p>
///     <b><u>Music Functions:</u></b>
///     <br>&nbsp;&nbsp; Music_Load()
///     <br>&nbsp;&nbsp; Music_Unload()
///     <br>&nbsp;&nbsp; Music_Register()
///     <br>&nbsp;&nbsp; Music_Stop()
///     <br>&nbsp;&nbsp; Music_Translate()
///     <br>&nbsp;&nbsp; Music_Play()
///     <br>&nbsp;&nbsp; Music_Resume()
///     <br>&nbsp;&nbsp; Music_Status()
///     </p>
///     <p>
///     <b><u>Basic functions:</u></b>
///     <br>&nbsp;&nbsp; clrscr() - might not bother with
///     <br>&nbsp;&nbsp; sound() - might not bother with
///     <br>&nbsp;&nbsp; nosound() - might not bother with
///     </p>
///     <p>
///     <b><u>Graphics functions:</u></b>
///     <br><b>Not yet implemented:</b>
///     <br>&nbsp;&nbsp; bar3d()
///     <br>&nbsp;&nbsp; detectgraph()
///     <br>&nbsp;&nbsp; floodfill()
///     <br>&nbsp;&nbsp; getarccoords()
///     <br>&nbsp;&nbsp; getaspectratio()
///     <br>&nbsp;&nbsp; getdefaultpalette()
///     <br>&nbsp;&nbsp; getdrivername()
///     <br>&nbsp;&nbsp; getfillpattern()
///     <br>&nbsp;&nbsp; getfillsettings()
///     <br>&nbsp;&nbsp; getgraphmode()
///     <br>&nbsp;&nbsp; getlinesettings()
///     <br>&nbsp;&nbsp; getmaxcolor()
///     <br>&nbsp;&nbsp; getmaxmode()
///     <br>&nbsp;&nbsp; getmodename()
///     <br>&nbsp;&nbsp; getmoderange()
///     <br>&nbsp;&nbsp; getpalette()
///     <br>&nbsp;&nbsp; getpalettesize()
///     <br>&nbsp;&nbsp; getviewsettings()
///     <br>&nbsp;&nbsp; graphdefaults()
///     <br>&nbsp;&nbsp; grapherrormsg()
///     <br>&nbsp;&nbsp; installuserdriver() - probably not necessary
///     <br>&nbsp;&nbsp; installuserfont()
///     <br>&nbsp;&nbsp; pieslice()
///     <br>&nbsp;&nbsp; registerbgidriver() - probably not necessary
///     <br>&nbsp;&nbsp; registerbgifont() - probably not necessary
///     <br>&nbsp;&nbsp; restorecrtmode()
///     <br>&nbsp;&nbsp; sector()
///     <br>&nbsp;&nbsp; setactivepage()
///     <br>&nbsp;&nbsp; setallpalette()
///     <br>&nbsp;&nbsp; setaspectratio()
///     <br>&nbsp;&nbsp; setgraphbufsize()
///     <br>&nbsp;&nbsp; setgraphmode()
///     <br>&nbsp;&nbsp; setlinestyle()
///     <br>&nbsp;&nbsp; setpalette()
///     <br>&nbsp;&nbsp; setrgbpalette()
///     <br>&nbsp;&nbsp; settextstyle() - fix for remaining fonts
///     <br>&nbsp;&nbsp; setusercharsize()
///     <br>&nbsp;&nbsp; setviewport()
///     <br>&nbsp;&nbsp; setvisualpage()
///     <br>&nbsp;&nbsp; setwritemode()
///     <br>&nbsp;&nbsp; clearmouseclick() (WIN)
///     <br>&nbsp;&nbsp; getactivepage() (WIN)
///     <br>&nbsp;&nbsp; getmouseclick() (WIN)
///     <br>&nbsp;&nbsp; getvisualpage() (WIN)
///     <br>&nbsp;&nbsp; initwindow() (WIN)
///     <br>&nbsp;&nbsp; ismouseclick() (WIN)
///     <br>&nbsp;&nbsp; kbhit() (WIN)
///     <br>&nbsp;&nbsp; mousex() (WIN)
///     <br>&nbsp;&nbsp; mousey() (WIN)
///     <br>&nbsp;&nbsp; registermousehandler() (WIN)
///     <br><b>RGB macros:</b>
///     <br>&nbsp;&nbsp; COLOR(r,g,b),
///     <br>&nbsp;&nbsp; RED_VALUE(v)
///     <br>&nbsp;&nbsp; GREEN_VALUE(v)
///     <br>&nbsp;&nbsp; BLUE_VALUE(v),
///     <br>&nbsp;&nbsp; IS_BGI_COLOR(v), IS_RGB_COLOR(v)
///     </p>

// TO DO: Move these to boss.cpp when done testing
#include <SDL.h>
#include <SDL_TTF.h>
#include <SDL_Mixer.h>

/// Boss Namespace
namespace Boss {

BOSS_API float getVersion();

/// Graphics Namespace
namespace Graphics {

/// BGI Namespace
namespace BGI {

// CONSTANTS

/// Standard Error Values from BGI
enum graph_errors {
    grOk = 0,                   ///< No error  
    grNoInitGraph = -1,         ///< (BGI) graphics not installed (use initgraph)  
    grNotDetected = -2,         ///< Graphics hardware not detected  
    grFileNotFound = -3,        ///< Device driver file not found  
    grInvalidDriver = -4,       ///< Invalid device driver file  
    grNoLoadMem = -5,           ///< Not enough memory to load driver  
    grNoScanMem = -6,           ///< Out of memory in scan fill  
    grNoFloodMem = -7,          ///< Out of memory in flood fill  
    grFontNotFound     = -8,    ///< Font file not found  
    grNoFontMem = -9,           ///< Not enough memory to load font  
    grInvalidMode = -10,        ///< Invalid graphics mode for selected driver  
    grError = -11,              ///< Graphics error  
    grIOerror = -12,            ///< Graphics I/O error  
    grInvalidFont = -13,        ///< Invalid font file  
    grInvalidFontNum = -14,     ///< Invalid font number  
    grInvalidDeviceNum = -15,   ///< Invalid device number  
    grInvalidVersion = -18      ///< Invalid version number  
    };

/// Graphic Driver constants
///
/// Most are obsolete, must use BOSS_FULLSCREEN, BOSS_WINDOWED or DETECT
enum BGIDriver {
    DETECT = 0,                 ///< (requests autodetect)  
    CGA = 1,                    ///< OBSOLETE, do not use
    MCGA = 2,                   ///< OBSOLETE, do not use
    EGA = 3,                    ///< OBSOLETE, do not use
    EGA64 = 4,                  ///< OBSOLETE, do not use
    EGAMONO = 5,                ///< OBSOLETE, do not use
    IBM8514 = 6,                ///< OBSOLETE, do not use
    HERCMONO = 7,               ///< OBSOLETE, do not use
    ATT400 = 8,                 ///< OBSOLETE, do not use
    VGA = 9,                    ///< OBSOLETE, do not use
    PC3270 = 10,                ///< OBSOLETE, do not use
    BOSS_FULLSCREEN = 100,      ///< Use for fullscreen graphics modes in BOSS
    BOSS_WINDOWED = 101         ///< Use for windowed graphics modes in BOSS
    };

// Graphic Modes
//
// Most are obsolete, must use BFS_xxxx modes
const int CGAC0 = 0; // 320 x 200 C0 1  
const int CGAC1 = 1; // 320 x 200  C1  1  
const int CGAC2 = 2; // 320 x 200  C2  1  
const int CGAC3 = 3; // 320 x 200  C3  1  
const int CGAHI = 4; // 640 x 200  2 color  1  
const int MCGAC0 = 0; //  320 x 200  C0  1  
const int MCGAC1 = 1; //  320 x 200  C1  1  
const int MCGAC2 = 2; //  320 x 200  C2  1  
const int MCGAC3 = 3; //  320 x 200  C3  1  
const int MCGAMED = 4; // 640 x 200  2 color  1  
const int MCGAHI = 5;  // 640 x 480  2 color  1  
const int EGALO = 0; //  640 x 200  16 color  4  
const int EGAHI = 1; //  640 x 350  16 color  2  
const int EGA64LO = 0; //  640 x 200  16 color  1  
const int EGA64HI = 1; //  640 x 350  4 color  1  
const int EGAMONOHI = 3; //  640 x 350  2 color  1 w/64K  
const int HERCMONOHI  = 0; //  720 x 348  2 color  2  
const int ATT400C0 = 0;  // 320 x 200  C0  1  
const int ATT400C1 = 1;  // 320 x 200  C1  1  
const int ATT400C2 = 2;  // 320 x 200  C2  1  
const int ATT400C3 = 3;  // 320 x 200  C3  1  
const int ATT400MED = 4; //640 x 200  2 color  1  
const int ATT400HI = 5; //  640 x 400  2 color  1  
const int VGALO = 0; //  640 x 200  16 color  2  
const int VGAMED = 1; //  640 x 350  16 color  2  
const int VGAHI = 2; //  640 x 480  16 color  1  
const int PC3270HI = 0; // 720 x 350  2 color  1  
const int IBM8514HI = 0; //  640 x 480  256 color  ?  
const int IBM8514LO = 0; //  1024 x 768  256 color  ?  

/// BOSS_FULLSCREEN modes:
enum BOSSGraphicsModes {
    BFS_640_480 = 0,        ///< 640x480
    BFS_800_600 = 1,        ///< 800x600
    BFS_1024_768 = 2,       ///< 1024x768
    BFS_1280_1024 = 3,      ///< 1280x1024
    BFS_1600_1200 = 4,      ///< 1600x1200
    BFS_320_200 = 5,        ///< 1600x1200
    BFS_320_240 = 6,        ///< 1600x1200
};

/// EGA Colors
enum EGAColor {
    BLACK = 0,              ///< EGA color 0
    BLUE = 1,               ///< EGA color 1
    GREEN = 2,              ///< EGA color 2
    CYAN = 3,               ///< EGA color 3
    RED = 4,                ///< EGA color 4
    MAGENTA = 5,            ///< EGA color 5
    BROWN = 6,              ///< EGA color 6
    LIGHTGRAY = 7,          ///< EGA color 7
    DARKGRAY = 8,           ///< EGA color 8
    LIGHTBLUE = 9,          ///< EGA color 9
    LIGHTGREEN = 10,        ///< EGA color 10
    LIGHTCYAN = 11,         ///< EGA color 11
    LIGHTRED = 12,          ///< EGA color 12
    LIGHTMAGENTA = 13,      ///< EGA color 13
    YELLOW = 14,            ///< EGA color 14
    WHITE = 15              ///< EGA color 15
    };

/// Fill Pattern Styles
enum BGIFillPatterns {
    EMPTY_FILL = 0,         ///<  Fill with background color  
    SOLID_FILL = 1,         ///<  Solid fill  
    LINE_FILL = 2,          ///<  Fill with ---  
    LTSLASH_FILL = 3,       ///<  Fill with ///  
    SLASH_FILL = 4,         ///<  Fill with ///, thick lines  
    BKSLASH_FILL = 5,       ///<  Fill with \\\, thick lines  
    LTBKSLASH_FILL = 6,     ///<  Fill with \\\ lines 
    HATCH_FILL = 7,         ///<  Light hatch fill  
    XHATCH_FILL = 8,        ///<  Heavy crosshatch fill  
    INTERLEAVE_FILL = 9,    ///<  Interleaving line fill  
    WIDE_DOT_FILL = 10,     ///< Widely spaced dot fill  
    CLOSE_DOT_FILL = 11,    ///< Closely spaced dot fill  
    USER_FILL = 12,         ///<  User-defined fill pattern  
    MAX_FILL_STYLES = 13    ///<  Used internally only
    };

/// Font Styles
enum BGIFont {
    DEFAULT_FONT = 0,       ///<  8x8 bit-mapped font  
    TRIPLEX_FONT = 1,       ///<  Stroked triplex font  
    SMALL_FONT = 2,         ///<  Stroked small font  
    SANS_SERIF_FONT = 3,    ///<  Stroked sans-serif font  
    GOTHIC_FONT = 4,        ///<  Stroked gothic font  
    SCRIPT_FONT = 5,        ///<  Stroked script font  
    SIMPLEX_FONT = 6,       ///<  Stroked triplex script font  
    TRIPLEX_SCR_FONT = 7,   ///<  Stroked triplex script font  
    COMPLEX_FONT = 8,       ///<  Stroked complex font  
    EUROPEAN_FONT = 9,      ///<  Stroked European font  
    BOLD_FONT = 10,         ///<  Stroked bold font
    MAX_FONTS = 11          ///<  Used internally only
    };

/// Font Directions
enum BGIFontDirection {
    HORIZ_DIR = 0,      ///< Horizontal text
    VERT_DIR = 1        ///< Vertical text
};

// Text Justification constants
const int LEFT_TEXT = 0;    ///< left-justify horizontal text
const int CENTER_TEXT = 1;  ///< center horizontal or vertical text
const int RIGHT_TEXT = 2;   ///< right-justify horizontal text
const int BOTTOM_TEXT = 0;  ///< bottom-justify vertical text
const int TOP_TEXT = 2;     ///< top-justify vertical text

// Put operation constants
enum BGIPutOperation {
    COPY_PUT = 0,       ///< Copy  
    XOR_PUT = 1,        ///< Exclusive OR, Not Currently Supported
    OR_PUT = 2,         ///< Inclusive OR, Not Currently Supported
    AND_PUT = 3,        ///< AND, Not Currently Supported
    NOT_PUT = 4         ///< Copy the inverse of the source, Not Currently Supported
    };

// BGI Structures
struct textsettingstype {
   int font;
   int direction;
   int charsize;
   int horiz;
   int vert;
};

struct fillsettingstype {
    int pattern;
    int color;
};

} // namespace BGI

// Math values
const float PI = 3.1415926535897932384626433832795f;


// Structures BOSS uses internally
struct grResolution {
    int w;
    int h;
};

struct grPoint {
    int x;
    int y;
};

struct grFont {
    TTF_Font* pFont;
    int charSize;
};

// Internal variables used by the graphics_sdl module
// TO DO:  Move this to graphics_sdl.cpp when done testing
struct GraphVars {
    // Last error used by graphresult
    int lastErr;

    // This is the one and only screen SDL Surface
    SDL_Surface* theScreen;

    // This is the current drawing color
    Uint32 fgColor;

    // This is the current background color
    Uint32 bgColor;

    // This is the current fill pattern and color
    int fillPattern;
    Uint32 fillColor;

    // Graphics "pen" position
    int penX;
    int penY;

    // flag to indicate whether TTF module is initialized
    bool bTTFInitialized;

    // Font Styles
    grFont fonts[BGI::MAX_FONTS];

    // Current Font - manipulated by settextstyle
    int curFont;
    int fontDirection;

    // Current Text Justification settings
    int justH;
    int justV;

    // Current Text size
    int curCharSize;

public:
    GraphVars();
};
// This is how applications can get access to the SDL Surface
BOSS_API extern GraphVars grVars;
// This is the VGA color map
BOSS_API extern Uint32 VgaColorsInUint32[256];
// This is the inverse map
BOSS_API int getVgaColor(Uint32 col32);

/// Raw namespace
namespace Raw {
    BOSS_API void RawInitColors(SDL_Surface* pSurf);

    BOSS_API void RawDrawHLine(SDL_Surface* screen, int x, int y, int width, Uint32 color, int fillPattern = BGI::SOLID_FILL);
    BOSS_API void RawDrawVLine(SDL_Surface* screen, int x, int y, int height, Uint32 color, int fillPattern = BGI::SOLID_FILL);
BOSS_API void RawDrawLine(SDL_Surface* screen, int x1, int y1, int x2, int y2, Uint32 color);
BOSS_API void RawFillTriangle(SDL_Surface* screen, grPoint* pts, Uint32 color, int fillPattern);
BOSS_API Uint32 RawGetPixel(SDL_Surface* screen, int x, int y);
BOSS_API void RawDrawPixel(SDL_Surface* screen, int x, int y, Uint32 color);
BOSS_API void Sulock(SDL_Surface* screen);
BOSS_API void Slock(SDL_Surface* screen);
} // namespace Raw

// implemented
namespace BGI {

// my custom functions (for testing)
/// Enhanced namespace
namespace Enhanced {
BOSS_API void filltriangle(grPoint* pts);
} // namespace Enhanced


BOSS_API void arc(int x, int y, int stangle, int endangle, int radius);
BOSS_API void bar(int left, int top, int right, int bottom);
BOSS_API void circle(int x, int y, int radius);
BOSS_API void cleardevice(void);
BOSS_API void clearviewport(void);
BOSS_API void closegraph(void);
BOSS_API void drawpoly(int numpoints, int *polypoints);
BOSS_API void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius);
BOSS_API void fillellipse(int x, int y, int xradius, int yradius);
BOSS_API void fillpoly(int numpoints, int* polypoints);
BOSS_API int getbkcolor(void);
BOSS_API int getcolor(void);
BOSS_API void getimage(int left, int top, int right, int bottom, void *bitmap);
BOSS_API int getmaxx(void);
BOSS_API int getmaxy(void);
BOSS_API void gettextsettings(struct textsettingstype* texttypeinfo);
BOSS_API int getx(void);
BOSS_API int gety(void);
BOSS_API unsigned getpixel(int x, int y);
BOSS_API int graphresult(void);
BOSS_API unsigned imagesize(int left, int top, int right, int bottom);
BOSS_API void initgraph(int* graphdriver, int* graphmode, char* pathtodriver);
BOSS_API void line(int x1, int y1, int x2, int y2);
BOSS_API void linerel(int dx, int dy);
BOSS_API void lineto(int x, int y);
BOSS_API void moverel(int dx, int dy);
BOSS_API void moveto(int x, int y);
BOSS_API void outtext(char *textstring);
BOSS_API void outtextxy (int x, int y, char* textstring);
BOSS_API void putimage(int left, int top, void* bitmap, int op);
BOSS_API void putpixel(int x, int y, int color); 
BOSS_API void rectangle(int left, int top, int right, int bottom);
BOSS_API void setbkcolor(int color);
BOSS_API void setcolor(int color); 
BOSS_API void setfillpattern (char* upattern, int color); 
BOSS_API void setfillstyle(int pattern, int color); 
BOSS_API void settextjustify(int horiz, int vert);
BOSS_API void settextstyle(int font, int direction, int charsize);
BOSS_API int textheight (char* textstring);
BOSS_API int textwidth (char* textstring);

/// Not_Implemented namespace
namespace Not_Implemented {

BOSS_API void bar3d(int left, int top, int right, int bottom, int depth, int topflag);
BOSS_API void detectgraph(int *graphdriver, int *graphmode);
BOSS_API void floodfill(int x, int y, int border);
BOSS_API void getarccoords(struct arccoordstype *arccoords);
BOSS_API void getaspectratio(int *xasp, int *yasp);
BOSS_API struct palettetype* getdefaultpalette(void);
BOSS_API char* getdrivername(void);
BOSS_API void getfillpattern(char *pattern); 
BOSS_API void getfillsettings(struct fillsettingstype* fillinfo);
BOSS_API int getgraphmode(void);
BOSS_API void getlinesettings(struct linesettingstype* lineinfo);
BOSS_API int getmaxcolor(void);
BOSS_API int getmaxmode(void); 
BOSS_API char* getmodename(int mode_number);
BOSS_API void getmoderange(int graphdriver, int *lomode, int *himode);
BOSS_API void getpalette(struct palettetype *palette);
BOSS_API int getpalettesize(void); 
BOSS_API void getviewsettings(struct viewporttype *viewport);
BOSS_API void graphdefaults(void);
BOSS_API char* grapherrormsg(int errorcode);
BOSS_API int installuserdriver(char *name, int (*detect)(void)); 
BOSS_API int installuserfont(char *name); 
BOSS_API void pieslice(int x, int y, int stangle, int endangle, int radius);
BOSS_API int registerbgidriver(void (*driver)(void)); 
BOSS_API int registerbgifont(void (*font)(void)); 
BOSS_API void restorecrtmode(void); 
// Windows-specific...probably will never implement
//BOSS_API void clearmouseclick(int kind); 
//BOSS_API int getactivepage(void); 
//BOSS_API void getmouseclick(int kind, int& x, int& y); 
//BOSS_API int getvisualpage(void); 
//BOSS_API void initwindow(int width, int height); 
//BOSS_API bool ismouseclick(int kind); 
//BOSS_API int mousex(void); 
//BOSS_API int mousey(void); 
//BOSS_API void registermousehandler(int kind, void h(int, int)); 
//
//  RGB macros: 
//                      COLOR(r,g,b), 
//                      RED_VALUE(v), GREEN_VALUE(v), BLUE_VALUE(v), 
//                      IS_BGI_COLOR(v), IS_RGB_COLOR(v)

BOSS_API void sector (int x, int y, int stangle, int endangle, int xradius, int yradius);
BOSS_API void setactivepage (int page);
BOSS_API void setallpalette (struct palettetype *palette); 
BOSS_API void setaspectratio (int xasp, int yasp);
BOSS_API unsigned setgraphbufsize (unsigned bufsize); 
BOSS_API void setgraphmode (int mode); 
BOSS_API void setlinestyle (int linestyle, unsigned upattern, int thickness);
BOSS_API void setpalette (int colornum, int color); 
BOSS_API void setrgbpalette (int colornum, int red, int green, int blue); 
BOSS_API void setusercharsize (int multx, int divx, int multy, int divy);
BOSS_API void setviewport (int left, int top, int right, int bottom, int clip);
BOSS_API void setvisualpage (int page);
BOSS_API void setwritemode (int mode);
} // namespace Not_Implemented

} // namespace BGI

} // namespace Graphics

// keyboard-specific structure
const int MAX_KEYS = 256;

struct Keyboard {

    // this is used to store the last key pressed during bioskey(1) so 
    // that bioskey(0) can return it
    int keyPressedInQueue;
    SDL_keysym keyPressedInQueueKeySym;

    // used by bioskey(1) to keep track of the last key down (for key repeats)
    int lastKeyDown;

    // used to do key repeats
    Uint32 timerStart;
    Uint32 timerOverFlow;

};

extern Keyboard keyboard;

BOSS_API void KeyboardHardReset();

/// Conio namespace
namespace Conio {

    // Constants

    // Key Constants
    const int KEY_HOME     = 71;
    const int KEY_UP       = 72;
    const int KEY_PGUP     = 73;
    const int KEY_LEFT     = 75;
    const int KEY_CENTER   = 76;
    const int KEY_RIGHT    = 77;
    const int KEY_END      = 79;
    const int KEY_DOWN     = 80;
    const int KEY_PGDN     = 81;
    const int KEY_INSERT   = 82;
    const int KEY_DELETE   = 83;
    const int KEY_F1       = 59;
    const int KEY_F2       = 60;
    const int KEY_F3       = 61;
    const int KEY_F4       = 62;
    const int KEY_F5       = 63;
    const int KEY_F6       = 64;
    const int KEY_F7       = 65;
    const int KEY_F8       = 66;
    const int KEY_F9       = 67;

    // Implemented
    BOSS_API void delay (int millisec); 
    BOSS_API int getch (void); 
    BOSS_API int kbhit(void); 

    // Not Implemented
    BOSS_API void clrscr(void);

} // namespace Conio

/// Bios namespace
namespace Bios {
    
    BOSS_API int bioskey(int command);

} // namespace Bios

/// Audio namespace
namespace Audio {

// some extern variables
// return values for digital sound status function

const int SOUND_STOPPED     = 0;       // no sound is playing
const int SOUND_PLAYING     = 1;       // a sound is playing

// return values for the midi sequence status function
const int SEQUENCE_STOPPED     = 0;    // the current sequence is stopped
const int SEQUENCE_PLAYING     = 1;    // the current sequence is playing
const int SEQUENCE_COMPLETE    = 2;    // the current sequence has completed
const int SEQUENCE_UNAVAILABLE = 0;    // this sequence is unavailable

// these return values are used to determine what happened when a midi file
// has been registered

const int XMIDI_UNREGISTERED  = 0; // the midi file couldn't be registered at all
const int XMIDI_BUFFERED      = 1; // the midi file was registered and buffered
const int XMIDI_UNBUFFERED    = 2; // the midi file was registered, but was too
                                        // big to be buffered, hence, the caller
                                        // needs to keep the midi data resident in
                                        // memory

// T Y P E D E F S //////////////////////////////////////////////////////////

struct _sound {
private:
    Mix_Chunk* pChunk;
    int mixChannelNum;

    friend BOSS_API int Sound_Load(const char *filename, _sound* sound, int translate_useless);
    friend BOSS_API void Sound_Unload(_sound* the_sound);
    friend BOSS_API void Sound_Translate(_sound* the_sound);
    friend BOSS_API int Sound_Play(_sound* the_sound, int repeat, int volume);
    friend BOSS_API int Sound_Status(int channel);
    friend BOSS_API void Sound_Stop(int channel);
public:
    _sound() : pChunk(NULL), mixChannelNum(-1) {}
};

#define _sound_ptr _sound*

// this holds a midi file

struct _music
{
private:
    Mix_Music* pMusic;

    friend BOSS_API int Music_Load(const char *filename, _music* the_music);
    friend BOSS_API void Music_Unload(_music* the_music);
    friend BOSS_API int Music_Register(_music* the_music);
    friend BOSS_API void Music_Stop(void);
    friend BOSS_API int Music_Play(_music* the_music,int sequence, int repeat, int volume);
    friend BOSS_API void Music_Resume(void);
    friend BOSS_API int Music_Status(void);
public:
    _music() : pMusic(NULL) {}
};

#define _music_ptr _music*

BOSS_API int Sound_Load(const char *filename, _sound* the_sound, int translate_useless = 0);
BOSS_API void Sound_Unload(_sound* the_sound);
BOSS_API void Sound_Translate(_sound* the_sound);
BOSS_API int Sound_Play(_sound* the_sound, int repeat = 0, int volume = MIX_MAX_VOLUME);
BOSS_API int Sound_Status(int channel = -1);
BOSS_API void Sound_Stop(int channel = -1);

BOSS_API int Music_Load(const char *filename, _music* the_music);
BOSS_API void Music_Unload(_music* the_music);
BOSS_API int Music_Register(_music* the_music);
BOSS_API void Music_Stop(void);
BOSS_API int Music_Play(_music* the_music,int sequence, int loops = 1, int volume = MIX_MAX_VOLUME);
BOSS_API void Music_Resume(void);
BOSS_API int Music_Status(void);

BOSS_API void nosound();
BOSS_API void sound(int);

} // namespace Audio

/// Random namespace
namespace Random {

BOSS_API void randomize();
BOSS_API int random(int x);

} // namespace Random

/// Mouse namespace
namespace Mouse {

    // Mouse functions via INT 33h
    // From following resources:
    // http://www.nuvisionmiami.com/books/asm/3rd/files/mouse.asm
    // http://www.sentex.net/~ajy/mouseint.html
    // http://www.geocities.com/rk_page/
    //
    // ax = 00h: Initialize Mouse, returns number of buttons
    // ax = 01h: Show Mouse Pointer
    // ax = 02h: Hide Mouse Pointer
    // ax = 03h: Get Mouse Position
    // ax = 04h: Set Mouse Position
    // ax = 20h: Enable Mouse

    struct MouseVars {
    };

    extern MouseVars mVars;

    BOSS_API void InitializeMouse(bool bInit = true);
    BOSS_API void EnableMouseCursor();
    BOSS_API void DisableMouseCursor();
    BOSS_API Uint8 GetMouseState(int* x, int* y);

} // namespace Mouse

} // namespace Boss

using namespace Boss::Graphics::BGI;
using namespace Boss::Graphics::BGI::Not_Implemented;
using namespace Boss::Graphics::Raw;
using namespace Boss::Graphics;
using namespace Boss::Conio;
using namespace Boss::Bios;
using namespace Boss::Audio;
using namespace Boss::Random;
using namespace Boss::Mouse;
using namespace Boss;

#endif // __CD_BOSS_H
