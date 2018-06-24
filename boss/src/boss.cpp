#include "boss.h"
#include <SDL.h>
#include <SDL_TTF.h>
#include <SDL_Mixer.h>

#include <assert.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <algorithm>

#include <map>

#if defined(BOSS_DLL)

BOOL APIENTRY DllMain( void* hModule, 
                       unsigned long  ul_reason_for_call, 
                       void* lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}
#endif // BOSS_DLL

namespace Boss {

const float BOSS_VERSION = 0.45f;

BOSS_API float getVersion()
{
	return BOSS_VERSION;
}

// Constants used by the graphics_sdl module
namespace Graphics {

	int Min(int x, int y) { return (x < y ? x : y); }
	int Max(int x, int y) { return (x > y ? x : y); }

// All Modes array:
grResolution Resolutions[11][6] = 
{
	{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }, // DETECT
	{ {320,200}, {320,200}, {320,200}, {320,200}, {640,200}, {0,0} }, // CGA
	{ {320,200}, {320,200}, {320,200}, {320,200}, {640,200}, {640,480} }, // MCGA
	{ {640,200}, {640,350}, {0,0}, {0,0}, {0,0}, {0,0} }, // EGA
	{ {640,200}, {640,350}, {0,0}, {0,0}, {0,0}, {0,0} }, // EGA64
	{ {0,0}, {0,0}, {0,0}, {640,350}, {0,0}, {0,0} }, // EGAMONO
	{ {640,480}, {1024,768}, {0,0}, {0,0}, {0,0}, {0,0} }, // IBM8514
	{ {720,348}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} }, // HERCMONO
	{ {320,200}, {320,200}, {320,200}, {320,200}, {640,200}, {640,400} }, 
// ATT400
	{ {640,200}, {640,350}, {640,480}, {0,0}, {0,0}, {0,0} }, // VGA
	{ {720,350}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} } // PC3270
};

SDL_Color VgaColors[256] =
{
// EGA Color Table
{0x00, 0x00, 0x00}, {0x00, 0x00, 0xaa}, {0x00, 0xaa, 0x00}, {0x00, 0xaa, 0xaa},
{0xaa, 0x00, 0x00}, {0xaa, 0x00, 0xaa}, {0xaa, 0x55, 0x00}, {0xaa, 0xaa, 0xaa},
{0x55, 0x55, 0x55}, {0x55, 0x55, 0xff}, {0x55, 0xff, 0x55}, {0x55, 0xff, 0xff},
{0xff, 0x55, 0x55}, {0xff, 0x55, 0xff}, {0xff, 0xff, 0x55}, {0xff, 0xff, 0xff},
// Gray Scale Table
{0x00, 0x00, 0x00}, {0x14, 0x14, 0x14}, {0x20, 0x20, 0x20}, {0x2c, 0x2c, 0x2c},
{0x38, 0x38, 0x38}, {0x45, 0x45, 0x45}, {0x51, 0x51, 0x51}, {0x61, 0x61, 0x61},
{0x71, 0x71, 0x71}, {0x82, 0x82, 0x82}, {0x92, 0x92, 0x92}, {0xa2, 0xa2, 0xa2},
{0xb6, 0xb6, 0xb6}, {0xcb, 0xcb, 0xcb}, {0xe3, 0xe3, 0xe3}, {0xff, 0xff, 0xff},
// 24-color Table
{0x00, 0x00, 0xff}, {0x41, 0x00, 0xff}, {0x7d, 0x00, 0xff}, {0xbe, 0x00, 0xff},
{0xff, 0x00, 0xff}, {0xff, 0x00, 0xbe}, {0xff, 0x00, 0x7d}, {0xff, 0x00, 0x41},
{0xff, 0x00, 0x00}, {0xff, 0x41, 0x00}, {0xff, 0x7d, 0x00}, {0xff, 0xbe, 0x00},
{0xff, 0xff, 0x00}, {0xbe, 0xff, 0x00}, {0x7d, 0xff, 0x00}, {0x41, 0xff, 0x00},
{0x00, 0xff, 0x00}, {0x00, 0xff, 0x41}, {0x00, 0xff, 0x7d}, {0x00, 0xff, 0xbe},
{0x00, 0xff, 0xff}, {0x00, 0xbe, 0xff}, {0x00, 0x7d, 0xff}, {0x00, 0x41, 0xff},
{0x7d, 0x7d, 0xff}, {0x9e, 0x7d, 0xff}, {0xbe, 0x7d, 0xff}, {0xdf, 0x7d, 0xff},
{0xff, 0x7d, 0xff}, {0xff, 0x7d, 0xdf}, {0xff, 0x7d, 0xbe}, {0xff, 0x7d, 0x9e},
{0xff, 0x7d, 0x7d}, {0xff, 0x9e, 0x7d}, {0xff, 0xbe, 0x7d}, {0xff, 0xdf, 0x7d},
{0xff, 0xff, 0x7d}, {0xdf, 0xff, 0x7d}, {0xbe, 0xff, 0x7d}, {0x9e, 0xff, 0x7d},
{0x7d, 0xff, 0x7d}, {0x7d, 0xff, 0x9e}, {0x7d, 0xff, 0xbe}, {0x7d, 0xff, 0xdf},
{0x7d, 0xff, 0xff}, {0x7d, 0xdf, 0xff}, {0x7d, 0xbe, 0xff}, {0x7d, 0x9e, 0xff},
{0xb6, 0xb6, 0xff}, {0xc7, 0xb6, 0xff}, {0xdb, 0xb6, 0xff}, {0xeb, 0xb6, 0xff},
{0xff, 0xb6, 0xff}, {0xff, 0xb6, 0xeb}, {0xff, 0xb6, 0xdb}, {0xff, 0xb6, 0xc7},
{0xff, 0xb6, 0xb6}, {0xff, 0xc7, 0xb6}, {0xff, 0xdb, 0xb6}, {0xff, 0xeb, 0xb6},
{0xff, 0xff, 0xb6}, {0xeb, 0xff, 0xb6}, {0xdb, 0xff, 0xb6}, {0xc7, 0xff, 0xb6},
{0xb6, 0xdf, 0xb6}, {0xb6, 0xff, 0xc7}, {0xb6, 0xff, 0xdb}, {0xb6, 0xff, 0xeb},
{0xb6, 0xff, 0xff}, {0xb6, 0xeb, 0xff}, {0xb6, 0xdb, 0xff}, {0xb6, 0xc7, 0xff},
{0x00, 0x00, 0x71}, {0x1c, 0x00, 0x71}, {0x38, 0x00, 0x71}, {0x55, 0x00, 0x71},
{0x71, 0x00, 0x71}, {0x71, 0x00, 0x55}, {0x71, 0x00, 0x38}, {0x71, 0x00, 0x1c},
{0x71, 0x00, 0x00}, {0x71, 0x1c, 0x00}, {0x71, 0x38, 0x00}, {0x71, 0x55, 0x00},
{0x71, 0x71, 0x00}, {0x55, 0x71, 0x00}, {0x38, 0x71, 0x00}, {0x1c, 0x71, 0x00},
{0x00, 0x71, 0x00}, {0x00, 0x71, 0x1c}, {0x00, 0x71, 0x38}, {0x00, 0x71, 0x55},
{0x00, 0x71, 0x71}, {0x00, 0x55, 0x71}, {0x00, 0x38, 0x71}, {0x00, 0x1c, 0x71},
{0x38, 0x38, 0x71}, {0x45, 0x38, 0x71}, {0x55, 0x38, 0x71}, {0x61, 0x38, 0x71},
{0x71, 0x38, 0x71}, {0x71, 0x38, 0x61}, {0x71, 0x38, 0x55}, {0x71, 0x38, 0x45},
{0x71, 0x38, 0x38}, {0x71, 0x45, 0x38}, {0x71, 0x55, 0x38}, {0x71, 0x61, 0x38},
{0x71, 0x71, 0x38}, {0x61, 0x71, 0x38}, {0x55, 0x71, 0x38}, {0x45, 0x71, 0x38},
{0x38, 0x71, 0x38}, {0x38, 0x71, 0x45}, {0x38, 0x71, 0x55}, {0x38, 0x71, 0x61},
{0x38, 0x71, 0x71}, {0x38, 0x61, 0x71}, {0x38, 0x55, 0x71}, {0x38, 0x45, 0x71},
{0x51, 0x51, 0x71}, {0x59, 0x51, 0x71}, {0x61, 0x51, 0x71}, {0x69, 0x51, 0x71},
{0x71, 0x51, 0x71}, {0x71, 0x51, 0x69}, {0x71, 0x51, 0x61}, {0x71, 0x51, 0x59},
{0x71, 0x51, 0x51}, {0x71, 0x59, 0x51}, {0x71, 0x61, 0x51}, {0x71, 0x69, 0x51},
{0x71, 0x71, 0x51}, {0x69, 0x71, 0x51}, {0x61, 0x71, 0x51}, {0x59, 0x71, 0x51},
{0x51, 0x71, 0x51}, {0x51, 0x71, 0x59}, {0x51, 0x71, 0x61}, {0x51, 0x71, 0x69},
{0x51, 0x71, 0x71}, {0x51, 0x69, 0x71}, {0x51, 0x61, 0x71}, {0x51, 0x59, 0x71},
{0x00, 0x00, 0x41}, {0x10, 0x00, 0x41}, {0x20, 0x00, 0x41}, {0x30, 0x00, 0x41},
{0x41, 0x00, 0x41}, {0x41, 0x00, 0x30}, {0x41, 0x00, 0x20}, {0x41, 0x00, 0x10},
{0x41, 0x00, 0x00}, {0x41, 0x10, 0x00}, {0x41, 0x20, 0x00}, {0x41, 0x30, 0x00},
{0x41, 0x41, 0x00}, {0x30, 0x41, 0x00}, {0x20, 0x41, 0x00}, {0x10, 0x41, 0x00},
{0x00, 0x41, 0x00}, {0x00, 0x41, 0x10}, {0x00, 0x41, 0x20}, {0x00, 0x41, 0x30},
{0x00, 0x41, 0x41}, {0x00, 0x30, 0x41}, {0x00, 0x20, 0x41}, {0x00, 0x10, 0x41},
{0x20, 0x20, 0x41}, {0x28, 0x20, 0x41}, {0x30, 0x20, 0x41}, {0x38, 0x20, 0x41},
{0x41, 0x20, 0x41}, {0x41, 0x20, 0x38}, {0x41, 0x20, 0x30}, {0x41, 0x20, 0x28},
{0x41, 0x20, 0x20}, {0x41, 0x28, 0x20}, {0x41, 0x30, 0x20}, {0x41, 0x38, 0x20},
{0x41, 0x41, 0x20}, {0x38, 0x41, 0x20}, {0x30, 0x41, 0x20}, {0x28, 0x41, 0x20},
{0x20, 0x41, 0x20}, {0x20, 0x41, 0x28}, {0x20, 0x41, 0x30}, {0x20, 0x41, 0x38},
{0x20, 0x41, 0x41}, {0x20, 0x38, 0x41}, {0x20, 0x30, 0x41}, {0x20, 0x28, 0x41},
{0x2c, 0x2c, 0x41}, {0x30, 0x2c, 0x41}, {0x34, 0x2c, 0x41}, {0x3c, 0x2c, 0x41},
{0x41, 0x2c, 0x41}, {0x41, 0x2c, 0x3c}, {0x41, 0x2c, 0x34}, {0x41, 0x2c, 0x30},
{0x41, 0x2c, 0x2c}, {0x41, 0x30, 0x2c}, {0x41, 0x34, 0x2c}, {0x41, 0x3c, 0x2c},
{0x41, 0x41, 0x2c}, {0x3c, 0x41, 0x2c}, {0x34, 0x41, 0x2c}, {0x30, 0x41, 0x2c},
{0x2c, 0x41, 0x2c}, {0x2c, 0x41, 0x30}, {0x2c, 0x41, 0x34}, {0x2c, 0x41, 0x3c},
{0x2c, 0x41, 0x41}, {0x2c, 0x3c, 0x41}, {0x2c, 0x34, 0x41}, {0x2c, 0x30, 0x41},
{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00},
{0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}, {0x00, 0x00, 0x00}
};

Uint32 VgaColorsInUint32[256];
std::map<Uint32, Uint8> Uint32ToVgaColor;

/// Returns EGA/CVGA color that the unsigned 32-bit number maps to.  
///
/// \return EGA/CVGA color that the unsigned 32-bit number maps to or 
/// -1 if the color does not exactly map to the standard VGA palette.
///
/// NOTE: initgraph() or RawInitColors() must be called prior to 
/// function.
BOSS_API int getVgaColor(Uint32 col32)
{
	std::map<Uint32, Uint8>::iterator it = Uint32ToVgaColor.find(col32);
	if(it == Uint32ToVgaColor.end()) {
		return -1;
	}

	return it->second;
}

// Font filenames
const char* szFontFilenames[MAX_FONTS] = {
					"data/fonts/arial.ttf", // DEFAULT_FONT
					"data/fonts/arial.ttf", // TRIPLEX_FONT
					"data/fonts/times.ttf", // SMALL_FONT
					"data/fonts/micross.ttf", // SANS_SERIF_FONT
					"data/fonts/wilhelmwd.ttf", // GOTHIC_FONT
					"data/fonts/script.fon", // SCRIPT_FONT
					"data/fonts/arial.ttf", // SIMPLEX_FONT
					"data/fonts/arial.ttf", // TRIPLEX_SCR_FONT
					"data/fonts/arial.ttf", // COMPLEX_FONT
					"data/fonts/arial.ttf", // EUROPEAN_FONT
					"data/fonts/arial.ttf" // BOLD_FONT
					};

// font sizing information here...maps charsize to pt size for each font
// TO DO:  Fix Gothic as it appears a bit too tall 
// (see main game page for graphics scrolling off page)
int FontPtSize[MAX_FONTS][10] = {
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 72}, // DEFAULT_FONT
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 72}, // TRIPLEX_FONT
	{ 0, 2, 5, 8, 11, 14, 17, 20, 23, 26}, // SMALL_FONT
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 72}, // SANS_SERIF_FONT
	{ 0, 18, 21, 24, 34, 44, 54, 64, 74, 84}, // GOTHIC_FONT
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 72}, // SCRIPT_FONT
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 72}, // SIMPLEX_FONT
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 72}, // TRIPLEX_SCR_FONT
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 72}, // COMPLEX_FONT
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 72}, // EUROPEAN_FONT
	{ 0, 8, 16, 24, 32, 40, 48, 56, 64, 72} // BOLD_FONT
	};

// 8x8 array of 1's and 0's, 1's get fill color, 0's get background colour
// mod the pixel x and y by 8 to get the index into the pattern array
// TO DO:  Set the rest of the fill...need examples...
char FillPatterns[MAX_FILL_STYLES][8][8] = {
		// EMPTY_FILL
		{ 
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0}
		},
		// SOLID_FILL
		{ 
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		}, 
		// LINE_FILL
		{ 
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		}, 
		// LTSLASH_FILL
		{ 
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		}, 
		// SLASH_FILL
		{ 
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		}, 
		// BKSLASH_FILL
		{ 
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		}, 
		// LTBKSLASH_FILL
		{ 
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		}, 
		// HASH_FILL
		{ 
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		}, 
		// XHATCH_FILL
		{ 
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		}, 
		// INTERLEAVE_FILL
		{ 
			{0, 0, 1, 1, 0, 0, 1, 1},
			{1, 1, 0, 0, 1, 1, 0, 0},
			{0, 0, 1, 1, 0, 0, 1, 1},
			{1, 1, 0, 0, 1, 1, 0, 0},
			{0, 0, 1, 1, 0, 0, 1, 1},
			{1, 1, 0, 0, 1, 1, 0, 0},
			{0, 0, 1, 1, 0, 0, 1, 1},
			{1, 1, 0, 0, 1, 1, 0, 0}
		}, 
		// WIDE_DOT_FILL
		{ 
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		}, 
		// CLOSE_DOT_FILL
		{ 
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		}, 
		// USER_FILL, settable via setfillpattern()
		{ 
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1},
			{1, 1, 1, 1, 1, 1, 1, 1}
		}
	};

GraphVars::GraphVars() :
	lastErr(grOk),
	theScreen(NULL),
	fgColor(0),
	bgColor(0),
	fillPattern(SOLID_FILL),
	fillColor(0),
	penX(0),
	penY(0),
	bTTFInitialized(false),
	curFont(0),
	fontDirection(HORIZ_DIR),
	justH(LEFT_TEXT),
	justV(TOP_TEXT),
	curCharSize(1)
{
}

GraphVars grVars;

// SDL function prototypes

namespace Raw {

/// Initializes the VGA color palette for the given surface.
BOSS_API void RawInitColors(SDL_Surface* pSurf)
{
	assert(pSurf && "RawInitColors()");
	// set up the Uint32 color table
	for(int color = 0; color < 256; ++color) {
		VgaColorsInUint32[color] = SDL_MapRGB(pSurf->format, 
			VgaColors[color].r, VgaColors[color].g, VgaColors[color].b);
	}
}

/// Locks the SDL Surface
///
/// \param screen An initialized SDL_Surface that is currently unlocked.
BOSS_API void Slock(SDL_Surface* screen)
{
  if ( SDL_MUSTLOCK(screen) )
  {
    if ( SDL_LockSurface(screen) < 0 )
    {
      return;
    }
  }
}

/// Unlocks the SDL Surface
///
/// \param screen An initialized SDL_Surface that is currently locked.
BOSS_API void Sulock(SDL_Surface* screen)
{
  if ( SDL_MUSTLOCK(screen) )
  {
    SDL_UnlockSurface(screen);
  }
} // Sulock

/// Sets the pixel at x,y to color.  The SDL_Surface screen must be locked.
///
/// \param screen An initialized SDL_Surface that is currently locked.
/// \param x The x-coordinate of the pixel.  0 <= x <= (screen.w-1)
/// \param y The y-coordinate of the pixel.  0 <= y <= (screen.h-1)
/// \param color The color to set the pixel.  The color must be in the 
/// pixel format of the screen surface.
///
BOSS_API void RawDrawPixel(SDL_Surface* screen, int x, int y, Uint32 
color)
{
	assert(screen);
	assert(x >= 0);
	assert(y >= 0);
	assert(x < screen->w);
	assert(y < screen->h);
	if(x < 0 || x >= screen->w || y < 0 || y >= screen->h) {
		return;
	}

	switch (screen->format->BytesPerPixel)
	{
	case 1: // Assuming 8-bpp
		{
		Uint8 *bufp;
		bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
		*bufp = color;
		}
		break;
	case 2: // Probably 15-bpp or 16-bpp
		{
		Uint16 *bufp;
		bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
		*bufp = color;
		}
		break;
	case 3: // Slow 24-bpp mode, usually not used
		{
		Uint8 *bufp;
		bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
		if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
		{
			bufp[0] = color;
			bufp[1] = color >> 8;
			bufp[2] = color >> 16;
		} else {
			bufp[2] = color;
			bufp[1] = color >> 8;
			bufp[0] = color >> 16;
		}
		}
		break;
	case 4: // Probably 32-bpp
		{
		Uint32 *bufp;
		bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
		*bufp = color;
		}
		break;
	} // switch (screen->format->BytesPerPixel)
} // RawDrawPixel

/// Returns the color of the pixel at x,y.  The SDL_Surface screen
/// must be locked.
///
/// \param screen An initialized SDL_Surface that is currently locked.
/// \param x The x-coordinate of the pixel.  0 <= x <= (screen.w-1)
/// \param y The y-coordinate of the pixel.  0 <= y <= (screen.h-1)
///
BOSS_API Uint32 RawGetPixel(SDL_Surface* screen, int x, int y)
{
	assert(screen);
	assert(x >= 0);
	assert(y >= 0);
	assert(x < screen->w);
	assert(y < screen->h);

	if(x < 0 || x >= screen->w || y < 0 || y >= screen->h) {
		return 0;
	}

	Uint32 pixel = 0;

	switch (screen->format->BytesPerPixel)
	{
	case 1: // Assuming 8-bpp
		{
			Uint8* bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
			pixel = *bufp;
		}
		break;
	case 2: // Probably 15-bpp or 16-bpp
		{
			Uint16* bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
			pixel = *bufp;
		}
		break;
	case 3: // Slow 24-bpp mode, usually not used
		{
			Uint8* bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN) {
				pixel = ((bufp[2] << 16) | (bufp[1] << 8) | bufp[0]);
			} 
			else {
				pixel = ((bufp[0] << 16) | (bufp[1] << 8) | bufp[2]);
			}
		}
		break;
	case 4: // Probably 32-bpp
		{
			Uint32* bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
			pixel = *bufp;
		}
		break;
	} // switch (screen->format->BytesPerPixel)

  return pixel;
} // RawGetPixel

/// Draws a horizontal line of the given width with x,y as one of the 
/// end points in the given color with the given fill pattern.
///
/// \param screen An initialized SDL_Surface that is currently locked.
/// \param x An edge of the horizontal line.  0 <= x <= (screen.w-1).  
/// See width parameter.
/// \param y The y-coordinate of the horizontal line.  0 <= y <= (scrren.h-1)
/// \param width The width of the line.  If this value is positive, 
/// then x is the left edge of
/// the horizontal line.  If this value is negative, then x is the 
/// right edge of the horizontal
/// line.
/// \param color The color of the line to draw.  Must be in the pixel 
/// format of the screen surface.
/// \param fillPattern The fill pattern of the horizontal line.  This 
/// value must be one of the
/// BGI::BGIFillPatterns enum values.  The default is SOLID_FILL.
BOSS_API void RawDrawHLine(SDL_Surface* screen, int x, int y, int width, Uint32 color, int fillPattern)
{
	if(width < 0) {
		x += (width+1);
		width = -width;
	}
	else if(width == 0) { // if width == 0 then we're done...
		return;
	}

	const int maxx = x+width-1;

  switch (screen->format->BytesPerPixel)
  {
    case 1: // Assuming 8-bpp
      {
        Uint8 *bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
		while(x <= maxx) {
			if(FillPatterns[fillPattern][y%8][x%8]) {
				*bufp = color;
			}
			else {
				*bufp = grVars.bgColor;
			}
			++bufp;
			++x;
		}
      }
      break;
    case 2: // Probably 15-bpp or 16-bpp
      {
        Uint16 *bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
		while(x <= maxx) {
			if(FillPatterns[fillPattern][y%8][x%8]) {
				*bufp = color;
			}
			else {
				*bufp = grVars.bgColor;
			}
			++bufp;
			++x;
		}
      }
      break;
    case 3: // Slow 24-bpp mode, usually not used
      {
        Uint8 *bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
		while(x <= maxx) {
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
			{
				if(FillPatterns[fillPattern][y%8][x%8]) {
					bufp[0] = color;
					bufp[1] = color >> 8;
					bufp[2] = color >> 16;
				}
				else {
					bufp[0] = grVars.bgColor;
					bufp[1] = grVars.bgColor >> 8;
					bufp[2] = grVars.bgColor >> 16;
				}
			} 
			else {
				if(FillPatterns[fillPattern][y%8][(x+width)%8]) {
					bufp[2] = color;
					bufp[1] = color >> 8;
					bufp[0] = color >> 16;
				}
				else {
					bufp[2] = grVars.bgColor;
					bufp[1] = grVars.bgColor >> 8;
					bufp[0] = grVars.bgColor >> 16;
				}
			}
			bufp += 3;
			++x;
		}
      }
      break;
    case 4: // Probably 32-bpp
      {
        Uint32 *bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
		while(x <= maxx) {
			if(FillPatterns[fillPattern][y%8][x%8]) {
				*bufp = color;
			}
			else {
				*bufp = grVars.bgColor;
			}
			++bufp;
			++x;
		}
      }
      break;
  }
} // RawDrawHLine

/// Draws a vertical line of the given height with x,y as one of the endpoints in the given 
/// color with the given fill pattern.
///
/// \param screen An initialized SDL_Surface that is currently locked.
/// \param x The x-coordinate of the vertical line.  0 <= x <= (screen.w-1)
/// \param y An edge of the the vertical line.  0 <= y <= (screen.h-1).  See height parameter.
/// \param height The height of the line.  If this value is positive, then y is the top edge of
/// the vertical line.  If this value is negative, then y is the bottom edge of the vertical line.
/// \param color The color of the line to draw.  Must be in the pixel format of the screen surface.
/// \param fillPattern The fill pattern of the vertical line.  This value must be one of the
/// BGI::BGIFillPatterns enum values.  The default is SOLID_FILL.
BOSS_API void RawDrawVLine(SDL_Surface* screen, int x, int y, int height, Uint32 color, int fillPattern)
{
	int mod = (height > 0 ? +1 : -1);
	if(height < 0) { height *= -1; }

  switch (screen->format->BytesPerPixel)
  {
    case 1: // Assuming 8-bpp
      {
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
		while(height > 0) {
			*bufp = color;
			bufp += mod*screen->pitch;
			--height;
		}
      }
      break;
    case 2: // Probably 15-bpp or 16-bpp
      {
        Uint16 *bufp;
        bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
		while(height > 0) {
			*bufp = color;
			bufp += mod*screen->pitch/2;
			--height;
		}
      }
      break;
    case 3: // Slow 24-bpp mode, usually not used
      {
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
		while(height > 0) {
			if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
			{
				bufp[0] = color;
				bufp[1] = color >> 8;
				bufp[2] = color >> 16;
			} 
			else {
				bufp[2] = color;
				bufp[1] = color >> 8;
				bufp[0] = color >> 16;
			}
			bufp += mod*screen->pitch;
			--height;
		}
      }
      break;
    case 4: // Probably 32-bpp
      {
        Uint32 *bufp;
        bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
		while(height > 0) {
			*bufp = color;
			bufp += mod*screen->pitch/4;
			--height;
		}
      }
      break;
  }
} // RawDrawVLine

/// Draws a line from x1,y1 to x2,y2 of the given color.
///
/// \param screen An initialized SDL_Surface that is currently locked.
/// \param x1 The x-coordinate of the first endpoint.  0 <= x <= (screen.w-1).
/// \param y1 The y-coordinate of the first endpoint.  0 <= y <= (screen.h-1)
/// \param x2 The x-coordinate of the second endpoint.  0 <= x <= (screen.w-1).
/// \param y2 The y-coordinate of the second endpoint.  0 <= y <= (screen.h-1)
/// \param color The color of the line to draw.  Must be in the pixel format of the screen surface.
BOSS_API void RawDrawLine(SDL_Surface* screen, int x1, int y1, int x2, int y2, Uint32 color)
{
//	printf("line(%d, %d, %d, %d)\n", x1, y1, x2, y2);
	//int x = (x1 < x2 ? x1 : x2);
	//int y = (y1 < y2 ? y1 : y2);
	//int w = abs(x1-x2) + 1;
	//int h = abs(y1-y2) + 1;

	// horizontal line
	if(y1 == y2) {
		//int curx = (x1 < x2 ? x1 : x2);
		//const int maxx = (x1 < x2 ? x2 : x1);

		RawDrawHLine(grVars.theScreen, x1, y1, (x2-x1), color, SOLID_FILL);
	}
	// vertical line
	else if(x1 == x2) {
		//int cury = (y1 < y2 ? y1 : y2);
		//const int maxy = (y1 < y2 ? y2 : y1);

		RawDrawVLine(grVars.theScreen, x1, y1, (y2-y1), color, SOLID_FILL);
	}
	else {
		const double slope = (double)(y2-y1)/(double)(x2-x1);
//		printf("slope = %f\n", slope);

		double curx = (double)(x1);// < x2 ? x1 : x2);
		double cury = (double)(y1);// < y2 ? y1 : y2);
		const double maxx = (double)(x2);//? x2 : x1);
		const double maxy = (double)(y2);// ? y2 : y1);
		double xfact = 0.0;
		double yfact = 0.0;

		// delta y > delta x; move in y direction
		if(slope > 1.0 || slope < -1.0) {
			yfact = (maxy > cury ? 1.0 : -1.0);
			xfact = (maxx > curx ? 1.0/slope : -1.0/slope);
			xfact = (maxx > curx ? (slope > 0.0 ? 1.0/slope : -1.0/slope) : 
									(slope > 0.0 ? -1.0/slope : 1.0/slope) );
		}
		else { // delta x > delta y; move in x direction
			xfact = (maxx > curx ? 1.0 : -1.0);
			yfact = (maxy > cury ? (slope > 0.0 ? slope : -slope) : (slope > 0.0 ? -slope : slope) );
		}
//		printf("xfact = %f, yfact = %f\n", xfact, yfact);

		// crappy line rasterization...
		bool bDone = false;
		while( !bDone ) {
//			printf("curx = %f, cury = %f, maxx = %f, maxy = %f\n", curx, cury, maxx, maxy);

			RawDrawPixel(grVars.theScreen, (int)curx, (int)cury, color);
			
			curx += xfact;
			cury += yfact;

			// both positive
			if(maxx > x1) {
				if(maxy > y1) {
					bDone = (curx > maxx && cury > maxy);
				}
				else {
					bDone = (curx > maxx && cury < maxy);
				}
			}
			else {
				if(maxy > y1) {
					bDone = (curx < maxx && cury > maxy);
				}
				else {
					bDone = (curx < maxx && cury < maxy);
				}
			}
		}
	}
} // RawDrawLine

// define a triangle-drawing routine that:
//	locks the surface
//	draws each horizontal raster line using RawDrawHLine
//  unlocks the surface
//
// This is my first triangle rasterization routine, can probably be optimized!
BOSS_API void RawFillTriangle(SDL_Surface* screen, grPoint* pts, Uint32 color, int fillPattern)
{
	// find a starting point
	// find minimum Y
	int curY = (pts[0].y < pts[1].y ? (pts[0].y < pts[2].y ? pts[0].y : pts[2].y) : 
					(pts[1].y < pts[2].y ? pts[1].y : pts[2].y));
	const int maxY = (pts[0].y > pts[1].y ? (pts[0].y > pts[2].y ? pts[0].y : pts[2].y) : 
					(pts[1].y > pts[2].y ? pts[1].y : pts[2].y));

//	printf("RawFillTriangle, curY = %d, maxY = %d\n", curY, maxY);

	int minX = 0;
	int maxX = 0;
	float interceptX= 0;
	bool bInterceptFound = false;
	bool bSortedByY = false;

	// here, we set up an array of 3 line segments, such that the first point
	// has the minimum of the two x-coords
	grPoint* sortedPt[3][2];
	// first line segment is pts[0] and pts[1]
	if(pts[0].x < pts[1].x) {
		sortedPt[0][0] = &pts[0];
		sortedPt[0][1] = &pts[1];
	}
	else {
		sortedPt[0][0] = &pts[1];
		sortedPt[0][1] = &pts[0];
	}
	// second line segment is pts[1] and pts[2]
	if(pts[1].x < pts[2].x) {
		sortedPt[1][0] = &pts[1];
		sortedPt[1][1] = &pts[2];
	}
	else {
		sortedPt[1][0] = &pts[2];
		sortedPt[1][1] = &pts[1];
	}
	// third line segment is pts[2] and pts[0]
	if(pts[2].x < pts[0].x) {
		sortedPt[2][0] = &pts[2];
		sortedPt[2][1] = &pts[0];
	}
	else {
		sortedPt[2][0] = &pts[0];
		sortedPt[2][1] = &pts[2];
	}

	// since this is a raw function, no need to lock/unlock
//	assert(grVars.theScreen);
//	Slock(grVars.theScreen);

	// we start at the top Y
	// we have 3 line segments (pts[0]-pts[1], pts[1]-pts[2], pts[2]-pts[0])
	// from minY to maxY there will be at least 1 intercept (and usually two)
	while(curY <= maxY) {

//		printf("CurY = %d\n", curY);

		// set our min/max to starting values
		minX = getmaxx();
		maxX = 0;

		// find the intersection points of the three lines for the current y
		// the first point always has the lower of the two x values
		for(int lineSegment = 0; lineSegment < 3; ++lineSegment) {
//			printf("  line segment #%d\n", lineSegment);

			bInterceptFound = false;

			grPoint* pt0 = sortedPt[lineSegment][0];
			grPoint* pt1 = sortedPt[lineSegment][1];

			// if curY is in this line segment:
			if(pt0->y < pt1->y) {
				bInterceptFound = (curY >= pt0->y && curY <= pt1->y);
				bSortedByY = true;
			}
			else {
				bInterceptFound = (curY >= pt1->y && curY <= pt0->y);
				bSortedByY = false;
			}
//			printf("  Intercept found\n");

			if(bInterceptFound) {
				// this is a horizontal line
				if(pt0->y == pt1->y) {
					minX = Min(minX, pt0->x);
					maxX = Max(maxX, pt1->x);
				}
				// this is a vertical line
				else if(pt0->x == pt1->x) {
					minX = Min(minX, pt0->x);
					maxX = Max(maxX, pt1->x);
				}
				else { // we must interpolate
//					printf("  Interpolating\n");

					float den = ((float)pt1->y - (float)pt0->y);
					interceptX = (float)pt0->x * ((float)pt1->y - (float)curY)/den + 
									(float)pt1->x * ((float)curY - (float)pt0->y)/den;

//					printf("    Intercept = %f\n", interceptX);

					minX = Min(minX, (int)interceptX);
					maxX = Max(maxX, (int)interceptX);
				}
	
			}
		} // for(int lineSegment = 0; lineSegment < 2; ++lineSegment) 

//		printf("minX = %d, maxX = %d\n", minX, maxX);

		// draw the horizontal line
		if(minX <= maxX) {
			RawDrawHLine(grVars.theScreen, minX, curY, (maxX-minX+1), color, fillPattern);
		}

		// go to next scan line
		++curY;
	} // while(minY <= maxY) 

	// since this is a raw function, no need to lock/unlock
//	Sulock(grVars.theScreen);
} // RawFillTriangle

// NOTE:  This algorithm stolen directly from Realtime Rendering...
bool RawPointInPolygon(int tx, int ty, int numpoints, int* polypoints)
{
	bool bInside = false;

	grPoint e0;
	grPoint e1;

	e0.x = polypoints[ (numpoints-1)*2 ];
	e0.y = polypoints[ (numpoints-1)*2 + 1 ];

	e1.x = polypoints[0];
	e1.y = polypoints[1];

	bool y0 = (e0.y >= ty);
	for(int i = 1; i <= numpoints-1; ++i) {
		bool y1 = (e1.y >= ty);

		if(y0 != y1) {
			if( ((e1.y - ty)*(e0.x - e1.x) >= (e1.x - tx)*(e0.y - e1.y)) == y1 ) {
				bInside = !bInside;
			}
		} // if(y0 != y1)

		y0 = y1;

		e0.x = e1.x;
		e0.y = e1.y;

		e1.x = polypoints[ (i*2) ];
		e1.y = polypoints[ (i*2) + 1 ];

	} // for(int i
	
	return bInside;
} // RawPointInPolygon

} // namespace SDL

using namespace Boss::Graphics::Raw;

namespace BGI {

namespace Enhanced {

BOSS_API void filltriangle(grPoint* pts)
{
	RawFillTriangle(grVars.theScreen, pts, grVars.fillColor, 
grVars.fillPattern);
}

} // namespace Enhanced

// TO DO:  optimize the heck out of this function, maybe gut it and
// place all 4 bit calculations in the main putimage loop...
int Internal_getBit(int x, int y, int width, int plane, unsigned char* buf) { 
	assert(plane >= 0);
	assert(plane < 4);
	assert(width >= 0);
	assert(buf);

	int theBit = 0; 
	const int bytes_per_plane = (width-1)/8 + 1; 
	const int planes_per_row = 4; 
	const int bytes_per_row = bytes_per_plane * planes_per_row; 

	// position our buffer to the correct row 
	buf += (bytes_per_row * y); 

	// position our buffer to the correct plane 
	buf += (bytes_per_plane * plane); 

	// position our buffer to the proper byte 
	buf += (x/8); 

	// now get the right bit 
	unsigned char theByte = *(buf); 
	int bitNumber = (x%8); 
   
	// if it's bit 0, we need to shift right 7 bits 
	// if it's bit 1, we need to shift right 6 bits 
	// ... 
	// if it's bit 7, we need to shift right 0 bits 
	theBit = (theByte >> (7-bitNumber)) & 0x01; 

	return theBit; 
} // Internal_getbit

int FloatToInt(float f)
{
	int i = (int)f;
	float fdiff = f - (float)i;
	if(fdiff > 0.5f) {
		++i;
	}
	return i;
}

int DoubleToInt(double d)
{
	int i = (int)d;
	double ddiff = d - (double)i;
	if(ddiff > 0.5) {
		++i;
	}
	return i;
}

// Implemented graphics.h Functions

/// arc draws a circular arc in the current drawing color centered at (x,y) with a radius 
/// given by radius. 
///
/// The arc travels from stangle to endangle. If stangle equals 0 and endangle equals 360, 
/// the call to arc draws a complete circle.
///
/// \param stangle Start angle is reckoned counterclockwise, with 0 degrees at 3 o'clock, 90 degrees at 12 o'clock, 
/// and so on. 
/// <br>
/// \param endangle End angle is reckoned identically to the start angle
///
/// NOTE:  The linestyle parameter does not affect arcs, circles, ellipses, or pie slices. 
/// Only the thickness parameter is used. 
BOSS_API void arc(int x, int y, int stangle, int endangle, int radius)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	if(radius < 0) { 
		grVars.lastErr = grError;
		return;
	}

	// put angles into the range 0 to 360
	while(stangle < 0) { stangle += 360; }
	while(endangle < 0) { endangle += 360; }
	while(stangle > 360) { stangle -= 360; }
	while(endangle > 360) { endangle -= 360; }

	// now, if stangle is greater than endangle, let's increase endangle
	if(stangle > endangle) { endangle += 360; }

	// stangle and endangle in degrees, rendered counter-clockwise
	float start = (float)stangle/360.0f * (2.0f*PI);
	const float end = (float)endangle/360.0f * (2.0f*PI);
	const float frad = (float)radius;
	const float anginc = 1.0f/frad;

	float plotx = 0.0f;
	float ploty = 0.0f;

	Slock(grVars.theScreen);
	while(start <= end) {
		plotx = (float)x + cosf(start)*frad;
		ploty = (float)y - sinf(start)*frad;

		RawDrawPixel(grVars.theScreen, FloatToInt(plotx), FloatToInt(ploty), grVars.fgColor);

		// increment it a fraction of the radius
		start += anginc;
	} // while(start <= end)

	Sulock(grVars.theScreen);

	SDL_UpdateRect(grVars.theScreen, x - radius, y - radius, radius*2+1, radius*2+1);

	grVars.lastErr = grOk;
} // arc

/// bar draws a filled-in, rectangular, two-dimensional bar. 
///
/// The bar is filled using the current fill pattern and fill color. bar does not outline the bar; 
/// to draw an outlined two-dimensional bar, use bar3d with depth equal to 0. 
///
/// \param left Refers to the left edge of pixels in the rectangle 
/// \param top Refers to the top edge of pixels in the rectangle 
/// \param right Refers to the right edge of pixels in the rectangle 
/// \param bottom Refers to the bottom edge of pixels in the rectangle 
BOSS_API void bar(int left, int top, int right, int bottom)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	Uint32 fillColor = grVars.fillColor;

	if(left > right) { 
		int temp = left;
		left = right;
		right = temp;
	}

	if(top > bottom) {
		int temp = top;
		top = bottom;
		bottom = temp;
	}

	SDL_Rect rc;
	rc.x = left;
	rc.y = top;
	rc.w = (right - left + 1);
	rc.h = (bottom - top + 1);

	// if it's a solid fill, we can use SDL_FillRect
	if(grVars.fillPattern == SOLID_FILL) {
		if(SDL_FillRect(grVars.theScreen, &rc, fillColor) == -1) {
			fprintf(stderr, "FillRect failed in cleardevice(), error='%s'\n", SDL_GetError());
			grVars.lastErr = grError;
			return;
		}
	}
	else { // else, we need to use RawDrawHLine

		Slock(grVars.theScreen);

		for(int y = top; y <= bottom; ++y) {
			RawDrawHLine(grVars.theScreen, left, y, (right - left + 1),
					fillColor, grVars.fillPattern);
		}

		Sulock(grVars.theScreen);
	}

	SDL_UpdateRect(grVars.theScreen, rc.x, rc.y, rc.w, rc.h);
	grVars.lastErr = grOk;
} // bar

/// circle draws a circle in the current drawing color with its center at (x,y) 
/// and the radius given by radius. 
///
/// The linestyle parameter does not affect arcs, circles, ellipses, or pie slices. 
/// Only the thickness parameter is used. 
///
/// \param x The x coordinate of the center of the circle in terms of pixels.
/// \param y The y coordinate of the center of the circle in terms of pixels.
/// \param radius The radius of the circle in pixels.
BOSS_API void circle(int x, int y, int radius)
{
	arc(x, y, 0, 360, radius);
} // circle

/// cleardevice erases (that is, fills with the current background color) the entire graphics 
/// surface and moves the CP (current position) to home (0,0). 
BOSS_API void cleardevice(void)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	if( SDL_FillRect(grVars.theScreen, NULL, grVars.bgColor) == -1) {
		fprintf(stderr, "FillRect failed in cleardevice(), error='%s'\n", SDL_GetError());
		grVars.lastErr = grError;
		return;
	}

	grVars.penX = 0;
	grVars.penY = 0;

	SDL_UpdateRect(grVars.theScreen, 0, 0, 0, 0);
	grVars.lastErr = grOk;
} // cleardevice

// TO DO:  Implement a "viewport", track its size and Current Position, this should
// only clear the viewport...clipping must be done, etc...
BOSS_API void clearviewport(void)
{
	cleardevice();
} // clearviewport

/// closegraph deallocates all memory allocated by the graphics system, 
/// then restores the screen to the mode it was in before you called initgraph. 
///
/// BOSS-specific:  A call to closegraph shuts down all Graphics subsystems within
/// Boss (SDL and SDL_TTF).
BOSS_API void closegraph(void)
{
	// close all the open fonts
	for(int font = 0; font < MAX_FONTS; ++font) {
		if(grVars.fonts[font].pFont) {
			TTF_CloseFont(grVars.fonts[font].pFont);
			grVars.fonts[font].pFont = NULL;
		}
	}

	TTF_Quit();
	SDL_Quit();

	grVars.lastErr = grOk;
} // closegraph

/// drawpoly draws a polygon with numpoints points, using the current line style and color. 
///
/// \param numpoints Refers to the number of points in the polygon.
/// \param polypoints The polypoints pointer points to a sequence of (numpoints * 2) integers.  
/// Each pair of integers gives the x- and y-coordinates of a point on the polygon. 
///
/// NOTE:  In order to draw a closed figure with n vertices, you must pass n + 1 coordinates 
/// to drawpoly where the nth coordinate is equal to the 0th. 
///
/// BOSS-specific:  Line style has not been implemented yet.
BOSS_API void drawpoly(int numpoints, int *polypoints)
{
//	fprintf(stderr, "drawpoly Function not implemented yet\n");
	if(!polypoints || numpoints < 1) {
		grVars.lastErr = grError;
		return;
	}

	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	Slock(grVars.theScreen);

	// if it's one point, then just draw a single pixel
	if(numpoints == 1) {
		RawDrawPixel(grVars.theScreen, polypoints[0], polypoints[1], grVars.fgColor);
	}
	else {

		for(int pt = 0; pt < numpoints - 1; ++pt) {

			RawDrawLine(grVars.theScreen, polypoints[pt*2], polypoints[pt*2+1], 
						polypoints[(pt+1)*2], polypoints[(pt+1)*2+1], grVars.fgColor);
		}
	}

	Sulock(grVars.theScreen);

	grVars.lastErr = grOk;

} // drawpoly

/// ellipse draws an elliptical arc in the current drawing color with its center at (x,y)
/// and the horizontal and vertical axes given by xradius and yradius, respectively. 
///
/// The ellipse travels from stangle to endangle.  If stangle equals 0 and endangle equals 360, 
/// the call to ellipse draws a complete ellipse. 
///
/// \param x The x-coordinate of the center of the ellipse in terms of pixels.
/// \param y The y-coordinate of the center of the ellipse in terms of pixels.
/// \param stangle The starting angle, reckoned counterclockwise, with 0 degrees at 
/// 3 o'clock, 90 degrees at 12 o'clock, and so on. 
/// \param endangle The ending angle, reckoned counterclockwise, with 0 degrees at 
/// 3 o'clock, 90 degrees at 12 o'clock, and so on. 
/// \param xradius The x-radius of the ellipse in terms of pixels.
/// \param yradius The y-radius of the ellipse in terms of pixels.
///
/// NOTE:  The linestyle parameter does not affect arcs, circles, ellipses, or pie slices. 
/// Only the thickness parameter is used. 
BOSS_API void ellipse(int x, int y, int stangle, int endangle, int xradius, int yradius)
{
	if(xradius < 0 || yradius < 0) {
		grVars.lastErr = grError;
		return;
	}
	
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	// stangle and endangle in degrees, rendered counter-clockwise
	float start = (float)stangle/360.0f * (2.0f*PI);
	const float end = (float)endangle/360.0f * (2.0f*PI);

	const float fxrad = (float)xradius;
	const float fyrad = (float)yradius;
	const float anginc = (xradius < yradius ? 1.0f/(float)(fyrad) : 1.0f/(float)(fxrad));

	float plotx = 0.0f;
	float ploty = 0.0f;

	assert(grVars.theScreen);
	Slock(grVars.theScreen);
	while(start <= end) {
		plotx = (float)x + cosf(start)*fxrad;
		ploty = (float)y + sinf(start)*fyrad;

		RawDrawPixel(grVars.theScreen, FloatToInt(plotx), FloatToInt(ploty), grVars.fgColor);

		// increment it a fraction of the radius
		start += anginc;
	} // while(start <= end)

	Sulock(grVars.theScreen);

	// TO DO:  maybe change this update to only update the rect where the arc was
	SDL_UpdateRect(grVars.theScreen, x-xradius, y-yradius, xradius*2+1, yradius*2+1);

	grVars.lastErr = grOk;
} // ellipse

/// Draws an ellipse using (x,y) as a center point and xradius and yradius as the horizontal 
/// and vertical axes, and fills it with the current fill color and fill pattern. 
/// 
/// \param x The x-coordinate of the center of the ellipse.
/// \param y The y-coordinate of the center of the ellipse.
/// \param xradius The x-radius of the ellipse.
/// \param yradius The y-radius of the ellipse.
BOSS_API void fillellipse(int x, int y, int xradius, int yradius)
{
//	fprintf(stderr, "fillellipse(%d,%d,%d,%d)\n", x, y, xradius, yradius);

	if(xradius < 0 || yradius < 0) {
		grVars.lastErr = grError;
		return;
	}
	
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	// start at top y, go to bottom y
	//const double maxY = (double)(y + yradius);
	double curY = (double)(y - yradius);

	int minX = 0;
	int maxX = 0;

	double a = (double)xradius;
	double b = (double)yradius;

	Uint32 fillColor = 0;
	switch(grVars.fillPattern) {
		case EMPTY_FILL:
			fillColor = grVars.bgColor;
			break;
//		case SOLID_FILL:
//			fillColor = grVars.fillColor;
//			break;
		default:
			fillColor = grVars.fillColor;
//			fprintf(stderr, "fillellipse() with fillPattern != SOLID_FILL not supported\n");
			break;
	} // switch(grVars.fillPattern)

	assert(grVars.theScreen);
	Slock(grVars.theScreen);

	// we only need to go halfway (to the y-center), because
	// the rest of the ellipse will be mirrored in y
	// 
	// the problem will be that the top line will always be only 1 pixel for 
	// medium-to-large ellipses due to rounding issues
	while(curY <= y) {

		// now find the min and max x
		// eqn of an ellipse is:
		// (x-xc)^2 / a^2 + (y-yc)^2 / b^2 = 1
		//
		// if we know y, then we can solve for x:
		// (x-xc)^2 / a^2 = 1 - (curY - yc)^2 / b^2
		// (x-xc)^2 = a^2 - (a/b)^2 * (curY - yc)^2
		// x - xc = sqrt( a^2 - (a/b)^2 * (curY - yc)^2 )
		// x = xc +/- sqrt( ... )
		double dbMinX = (double)x - sqrt( a*a - (a*a/(b*b)) * (curY - y)*(curY - y));
		minX = DoubleToInt( dbMinX );

		// this is an attempt to smooth out the top/bottom of the ellipse
		// for the top and bottom lines of the ellipse, we stretch it a little so
		// it doesn't appear as an annoying dot.  This means that technically it's
		// not an EXACT ellipse as it will be malformed, but it looks MUCH nicer!
		if(curY == (double)(y-yradius)) {
			double dbNextMinX = (double)x - sqrt( a*a - (a*a/(b*b)) * (curY - y + 1)*(curY - y + 1));
			
			minX -= (minX - DoubleToInt(dbNextMinX))/2;
//			fprintf(stderr, "dbMinX = %f, dbNextMinX = %f, x=%d\n", dbMinX, dbNextMinX, x);
		}

		// maxX will be x - minX + x
		maxX = 2*x - minX;

		// draw upper line and lowe rline
		RawDrawHLine(grVars.theScreen, minX, DoubleToInt(curY), (maxX-minX+1), fillColor, grVars.fillPattern);
		RawDrawHLine(grVars.theScreen, minX, 2*y - DoubleToInt(curY), (maxX-minX+1), fillColor, grVars.fillPattern);

		++curY;
	} // while(curY <= maxY)

	Sulock(grVars.theScreen);

	SDL_UpdateRect(grVars.theScreen, x-xradius, y-yradius, xradius*2+1, yradius*2+1);

	grVars.lastErr = grOk;
} // fillellipse

/// fillpoly draws the outline of a polygon with numpoints points in the current line 
/// style and color (just as drawpoly does), then fills the polygon using the current 
/// fill pattern and fill color. 
///
/// \param numpoints The number of points in the polygon.  The last point is automatically
/// connected to the first.
/// \param polypoints A sequence of (numpoints * 2) integers. Each pair of integers gives the 
/// x- and y-coordinates of a point on the polygon. 
///
/// WARNING:  It is not advised to use this function for any polygon over order 3 as it is 
/// much slower than drawing a simple triangle:  Each pixel is tested if it is in the polygon 
/// instead of a rasterized triangle).  If you can rework your code to use triangles you'll be happier.
BOSS_API void fillpoly(int numpoints, int* polypoints)
{
	if(!polypoints || numpoints < 1) {
		grVars.lastErr = grError;
		return;
	}

	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	int minx = getmaxx();
	int miny = getmaxy();
	int maxx = 0;
	int maxy = 0;

	// a dot
	if(numpoints == 1) {
		minx = maxx = polypoints[0];
		miny = maxy = polypoints[1];
		putpixel(polypoints[0], polypoints[1], grVars.fillColor);
	}
	// a line
	else if(numpoints == 2) {
		minx = Min(polypoints[0], polypoints[2]);
		miny = Min(polypoints[1], polypoints[3]);
		maxx = Max(polypoints[0], polypoints[2]);
		maxy = Max(polypoints[1], polypoints[3]);

		Uint32 oldFgColor = grVars.fgColor;
		grVars.fgColor = grVars.fillColor;
		line(polypoints[0], polypoints[1], polypoints[2], polypoints[3]);
		grVars.fgColor = oldFgColor;
	}
	// a triangle
	else if(numpoints == 3) {
		minx = Min(polypoints[0], Min(polypoints[2], polypoints[4]));
		miny = Min(polypoints[1], Min(polypoints[3], polypoints[5]));
		maxx = Max(polypoints[0], Max(polypoints[2], polypoints[4]));
		maxy = Max(polypoints[1], Max(polypoints[3], polypoints[5]));

		assert(grVars.theScreen);
		Slock(grVars.theScreen);
		grPoint pts[3];
		pts[0].x = polypoints[0]; pts[0].y = polypoints[1];
		pts[1].x = polypoints[2]; pts[1].y = polypoints[3];
		pts[2].x = polypoints[4]; pts[2].y = polypoints[5];
		RawFillTriangle(grVars.theScreen, pts, grVars.fillColor, grVars.fillPattern);
		RawDrawLine(grVars.theScreen, pts[0].x, pts[0].y, pts[1].x, pts[1].y, grVars.fgColor);
		RawDrawLine(grVars.theScreen, pts[1].x, pts[1].y, pts[2].x, pts[2].y, grVars.fgColor);
		RawDrawLine(grVars.theScreen, pts[2].x, pts[2].y, pts[0].x, pts[0].y, grVars.fgColor);
		Sulock(grVars.theScreen);
	}
	// beyond this point, we get into tricky waters...
	else {
//		fprintf(stderr, "fillpoly(%d+, ...) not yet supported, use filltriangle instead\n", numpoints);

		// we find the bounding box for this polygon:
		for(int pt = 0; pt < numpoints; ++pt) {
			if(polypoints[pt*2] < minx) {
				minx = polypoints[pt*2];
			}
			if(polypoints[pt*2] > maxx) {
				maxx = polypoints[pt*2];
			}
			if(polypoints[pt*2+1] < miny) {
				miny = polypoints[pt*2+1];
			}
			if(polypoints[pt*2+1] > maxy) {
				maxy = polypoints[pt*2+1];
			}
		} // for(int pt

//		fprintf(stderr, "Bounding box is %d,%d to %d,%d\n", minx, miny, maxx, maxy);

		assert(grVars.theScreen);
		Slock(grVars.theScreen);

		// now we have a bounding box, loop for EVERY line and find the 
		// horizontal line segments that are in the polygon and fill them in
		for(int y = miny; y <= maxy; ++y) {
			for(int x = minx; x <= maxx; ++x) {
				int hLineStartX = 0;
				int hLineWidth = 1;

				if(RawPointInPolygon(x, y, numpoints, polypoints)) {
					hLineStartX = x;
					hLineWidth = 1;
					while(RawPointInPolygon(++x, y, numpoints, polypoints)) {
						++hLineWidth;
						if(x >= maxx) {
							break;
						}
					}

//					fprintf(stderr, "Drawing hor strip x=%d, y=%d, width=%d\n", hLineStartX, y, hLineWidth);

					RawDrawHLine(grVars.theScreen, hLineStartX, y, hLineWidth, grVars.fillColor, 
						grVars.fillPattern);
				}
			} // for(int x...
		} // for(int y...

		// now draw outline
		for(int pt = 0; pt < numpoints-1; ++pt) {
			RawDrawLine(grVars.theScreen, polypoints[pt*2], polypoints[pt*2+1],
											polypoints[(pt+1)*2], polypoints[(pt+1)*2+1], 
											grVars.fgColor);
		}
		RawDrawLine(grVars.theScreen, polypoints[(numpoints-1)*2], polypoints[(numpoints-1)*2+1],
					polypoints[0], polypoints[1], grVars.fgColor);

		Sulock(grVars.theScreen);
	}

	SDL_UpdateRect(grVars.theScreen, minx, miny, (maxx-minx+1), (maxy-miny+1));
	grVars.lastErr = grOk;
} // fillpoly

/// getbkcolor returns the current background color. (See the table in setbkcolor for details.)
BOSS_API int getbkcolor(void)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return -1;
	}

	for(int col = 0; col < 256; ++col) {
		if(grVars.bgColor == VgaColorsInUint32[col]) {
			grVars.lastErr = grOk;
			return col;
		}
	}

	fprintf(stderr, "Unknown color found in getbkcolor\n");
	grVars.lastErr = grError;

	return -1;
} // getbkcolor

/// getcolor returns the current drawing color. 
///
/// The drawing color is the value to which pixels are set when lines and so on are drawn. 
/// In BOSS this will always be one of the VGA colors (0-255).
BOSS_API int getcolor(void)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return -1;
	}

	for(int col = 0; col < 256; ++col) {
		if(grVars.fgColor == VgaColorsInUint32[col]) {
			grVars.lastErr = grOk;
			return col;
		}
	}

	fprintf(stderr, "Unknown color found in getcolor\n");
	grVars.lastErr = grError;

	return -1;
} // getcolor

BOSS_API void getfillpattern(char *pattern)
{
	if(!pattern) return;

	// assume pattern is an allocated 8-byte array
	for(int row = 0; row < 8; ++row) {
		pattern[row] = 0;

		pattern[row] |= FillPatterns[grVars.fillPattern][row][0];
		pattern[row] <<= 1;
		pattern[row] |= FillPatterns[grVars.fillPattern][row][1];
		pattern[row] <<= 1;
		pattern[row] |= FillPatterns[grVars.fillPattern][row][2];
		pattern[row] <<= 1;
		pattern[row] |= FillPatterns[grVars.fillPattern][row][3];
		pattern[row] <<= 1;
		pattern[row] |= FillPatterns[grVars.fillPattern][row][4];
		pattern[row] <<= 1;
		pattern[row] |= FillPatterns[grVars.fillPattern][row][5];
		pattern[row] <<= 1;
		pattern[row] |= FillPatterns[grVars.fillPattern][row][6];
		pattern[row] <<= 1;
		pattern[row] |= FillPatterns[grVars.fillPattern][row][7];
	}
//	fprintf(stderr, "getfillpattern Function not implemented yet\n");
}

BOSS_API void getfillsettings(fillsettingstype* fillinfo)
{
	if(!fillinfo) return;

	fillinfo->color = grVars.fillColor;
	fillinfo->pattern = grVars.fillPattern;
//	fprintf(stderr, "getfillsettings Function not implemented yet\n");
}


/// getimage copies an image from the screen to memory
///
/// \param left X-coordinate of the left edge of the image to grab.
/// \param top Y-coordinate of the top edge of the image to grab.
/// \param right X-coordinate of the right edge of the image to grab.
/// \param bottom Y-coordinate of the bottom edge of the image to grab.
/// \param bitmap This pointer must point to valid memory that is large enough to store 
/// the image data.  See imagesize() on how to correctly size this parameter.
///
/// NOTE:  This function assumes the image is 16-colour data only.  Eventually this function
/// should be updated to support 256-colour or higher colour depths.  Only use this if your
/// on-screen images use the EGA 16-colours (first 16-colours of the VGA pallete).
BOSS_API void getimage(int left, int top, int right, int bottom, void *bitmap)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	// bitmap must be allocated elsewhere
	if(bitmap) {
		unsigned short int width = (right - left + 1);
		unsigned short int height = (bottom - top + 1);
//		fprintf(stderr, "getimage: width=%d, height=%d\n", width, height);

		unsigned short int* shData = (unsigned short int*)bitmap;

		*shData = width;
		++shData;
		*shData = height;
		++shData;

		Uint32 color32 = 0;
		//int numPlanes = 4;
		int numBytesPerPlane = (width-1)/8 + 1;
		//int numBytesPerRow = numBytesPerPlane * numPlanes;

		unsigned char* chData = (unsigned char*)(shData);
		unsigned char* pByte1 = chData;
		unsigned char* pByte2 = (chData+numBytesPerPlane);
		unsigned char* pByte3 = (chData+numBytesPerPlane*2);
		unsigned char* pByte4 = (chData+numBytesPerPlane*3);
		int bitcount = 0;

		assert(grVars.theScreen);
		Slock(grVars.theScreen);
		for(int y = top; y <= bottom; ++y) {
			
			*pByte1 = 0;
			*pByte2 = 0;
			*pByte3 = 0;
			*pByte4 = 0;

			for(int x = left; x <= right; ++x) {
//				fprintf(stderr, "y=%d, x=%d\n", y, x);

				color32 = RawGetPixel(grVars.theScreen, x, y);
//				fprintf(stderr, "Pixel at %d,%d is color %ld\n", x, y, color32);

				// this only works in EGA mode
				// TO DO:  Make this work for 256-color mode
				int col = 0;
				for(col = 0; col < 16; ++col) {
					if(color32 == VgaColorsInUint32[col]) {
						break;
					}
				}
				assert(col < 16);
				assert(col >= 0);

				// now we have the 4-bit color number (0-15)
				// the top bit goes in the first byte, etc

				// if the bit is set, roll it into the plane

				// if we're done, then get to next row
				if(bitcount == 8) { 
					++pByte1;
					++pByte2;
					++pByte3;
					++pByte4;
					*pByte1 = 0;
					*pByte2 = 0;
					*pByte3 = 0;
					*pByte4 = 0;
					bitcount = 0;
				} 
				if(bitcount != 0) {
					// roll the planes forward one bit
					*pByte1 <<= 1;
					*pByte2 <<= 1;
					*pByte3 <<= 1;
					*pByte4 <<= 1;
				}

				if(col & 0x08) {
					*pByte1 |= 0x1;
				}
				if(col & 0x04) {
					*pByte2 |= 0x1;
				}
				if(col & 0x02) {
					*pByte3 |= 0x1;
				}
				if(col & 0x01) {
					*pByte4 |= 0x1;
				}
				++bitcount;

//				fprintf(stderr, "Bytes are now: %d %d %d %d\n", *pByte1, *pByte2, *pByte3, *pByte4);

			} // for x

			// IMPORTANT:  Must roll the bits forward for each row if we're not finished with the bytes
			if(bitcount != 0) {
				while(bitcount < 8) {
					*pByte1 <<= 1;
					*pByte2 <<= 1;
					*pByte3 <<= 1;
					*pByte4 <<= 1;
					++bitcount;
				}

				// advance to next bytes
				bitcount = 0;
				pByte1 += numBytesPerPlane*3 + 1;
				pByte2 += numBytesPerPlane*3 + 1;
				pByte3 += numBytesPerPlane*3 + 1;
				pByte4 += numBytesPerPlane*3 + 1;
			}
		} // for y
		Sulock(grVars.theScreen);

//		fprintf(stderr, "\ngetimage buff = ");
//		int buffsize = imagesize(left, top, right, bottom);
//		unsigned char* pc = (unsigned char*)(bitmap);
//		for(int loop = 0; loop < buffsize; ++loop) {
//			fprintf(stderr, "%d ", (int)(*pc));
//			++pc;
//		}
//		fprintf(stderr, "\n");
	}

//	fprintf(stderr, "getimage Function not implemented yet\n");
	grVars.lastErr = grOk;
} // getimage

/// getmaxcolor returns the highest valid color value for the current graphics 
/// driver and mode that can be passed to setcolor. 
///
/// NOTE:  The BOSS version of this function always returns 15, however the programmer
/// may also use VGA colors.
BOSS_API int getmaxcolor(void)
{
//	fprintf(stderr, "getmaxcolor Function not implemented yet\n");

	return 15;
}

/// getmaxx returns the maximum (screen-relative) x value for the 
/// current graphics driver and mode
///
/// \return the maximum x screen coordinate.  If initgraph() has not been
/// called this function returns 0.  See graphresult() for error information.
BOSS_API int getmaxx(void)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return 0;
	}

	grVars.lastErr = grOk;
	return grVars.theScreen->w;
} // getmaxx

/// getmaxx returns the maximum (screen-relative) y value for the 
/// current graphics driver and mode
///
/// \return the maximum y screen coordinate.  If initgraph() has not been
/// called this function returns 0.  See graphresult() for error information.
BOSS_API int getmaxy(void)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return 0;
	}

	grVars.lastErr = grOk;
	return grVars.theScreen->h;
} // getmaxy

/// gettextsettings fills the textsettingstype structure pointed to by textinfo with 
/// information about the current text font, direction, size, and justification. 
///
/// The textsettingstype structure used by gettextsettings is defined in graphics.h as follows: 
/// \code
/// struct textsettingstype {
///		int font;
///		int direction;
///		int charsize;
///		int horiz;
///		int vert;
///	};
/// \endcode
BOSS_API void gettextsettings(textsettingstype *texttypeinfo)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	if(texttypeinfo) {
		texttypeinfo->charsize = grVars.curCharSize;
		texttypeinfo->direction = grVars.fontDirection;
		texttypeinfo->font = grVars.curFont;
		texttypeinfo->horiz = grVars.justH;
		texttypeinfo->vert = grVars.justV;
	}
	grVars.lastErr = grOk;
} // gettextsettings

/// getx finds the current graphics position's x-coordinate. The value is viewport-relative. 
///
/// \return the x-coordinate of the current position. 
BOSS_API int getx(void)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return 0;
	}
	grVars.lastErr = grOk;
	return grVars.penX;
} // getx

/// gety finds the current graphics position's y-coordinate. The value is viewport-relative. 
///
/// \return gety returns the y-coordinate of the current position. 
BOSS_API int gety(void)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return 0;
	}
	grVars.lastErr = grOk;
	return grVars.penY;
} // gety

/// getpixel gets the color of the pixel located at (x,y). 
///
/// \param x The x-coordinate of the pixel to retrieve
/// \param y The x-coordinate of the pixel to retrieve
///
/// \return getpixel returns the color of the given pixel. 
///
/// BOSS-specific:  This function returns the EGA/VGA palette colour number of
/// the given pixel
BOSS_API unsigned getpixel(int x, int y)
{
	assert(grVars.theScreen);
	assert(x >= 0);
	assert(x < grVars.theScreen->w);
	assert(y >= 0);
	assert(y < grVars.theScreen->h);

	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return 0;
	}

	Uint32 ActualColor = RawGetPixel(grVars.theScreen, x, y);

	// now do an inverse look-up of the color of the pixel to
	// get the paletted number
	for(int color = 0; color < 256; ++color) {
		if(ActualColor == VgaColorsInUint32[color]) {
			return color;
		}
	}

	return 0;
} // getpixel

/// graphresult returns the error code for the last graphics operation that reported 
/// an error and resets the error level to grOk. 
///
/// The following table lists the error codes returned by graphresult.  The enumerated 
/// type graph_errors defines the errors in this table. graph_errors is declared in boss.h. 
///
/// <TABLE>
/// <TR><TD><B>code</B></TD> <TD><B>constant</B></TD> <TD><B>Corresponding error message string</B></TD></TR>
/// <TR><TD>0  </TD><TD>grOk  </TD><TD>No error  </TD></TR>
/// <TR><TD>-1  </TD><TD>grNoInitGraph  </TD><TD>(BGI) graphics not installed (use initgraph)  </TD></TR>
/// <TR><TD>-2  </TD><TD>grNotDetected  </TD><TD>Graphics hardware not detected  </TD></TR>
/// <TR><TD>-3  </TD><TD>grFileNotFound  </TD><TD>Device driver file not found  </TD></TR>
/// <TR><TD>-4  </TD><TD>grInvalidDriver  </TD><TD>Invalid device driver file  </TD></TR>
/// <TR><TD>-5  </TD><TD>grNoLoadMem  </TD><TD>Not enough memory to load driver  </TD></TR>
/// <TR><TD>-6  </TD><TD>grNoScanMem  </TD><TD>Out of memory in scan fill  </TD></TR>
/// <TR><TD>-7  </TD><TD>grNoFloodMem  </TD><TD>Out of memory in flood fill  </TD></TR>
/// <TR><TD>-8  </TD><TD>grFontNotFound      </TD><TD>Font file not found  </TD></TR>
/// <TR><TD>-9  </TD><TD>grNoFontMem  </TD><TD>Not enough memory to load font  </TD></TR>
/// <TR><TD>-10  </TD><TD>grInvalidMode  </TD><TD>Invalid graphics mode for selected driver  </TD></TR>
/// <TR><TD>-11  </TD><TD>grError  </TD><TD>Graphics error  </TD></TR>
/// <TR><TD>-12  </TD><TD>grIOerror  </TD><TD>Graphics I/O error  </TD></TR>
/// <TR><TD>-13  </TD><TD>grInvalidFont  </TD><TD>Invalid font file  </TD></TR>
/// <TR><TD>-14  </TD><TD>grInvalidFontNum  </TD><TD>Invalid font number  </TD></TR>
/// <TR><TD>-15  </TD><TD>grInvalidDeviceNum  </TD><TD>Invalid device number  </TD></TR>
/// <TR><TD>-18  </TD><TD>grInvalidVersion  </TD><TD>Invalid version number  </TD></TR>
/// </TABLE>
/// 
/// Note: The variable maintained by graphresult is reset to 0 after graphresult has been called. 
/// Therefore, you should store the value of graphresult into a temporary variable and then test it. 
///
/// \return graphresult returns the current graphics error number, an integer in the range -15 to 0.
BOSS_API int graphresult(void)
{
	int err = grVars.lastErr;
	grVars.lastErr = grOk;
	return err;
}

/// imagesize determines the size of the memory area required to store a bit image.
///
/// \param left X-coordinate of the left edge of the image to size.
/// \param top Y-coordinate of the top edge of the image to size.
/// \param right X-coordinate of the right edge of the image to size.
/// \param bottom Y-coordinate of the bottom edge of the image to size.
///
/// \return imagesize returns the size of the required memory area in bytes
BOSS_API unsigned imagesize(int left, int top, int right, int bottom)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return 0;
	}

	unsigned short int width = (right - left + 1);
	unsigned short int height = (bottom - top + 1);

	int numRows = height;
	int numPlanesPerRow = 4;
	int numBytesPerPlane = width/8 + 1;

	grVars.lastErr = grOk;
	return 2*sizeof(unsigned short int) + (numBytesPerPlane * numPlanesPerRow * numRows);
} // imagesize

/// This function initializes the graphics subsystems into various modes.  
///
/// \param graphdriver must point to an integer containing one of the following 
/// values (all other BGI values will return an error):
/// <BR>
/// <TABLE>
/// <TR>
///	<TD ALIGN="CENTER"><B>Enum</B></TD> <TD ALIGN="CENTER"><B>Value</B></TD> <TD><B>Notes</B></TD></TR>
/// <TR><TD ALIGN="CENTER">DETECT</TD> <TD>0</TD>	
///		<TD>This will be 640x480 fullscreen (equivalent to BOSS_FULLSCREEN with mode BFS_640_480)</TD></TR>
/// <TR><TD ALIGN="CENTER">BOSS_FULLSCREEN</TD> <TD>100</TD>
///		<TD>This allows fullscreen mode at various resolutions (depending on graphmode)</TD></TR>
/// <TR><TD ALIGN="CENTER">BOSS_WINDOWED</TD> <TD>101</TD>
///		<TD>This allows windowed mode at various resolutions (see notes on graphmode)</TD></TR>
/// </TABLE>
/// <BR>
/// \param graphmode For BOSS_FULLSCREEN, graphmode must point to an integer with one of the following
/// values:
/// <BR>
/// <TABLE>
/// <TR>
///	<TD ALIGN="CENTER"><B>Enum</B></TD><TD ALIGN="CENTER"><B>Value</B></TD><TD><B>Notes</B></TD></TR>
/// <TR><TD>BFS_640_480</TD> <TD>0</TD> <TD>640x480 fullscreen</TD></TR>
/// <TR><TD>BFS_800_600</TD> <TD>1</TD> <TD>800x600 fullscreen</TD></TR>
/// <TR><TD>BFS_1024_768</TD> <TD>2</TD> <TD>1024x768 fullscreen</TD></TR>
/// <TR><TD>BFS_1280_1024</TD> <TD>3</TD> <TD>1280x1024 fullscreen</TD></TR>
/// <TR><TD>BFS_1600_1200</TD> <TD>4</TD> <TD>1600x1200 fullscreen</TD></TR>
/// <TR><TD>BFS_320_200</TD> <TD>5</TD> <TD>320x200 fullscreen</TD></TR>
/// <TR><TD>BFS_320_240</TD> <TD>6</TD> <TD>320x240 fullscreen</TD></TR>
/// </TABLE>
/// <BR>
/// For BOSS_WINDOWED, graphmode must point to an integer where the upper word 
/// contains the width and the lower word contains the height.  For example, to 
/// create a window of width 500 and height 300:
/// <BR>
/// \code
/// int gdriver = BOSS_WINDOWED;
/// int gmode = ((short)500 << 16) | (short)(300);
/// initgraph(&gdriver, &gmode, "");
/// \endcode
/// <BR>
/// \param pathtodriver This parameter is ignored.
///
/// \return This function does not return a value.  Callers should check graphresult and
/// check the error code.
/// 
/// NOTE: In all modes, the color depth will be 24 or 32-bits, but the BGI interface only is
/// supposed to allow the EGA color palette to be accessible.  With the exception of putimage 
/// and getimage (in which the image data must use only the first 16 EGA colors), the BOSS 
/// interface allows access to the VGA color palette (256 colors) .  (To use a higher number 
/// colours, start programming in SDL).
BOSS_API void initgraph(int *graphdriver, int *graphmode, char *pathtodriver)
{
#if defined(_DEBUG) 
	fprintf(stderr, "Initializing BOSS version %f\n", getVersion()); 
#endif
	assert(graphdriver);
	assert(graphmode);
	assert(pathtodriver);

	int Flags = SDL_SWSURFACE;
	int width = 0;
	int height = 0;

	// determine the parameters of the surface to create...first width and height:
	if(*graphdriver == DETECT) {
		width = 640;
		height = 480;
		Flags |= SDL_FULLSCREEN;
	}
	else if(*graphdriver == BOSS_FULLSCREEN) {
		switch(*graphmode) {
			case BFS_640_480:
				width = 640;
				height = 480;
				break;
			case BFS_800_600:
				width = 800;
				height = 600;
				break;
			case BFS_1024_768:
				width = 1024;
				height = 768;
				break;
			case BFS_1280_1024:
				width = 1280;
				height = 1024;
				break;
			case BFS_1600_1200:
				width = 1600;
				height = 1200;
				break;
			case BFS_320_200:
				width = 320;
				height = 200;
				break;
			case BFS_320_240:
				width = 320;
				height = 240;
				break;
			default:
				grVars.lastErr = grInvalidMode;
				return;
				break;
		} // switch
		Flags |= SDL_FULLSCREEN;
	}
	else if(*graphdriver == BOSS_WINDOWED) {
		width = (*graphmode) >> 16;
		height = (*graphmode) & 0xFFFF;
		if(width < 1 || height < 1) {
			grVars.lastErr = grInvalidMode;
			return;
		}
	}
	else {
		grVars.lastErr = grInvalidDriver;
		return;
	}

	// register SDL_Quit to be called upon our program exiting
	atexit(SDL_Quit);

	// initialize SDL
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		fprintf( stderr, "Video initialization failed: %s\n",
			SDL_GetError() );
		grVars.lastErr = grNoInitGraph;
	}
	else {

		// get the screen SDL surface
		grVars.theScreen = SDL_SetVideoMode(width, height, 32, Flags);
		if(grVars.theScreen  == NULL) {
			fprintf(stderr, "Could not obtain the screen surface: %s\n",
				SDL_GetError() );

			grVars.lastErr = grInvalidMode;
		}
		else { // we're good to go...initialize the remaining stuff

			RawInitColors(grVars.theScreen);

			grVars.lastErr = grOk;
		}
	} // else
} // initgraph

/// line draws a line in the current color, using the current line style and thickness 
/// between the two points specified, (x1,y1) and (x2,y2), without updating the current 
/// position (CP). 
///
/// \param x1 X-coordinate of the first endpoint of the line.
/// \param y1 Y-coordinate of the first endpoint of the line.
/// \param x2 X-coordinate of the second endpoint of the line.
/// \param y2 Y-coordinate of the second endpoint of the line.
/// 
/// NOTE: BOSS-Specific:  Line style and thickness are not yet implemented.
BOSS_API void line(int x1, int y1, int x2, int y2)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	Slock(grVars.theScreen);
	RawDrawLine(grVars.theScreen, x1, y1, x2, y2, grVars.fgColor);
	Sulock(grVars.theScreen);
	SDL_UpdateRect(grVars.theScreen, x1, y1, x2-x1+1, y2-y1+1);
	grVars.lastErr = grOk;
} // line

/// linerel draws a line from the CP (current position) to a point that 
/// is a relative distance (dx,dy) from the CP. The CP is advanced by (dx,dy). 
/// 
/// \param dx The x-distance from the Current Position to which the line should be drawn.
/// \param dy The y-distance from the Current Position to which the line should be drawn.
BOSS_API void linerel(int dx, int dy)
{
	line(grVars.penX, grVars.penY, grVars.penX + dx, grVars.penY + dy);
	if(grVars.lastErr == grOk) {
		moverel(dx, dy);
	}
}

/// lineto draws a line from the CP (current position) to (x,y), then moves the CP to (x,y). 
/// 
/// \param x The x-coordinate of the second endpoint
/// \param y The y-coordinate of the second endpoint.
BOSS_API void lineto(int x, int y)
{
	line(grVars.penX, grVars.penY, x, y);
	if(grVars.lastErr == grOk) {
		moveto(x, y);
	}
} // lineto

/// moverel moves the current position (CP) dx pixels in the x direction 
/// and dy pixels in the y direction. 
/// 
/// \param dx The x-distance from the Current Position to which the CP should be moved.
/// \param dy The y-distance from the Current Position to which the CP should be moved.
BOSS_API void moverel(int dx, int dy)
{
	grVars.penX += dx;
	grVars.penY += dy;
	grVars.lastErr = grOk;
} // moverel

/// moveto moves the current position (CP) to viewport position (x,y). 
/// 
/// \param x The x-coordinate to move the CP destination.
/// \param y The y-coordinate to move the CP destination.
BOSS_API void moveto(int x, int y)
{
	grVars.penX = x;
	grVars.penY = y;
	grVars.lastErr = grOk;
} // moveto

// if horiz text justification is LEFT_TEXT and font direction
// is HORIZ_DIR, then the Current Position is advanced by textwidth(textstring)
BOSS_API void outtext(char *textstring)
{
	outtextxy(grVars.penX, grVars.penY, textstring);

	if(grVars.justH == LEFT_TEXT && grVars.fontDirection == HORIZ_DIR && grVars.lastErr == grOk)
	{
		moverel( textwidth(textstring), 0 );
	}
}

BOSS_API void outtextxy(int x, int y, char *textstring)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	TTF_Font* pFont = grVars.fonts[grVars.curFont].pFont;
	
	if(!pFont) { 
		settextstyle(grVars.curFont, grVars.fontDirection, grVars.curCharSize);

		pFont = grVars.fonts[grVars.curFont].pFont;
//		fprintf(stderr, "outtextxy Function not implemented yet for this text setting (font = )\n",
//			grVars.fontDirection, grVars.justH, grVars.justV);
//		return;
	}

	assert(grVars.theScreen);
	assert(pFont);

	// convert from a Uint32 color to a SDL_Color...am i doing it right?
	SDL_PixelFormat* pFormat = grVars.theScreen->format;
	assert(pFormat);

	int red = ((grVars.fgColor & pFormat->Rmask) >> pFormat->Rshift);
	int green = ((grVars.fgColor & pFormat->Gmask) >> pFormat->Gshift);
	int blue = ((grVars.fgColor & pFormat->Bmask) >> pFormat->Bshift);

	// now set the foreground color
	SDL_Color clrFg = {red,green,blue,0};
	Uint32 clrFg32 = SDL_MapRGB(grVars.theScreen->format, red, green, blue);

//	fprintf(stderr, "red=%d, green=%d, blue=%d, clrFg32=%d\n", red, green, blue, clrFg32);

	// now render to an offscreen surface:
	SDL_Surface *sText = TTF_RenderText_Solid( pFont, textstring, clrFg );
	// only if we get a valid surface do we display the text
	if(!sText) {
		// TO DO:  Figure out what to do here if not...error code?
		return;
	}

	// now figure out the justification...
	int w = 0;
	int h = 0;
	TTF_SizeText(pFont, textstring, &w, &h);
	// TO DO:  What does TTF_SizeText return?  an error code?  If so, handle it here:
//	{
//		fprintf(stderr, "Encountered some error caling TTF_SizeText in outtextxy()");
//		fprintf(stderr, "width = %d, height = %d\n", w, h);
//		return;
//	}

	// now if it's HORIZ_DIR, it's just a straight blit
	// but if it's VERT_DIR, we need to rotate the image...

	SDL_Rect rcDest = {0,0,0,0};

	if(grVars.fontDirection == HORIZ_DIR) {

		// handle horizontal justification
		if(grVars.justH == LEFT_TEXT) {
			rcDest.x = x;
		}
		else if(grVars.justH == CENTER_TEXT) {
			rcDest.x = (x - w/2);
		}
		else if(grVars.justH == RIGHT_TEXT) {
			rcDest.x = (x - w);
		}

		// handle vertical justification
		if(grVars.justV == TOP_TEXT) {
			rcDest.y = y;
		}
		else if(grVars.justV == CENTER_TEXT) {
			rcDest.y = (y - h/2);
		}
		else if(grVars.justV == BOTTOM_TEXT) {
			rcDest.y = (y - h);
		}

		// need this for our UpdateRect() call later
		rcDest.w = w;
		rcDest.h = h;

		// now we have the insertion point, do the blit:
		SDL_BlitSurface( sText, NULL, grVars.theScreen, &rcDest );

	} // if(HORIZ_DIR)
	else if(grVars.fontDirection == VERT_DIR) {

		// handle horizontal justification
		if(grVars.justH == LEFT_TEXT) {
//			rcDest.x = x;
			rcDest.y = y;
		}
		else if(grVars.justH == CENTER_TEXT) {
//			rcDest.x = (x - w/2);
			rcDest.y = (y - h/2);
		}
		else if(grVars.justH == RIGHT_TEXT) {
//			rcDest.x = (x - w);
			rcDest.y = (y - h);
		}

		// handle vertical justification
		if(grVars.justV == TOP_TEXT) {
//			rcDest.y = y;
			rcDest.x = x;
		}
		else if(grVars.justV == CENTER_TEXT) {
//			rcDest.y = (y - h/2);
			rcDest.x = (x - w/2);
		}
		else if(grVars.justV == BOTTOM_TEXT) {
//			rcDest.y = (y - h);
			rcDest.x = (x - w);
		}

		// need this for our UpdateRect() call later
		rcDest.h = w; // swapped as the text is rotated
		rcDest.w = h;

//		fprintf(stderr, "Printing Vertically the text '%s'\n", textstring);
//		fprintf(stderr, "rcDest.x=%d, .y=%d, .w=%d, .h=%d\n",
//					rcDest.x, rcDest.y, rcDest.w, rcDest.h);

		// now we have the insertion point, do the blit:
		// each pixel(xn,yn) in the source surface gets blitted to
		// a pixel in the destination surface that is rotated and translated
		Slock(sText);
		Slock(grVars.theScreen);

		for(int pixy = 0; pixy < h; ++pixy) {
			for(int pixx = 0; pixx < w; ++pixx) {
//				fprintf(stderr, "...on pixel %d,%d, color=%d\n", pixx, pixy, 
//					RawGetPixel(sText, pixx, pixy) );
				if(RawGetPixel(sText, pixx, pixy)) {
					RawDrawPixel(grVars.theScreen, pixy + rcDest.x, 
						rcDest.h - pixx - 1 + rcDest.y, clrFg32);
				}
			}
		}

		Sulock(grVars.theScreen);
		Sulock(sText);

	} // if(VERT_DIR)

	SDL_FreeSurface( sText );
	
	// the w and h are wrong...
	SDL_UpdateRect(grVars.theScreen, rcDest.x, rcDest.y, rcDest.w, rcDest.h);

	grVars.lastErr = grOk;
} // outtextxy

// only operation supported is COPY_PUT, image must be EGA only!
BOSS_API void putimage(int left, int top, void *bitmap, int op)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	if(!bitmap || !(op == COPY_PUT || op == XOR_PUT || op == OR_PUT || op == AND_PUT || op == NOT_PUT)) {
		grVars.lastErr = grError;
		return;
	}

	if(op != COPY_PUT) {
		fprintf(stderr, "putimage(..., %d) not yet supported.\n", op);
		return;
	}

	unsigned char* pBuf = (unsigned char*)bitmap;

	unsigned short int width = *(unsigned short int*)(pBuf);
	pBuf += sizeof(unsigned short int);
	unsigned short int height = *(unsigned short int*)(pBuf);
	pBuf += sizeof(unsigned short int);

//	fprintf(stderr, "putimage: width=%d, height=%d\n", width, height);

	// TO DO:  Generalize this function for any color mode
	// The following assumes 16-bit EGA:
	
	// The remaining image data has the following pseudo-structure:
	//
	// struct ImageBuffer { 
	//	ImageRow rows[height]; 
	// }
	//
	// where : 
	// 
	// struct ImageRow { 
	//	ImageRowBits Bit3;
	//	ImageRowBits Bit2;
	//	ImageRowBits Bit1;
	//	ImageRowBits Bit0;
	// }; 
	//
	// ImageRowBits { 
	//		bit pixel[width]; 
	//		bit pad[((width-1)/8 + 1) * 8 - width]; 
	// } 
	//
	// The 4 bits are used to lookup into the existing 16-color palette.
	//

	assert(grVars.theScreen);
	Slock(grVars.theScreen);

	for(int y = 0; y < height; ++y) { 
		for(int x = 0; x < width; ++x) { 

			// here we roll in each bit from each plane, culminating
			// in a 4-bit number that represents our color number in 
			// the 16-color palette
			int colNum = Internal_getBit(x, y, width, 0, pBuf);
			colNum <<= 1;
			colNum |= Internal_getBit(x, y, width, 1, pBuf);
			colNum <<= 1;
			colNum |= Internal_getBit(x, y, width, 2, pBuf);
			colNum <<= 1;
			colNum |= Internal_getBit(x, y, width, 3, pBuf);

			Uint32 RealColor = SDL_MapRGB(grVars.theScreen->format, VgaColors[colNum].r, 
							VgaColors[colNum].g, VgaColors[colNum].b);
			RawDrawPixel(grVars.theScreen, x+left, y+top, RealColor);
		} 
	}

	Sulock(grVars.theScreen);

	SDL_UpdateRect(grVars.theScreen, left, top, width, height);

	grVars.lastErr = grOk;
} // putimage

BOSS_API void putpixel(int x, int y, int color)
{
	assert(grVars.theScreen);
	assert(x >= 0);
	assert(y >= 0);
	assert(x < grVars.theScreen->w);
	assert(y < grVars.theScreen->h);
	assert(color >= 0);
	assert(color < 256);

	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	assert(grVars.theScreen);

	Slock(grVars.theScreen);
//		Uint32 RealColor = SDL_MapRGB(grVars.theScreen->format, VgaColors[color].r, 
//								VgaColors[color].g, VgaColors[color].b);
		RawDrawPixel(grVars.theScreen, x, y, VgaColorsInUint32[color]);
	Sulock(grVars.theScreen);

	SDL_UpdateRect(grVars.theScreen, x, y, 1, 1);

	grVars.lastErr = grOk;
} // putpixel

BOSS_API void rectangle(int left, int top, int right, int bottom)
{
	assert(grVars.theScreen);
	assert(left >= 0);
	assert(left < grVars.theScreen->w);
	assert(top >= 0);
	assert(top < grVars.theScreen->h);
	assert(right >= 0);
	assert(right < grVars.theScreen->w);
	assert(bottom >= 0);
	assert(bottom < grVars.theScreen->h);

	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	assert(grVars.theScreen);

	Slock(grVars.theScreen);

		RawDrawHLine(grVars.theScreen, left, top, (right-left+1), grVars.fgColor, SOLID_FILL);
		RawDrawHLine(grVars.theScreen, left, top, (right-left+1), grVars.fgColor, SOLID_FILL);
		RawDrawVLine(grVars.theScreen, left, top, (bottom-top+1), grVars.fgColor, SOLID_FILL);
		RawDrawVLine(grVars.theScreen, right, top, (bottom-top+1), grVars.fgColor, SOLID_FILL);

	Sulock(grVars.theScreen);

	SDL_UpdateRect(grVars.theScreen, left, top, (right-left+1), (bottom-top+1));

	grVars.lastErr = grOk;
} // rectangle

BOSS_API void setbkcolor(int color)
{
	assert(color >= 0);
	assert(color < 256);

	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	if(color >=0 && color <= 255) {
		grVars.bgColor = SDL_MapRGB(grVars.theScreen->format, 
								VgaColors[color].r, 
								VgaColors[color].g,
								VgaColors[color].b);
		grVars.lastErr = grOk;
	}
	else {
		fprintf(stderr, "setbkcolor function received unknown color: %d", color);
		grVars.lastErr = grError;
	}
} // setbkcolor

BOSS_API void setcolor(int color)
{
	assert(color >= 0);
	assert(color < 256);

	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	if(color >=0 && color <= 255) {
		grVars.fgColor = SDL_MapRGB(grVars.theScreen->format, 
								VgaColors[color].r, 
								VgaColors[color].g,
								VgaColors[color].b);
		grVars.lastErr = grOk;
	}
	else {
		fprintf(stderr, "setcolor function received unknown color: %d", color);
		grVars.lastErr = grError;
	}
} // setcolor

BOSS_API void setfillpattern(char *upattern, int color)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	// upattern contains 8 bytes x 8 bits = 64 bits.  
	// take these bits and set the USER_FILL pattern

	for(int y = 0; y < 8; ++y) {
		for(int x = 0; x < 8; ++x) {
			unsigned char byte = (unsigned char)upattern[y];
			int bit = (byte & 0x1);
			FillPatterns[USER_FILL][y][x] = bit;
			byte >>= 1;
		}
	}

	grVars.lastErr = grOk;
} // setfillpattern

BOSS_API void setfillstyle(int pattern, int color)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	switch(pattern) {
		case EMPTY_FILL: 
		case SOLID_FILL: 
		case LINE_FILL: 
		case LTSLASH_FILL: 
		case SLASH_FILL: 
		case BKSLASH_FILL: 
		case LTBKSLASH_FILL: 
		case HATCH_FILL: 
		case XHATCH_FILL: 
		case INTERLEAVE_FILL: 
		case WIDE_DOT_FILL: 
		case CLOSE_DOT_FILL: 
		case USER_FILL:
			grVars.fillPattern = pattern;
			break;
		default:
			grVars.lastErr = grError;
			break;
	} // switch(pattern)

	grVars.fillColor = SDL_MapRGB(grVars.theScreen->format, 
								VgaColors[color].r, 
								VgaColors[color].g,
								VgaColors[color].b);
	grVars.lastErr = grOk;
} // setfillstyle

BOSS_API void settextjustify(int horiz, int vert)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	if( (horiz != LEFT_TEXT && horiz != CENTER_TEXT && horiz != RIGHT_TEXT) ||
		(vert != BOTTOM_TEXT && vert != CENTER_TEXT && vert != TOP_TEXT) )
	{
		grVars.lastErr = grError;
	}
	else {
		grVars.justH = horiz;
		grVars.justV = vert;
		grVars.lastErr = grOk;
	}
} // settextjustify

// TO DO:  Determine a way to catch the errors if the file doesn't exist...
BOSS_API void settextstyle(int font, int direction, int charsize)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return;
	}

	// if font is invalid, return and set error
	if(font >= MAX_FONTS || font < 0) {
		grVars.lastErr = grInvalidFontNum;
		return; 
	}

	// if direction is invalid, return and set error
	if(direction != HORIZ_DIR && direction != VERT_DIR) {
		grVars.lastErr = grInvalidFont;
		return;
	}
	if(charsize < 0 || charsize > 9) {
		grVars.lastErr = grInvalidFont;
		return;
	}

	// if TTF module has not been initialized, initialize it
	if(!grVars.bTTFInitialized) {
//		fprintf(stderr, "TTF Initing\n");
		// TTF_Font test...
		if(!TTF_Init()) {
			atexit(TTF_Quit);

			// initialize our font array...
			for(int font = 0; font < MAX_FONTS; ++font) {
				grVars.fonts[font].charSize = 0;
				grVars.fonts[font].pFont = NULL;
			}

			grVars.bTTFInitialized = true;

//			fprintf(stderr, "TTF Inited\n");
		}
		else {
			fprintf(stderr, "TTF_Init failed inside settextstyle\n");
			grVars.lastErr = grNoFontMem;
			return;
		}
	} // if(!grVars.bTTFInitialized)

	// convert charsize into point size
	int ptSize = FontPtSize[font][charsize];
	if(ptSize == 0) {
		ptSize = charsize*8;
	}
//	fprintf(stderr, "Font point size = %d, charsize = %d\n", ptSize, charsize);

	// now we need to get the proper font
	if(!grVars.fonts[font].pFont || grVars.fonts[font].charSize != charsize) {
		if(grVars.fonts[font].pFont) {
//			fprintf(stderr, "Closing font to reopen\n");
			TTF_CloseFont( grVars.fonts[font].pFont );
			grVars.fonts[font].pFont = NULL;
		}
//		fprintf(stderr, "Opening font to reopen\n");
		FILE* fp = fopen( szFontFilenames[font], "rb");
		if(fp) {
			fclose(fp);

			grVars.fonts[font].pFont = TTF_OpenFont( szFontFilenames[font], ptSize );
//			fprintf(stderr, "Finished open call\n");
			grVars.fonts[font].charSize = charsize;
		}
		else {
			if(!grVars.fonts[font].pFont) {
				fprintf(stderr, "Could not find font file: %s\n", szFontFilenames[font]);
				grVars.lastErr = grFontNotFound;
				return;
			}
		}
	}
	assert(grVars.fonts[font].pFont);
	
	// set our current text style
	grVars.curFont = font;
	grVars.fontDirection = direction;

	// set out current text size
	grVars.curCharSize = charsize;

	grVars.lastErr = grOk;
} // settextstyle

BOSS_API int textheight(char *textstring)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return -1;
	}

	TTF_Font* pFont = grVars.fonts[grVars.curFont].pFont;
	
	if(!pFont) { 
		fprintf(stderr, "textheight Function not implemented yet for this text setting\n");
		return -1;
	}

	assert(pFont);

	// now figure out the width...
	int width = 0;
	int height = 0;
	TTF_SizeText(pFont, textstring, &width, &height);

	grVars.lastErr = grOk;
	return height;
} // textheight

BOSS_API int textwidth(char *textstring)
{
	if(!grVars.theScreen) {
		grVars.lastErr = grNoInitGraph;
		return -1;
	}

	TTF_Font* pFont = grVars.fonts[grVars.curFont].pFont;
	
	if(!pFont) { 
		fprintf(stderr, "textwidth Function not implemented yet for this text setting\n");
		return -1;
	}

	assert(pFont);

	// now figure out the width...
	int width = 0;
	int height = 0;
	TTF_SizeText(pFont, textstring, &width, &height);

	grVars.lastErr = grOk;
	return width;
} // textwidth

// Non-implemented Functions
namespace Not_Implemented {

BOSS_API void bar3d(int left, int top, int right, int bottom, int depth, int topflag)
{
	fprintf(stderr, "bar3d Function not implemented yet\n");
}

BOSS_API void detectgraph(int *graphdriver, int *graphmode)
{
	fprintf(stderr, "detectgraph Function not implemented yet\n");
}

BOSS_API void floodfill(int x, int y, int border)
{
	fprintf(stderr, "floodfill Function not implemented yet\n");
}

BOSS_API void getarccoords(struct arccoordstype *arccoords)
{
	fprintf(stderr, "getarccoords Function not implemented yet\n");
}

BOSS_API void getaspectratio(int *xasp, int *yasp)
{
	fprintf(stderr, "getaspectratio Function not implemented yet\n");
}

BOSS_API struct palettetype* getdefaultpalette (void)
	{
	fprintf(stderr, "getdefaultpalette Function not implemented yet\n");

	return 0;
}

BOSS_API char* getdrivername(void)
{
	fprintf(stderr, "getdrivername Function not implemented yet\n");

	return 0;
}

BOSS_API int getgraphmode(void)
{
	fprintf(stderr, "getgraphmode Function not implemented yet\n");

	return 0;
}

BOSS_API void getlinesettings(linesettingstype *lineinfo)
{
	fprintf(stderr, "getlinesettings Function not implemented yet\n");
}

BOSS_API int getmaxmode(void)
{
	fprintf(stderr, "getmaxmode Function not implemented yet\n");

	return 0;
}

BOSS_API char* getmodename(int mode_number)
{
	fprintf(stderr, "getmodename Function not implemented yet\n");

	return 0;
}

BOSS_API void getmoderange(int graphdriver, int *lomode, int *himode)
{
	fprintf(stderr, "getmoderange Function not implemented yet\n");
}

BOSS_API void getpalette(palettetype *palette)
{
	fprintf(stderr, "getpalette Function not implemented yet\n");
}

BOSS_API int getpalettesize(void)
{
	fprintf(stderr, "getpalettesize Function not implemented yet\n");

	return 0;
}

BOSS_API void getviewsettings(viewporttype *viewport)
{
	fprintf(stderr, "getviewsettings Function not implemented yet\n");
}

BOSS_API void graphdefaults(void)
{
	fprintf(stderr, "graphdefaults Function not implemented yet\n");
}

BOSS_API char* grapherrormsg(int errorcode)
{
	fprintf(stderr, "grapherrormsg Function not implemented yet\n");

	return 0;
}

BOSS_API int installuserdriver(char *name, int (*detect)(void))
{
	fprintf(stderr, "installuserdriver Function not implemented yet\n");

	return 0;
}

BOSS_API int installuserfont(char *name)
{
	fprintf(stderr, "installuserfont Function not implemented yet\n");

	return 0;
}

// fills in a wedge with the fill color and style, then draws a border around 
// it with current drawing color
BOSS_API void pieslice(int x, int y, int stangle, int endangle, int radius)
{
	// since i don't have the stamina to figure out a good way to do this
	// I figure I'll just draw an elliptical arc with radii from

	fprintf(stderr, "pieslice Function not implemented yet\n");
}

BOSS_API int registerbgidriver(void (*driver)(void))
{
	fprintf(stderr, "registerbgidriver Function not implemented yet\n");

	return 0;
}

BOSS_API int registerbgifont(void (*font)(void))
{
	fprintf(stderr, "registerbgifont Function not implemented yet\n");

	return 0;
}

// Windows-specific...probably will never implement
//BOSS_API void clearmouseclick(int kind)
//{
//	fprintf(stderr, "clearmouseclick Function not implemented yet\n");
//}
//
//BOSS_API int getactivepage(void)
//{
//	fprintf(stderr, "getactivepage Function not implemented yet\n");
//
//	return 0;
//}
//
//BOSS_API void getmouseclick(int kind, int& x, int& y)
//{
//	fprintf(stderr, "getmouseclickFunction not implemented yet\n");
//}
//
//BOSS_API int getvisualpage(void)
//{
//	fprintf(stderr, "getvisualpage Function not implemented yet\n");
//
//	return 0;
//}
//
//BOSS_API void initwindow(int width, int height)
//{
//	fprintf(stderr, "initwindow Function not implemented yet\n");
//}
//
//BOSS_API bool ismouseclick(int kind)
//{
//	fprintf(stderr, "ismouseclick Function not implemented yet\n");
//
//	return false;
//}
//
//BOSS_API int mousex(void)
//{
//	fprintf(stderr, "mousex Function not implemented yet\n");
//
//	return 0;
//}
//
//BOSS_API int mousey(void)
//{
//	fprintf(stderr, "mousey Function not implemented yet\n");
//
//	return 0;
//}
//
//BOSS_API void registermousehandler(int kind, void h(int, int))
//{
//	fprintf(stderr, "registermousehandler Function not implemented yet\n");
//}

BOSS_API void restorecrtmode(void)
{
	fprintf(stderr, "restorecrtmode Function not implemented yet\n");
}

// fills in a wedge with the fill color and style, then draws a border around 
// it with current drawing color
BOSS_API void sector(int x, int y, int stangle, int endangle, int xradius, int yradius)
{
//	assert(xradius > 0);
//	assert(yradius > 0);
//
//	// clip our angles to the range 0 and 360
//	while(stangle < 0) { stangle += 360; }
//	while(endangle < 0) { endangle += 360; }
//	while(stangle >= 360) { stangle -= 360; }
//	while(endangle >= 360) { endangle -= 360; }
//
//	// start at top y, go to bottom y
//	const double maxY = (double)(y + yradius);
//	double curY = (double)(y - yradius);
//
//	int minX = 0;
//	int maxX = 0;
//
//	double a = (double)xradius;
//	double b = (double)yradius;
//
//	Uint32 fillColor = 0;
//	switch(grVars.fillPattern) {
//		case EMPTY_FILL:
//			fillColor = grVars.bgColor;
//			break;
//		case SOLID_FILL:
//			fillColor = grVars.fillColor;
//			break;
//		default:
//			fillColor = grVars.fillColor;
//			fprintf(stderr, "fillellipse() with fillPattern != SOLID_FILL not supported\n");
//			break;
//	} // switch(grVars.fillPattern)
//
//	assert(grVars.theScreen);
//	Slock(grVars.theScreen);
//
//	// go from top to bottom
//	while(curY <= maxY) {
//
//		// now find the min and max x
//		// eqn of an ellipse is:
//		// (x-xc)^2 / a^2 + (y-yc)^2 / b^2 = 1
//		//
//		// if we know y, then we can solve for x:
//		// (x-xc)^2 / a^2 = 1 - (curY - yc)^2 / b^2
//		// (x-xc)^2 = a^2 - (a/b)^2 * (curY - yc)^2
//		// x - xc = sqrt( a^2 - (a/b)^2 * (curY - yc)^2 )
//		// x = xc +/- sqrt( ... )
//		double dbMinX = (double)x - sqrt( a*a - (a*a/(b*b)) * (curY - y)*(curY - y));
//		minX = DoubleToInt( dbMinX );
//
//		// this is an attempt to smooth out the top/bottom of the ellipse
//		// for the top and bottom lines of the ellipse, we stretch it a little so
//		// it doesn't appear as an annoying dot.  This means that technically it's
//		// not an EXACT ellipse as it will be malformed, but it looks MUCH nicer!
//		if(curY == (double)(y-yradius) || curY == (double)(y+yradius)) {
//			double dbNextMinX = (double)x - sqrt( a*a - (a*a/(b*b)) * (curY - y + 1)*(curY - y + 1));
//			
//			minX -= (minX - DoubleToInt(dbNextMinX))/2;
////			fprintf(stderr, "dbMinX = %f, dbNextMinX = %f, x=%d\n", dbMinX, dbNextMinX, x);
//		}
//
//		// maxX will be x - minX + x
//		maxX = 2*x - minX;
//
//		// angle is measured counter-clockwise, with 0 degrees at 3 o'clock
//		// now if our start angle is between 0 and 180, our maxX is affected
//		// if our start angle is between 180 and 360, our minX is affected
//
//		// if our end angle is between 0 and 180, our minX is affected
//		// if our end angle is between 180 and 360, our maxX is affected
//
//		// draw upper line and lowe rline
//		RawDrawHLine(grVars.theScreen, minX, DoubleToInt(curY), (maxX-minX+1), fillColor, grVars.//fillPattern);
//		RawDrawHLine(grVars.theScreen, minX, 2*y - DoubleToInt(curY), (maxX-minX+1), fillColor, grVars.fillPattern);
//
//		++curY;
//	} // while(curY <= maxY)
//
//	Sulock(grVars.theScreen);
//
//	SDL_UpdateRect(grVars.theScreen, x-xradius, y-yradius, xradius*2+1, yradius*2+1);

	fprintf(stderr, "sector Function not implemented yet\n");
}

BOSS_API void setactivepage(int page)
{
	fprintf(stderr, "setactivepage Function not implemented yet\n");
}

BOSS_API void setallpalette(struct palettetype *palette)
{
	fprintf(stderr, "setallpalette Function not implemented yet\n");
}

BOSS_API void setaspectratio(int xasp, int yasp)
{
	fprintf(stderr, "setaspectratio Function not implemented yet\n");
}

BOSS_API unsigned setgraphbufsize(unsigned bufsize)
{
	fprintf(stderr, "setgraphbufsize Function not implemented yet\n");

	return 0;
}

BOSS_API void setgraphmode(int mode)
{
	fprintf(stderr, "setgraphmode Function not implemented yet\n");
}

BOSS_API void setlinestyle(int linestyle, unsigned upattern, int thickness)
{
	fprintf(stderr, "setlinestyle Function not implemented yet\n");
}

BOSS_API void setpalette(int colornum, int color)
{
	fprintf(stderr, "setpalette Function not implemented yet\n");
}

BOSS_API void setrgbpalette(int colornum, int red, int green, int blue)
{
	fprintf(stderr, "setrgbpalette Function not implemented yet\n");
}

BOSS_API void setusercharsize(int multx, int divx, int multy, int divy)
{
	fprintf(stderr, "setusercharsize Function not implemented yet\n");
}

BOSS_API void setviewport(int left, int top, int right, int bottom, int clip)
{
	fprintf(stderr, "setviewport Function not implemented yet\n");
}

BOSS_API void setvisualpage(int page)
{
	fprintf(stderr, "setvisualpage Function not implemented yet\n");
}

BOSS_API void setwritemode(int mode)
{
	fprintf(stderr, "setwritemodesetwritemodeFunction not implemented yet\n");
}

} // namespace Not_Implemented

} // namespace BGI

} // namespace Graphics

Keyboard keyboard = {0};

BOSS_API void KeyboardHardReset()
{
	keyboard.keyPressedInQueue = 0;
	keyboard.lastKeyDown = 0;
	keyboard.timerOverFlow = 0;
	keyboard.timerStart = 0;
}

// this function returns the ASCII value of the key, considering shift
// if the key is non-ASCII then -1 is returned
int RawGetASCII(const SDL_keysym& keysym)
{
//	fprintf(stderr, "Mod = %d\n", keysym.mod);

	bool bNumLockOn = ( (keysym.mod & KMOD_NUM) > 0);
//	if(bNumLockOn) {
//		fprintf(stderr, "We're Num Locked\n");
//	}

	// if our numlock is on, translate the num keypad
	if(bNumLockOn) {
		switch(keysym.sym) {
			case SDLK_KP0: return '0';
			case SDLK_KP1: return '1';
			case SDLK_KP2: return '2';
			case SDLK_KP3: return '3';
			case SDLK_KP4: return '4';
			case SDLK_KP5: return '5';
			case SDLK_KP6: return '6';
			case SDLK_KP7: return '7';
			case SDLK_KP8: return '8';
			case SDLK_KP9: return '9';
			default: break;
		} // switch
	} // if(bNumLockOn)

	// check other numpad keys
	switch(keysym.sym) {
		case SDLK_KP_PERIOD: return '.';
		case SDLK_KP_DIVIDE: return '/';
		case SDLK_KP_MULTIPLY: return '*';
		case SDLK_KP_MINUS: return '-';
		case SDLK_KP_PLUS: return '+';
		case SDLK_KP_ENTER: return SDLK_RETURN; // 13?
		case SDLK_KP_EQUALS: return '=';
		default: break;
	}

	// otherwise, only if it's a printable character...
	if(keysym.sym <= 127) {

		// if we got this far, it wasn't a special character
		// check if any shift modifiers or caps lock was down:
		bool bShift = ( (keysym.mod & KMOD_CAPS) > 0
					|| (keysym.mod & KMOD_RSHIFT) > 0
					|| (keysym.mod & KMOD_LSHIFT) > 0
					|| (keysym.mod & KMOD_SHIFT) > 0 );
//		if(bShift) {
//						fprintf(stderr, "We're shifted\n");
//		}
//		fprintf(stderr, "We have a letter '%c' that is # %d\n", keysym.sym,
//							keysym.sym);

		// if we're shifted
		if(bShift) {

			if(keysym.sym >= 'a' && keysym.sym <= 'z') {
//							fprintf(stderr, "We have a letter '%c'\n", event.key.keysym.sym);
				return keysym.sym - ('a' - 'A');
			}
			else {
				switch(keysym.sym) {
					case '`': return '~';
					case '1': return '!';
					case '2': return '@';
					case '3': return '#';
					case '4': return '$';
					case '5': return '%';
					case '6': return '^';
					case '7': return '&';
					case '8': return '*';
					case '9': return '(';
					case '0': return ')';
					case '-': return '_';
					case '=': return '+';
					case '[': return '{';
					case ']': return '}';
					case ';': return ':';
					case '\'': return '"'; // single quote, ASCII=39
					case ',': return '<';
					case '.': return '>';
					case '/': return '?';
					case '\\': return '|';
					default: return keysym.sym;
				} // switch(keysym.sym) 
			} // if not a letter
		} // if(bShift)
		else { // else, we're not shifted
			// then return the unshifted character...
			return keysym.sym;
		}
	} // if it's a printable character

	// else, return 0
	return -1;
}


namespace Conio {

BOSS_API void clrscr(void)
{
	fprintf(stderr, "clrscr function not implemented\n");
//	system("cls");
} // clrscr

BOSS_API void delay(int millisec)
{
	SDL_Delay((Uint32)millisec);
} // delay

// blocks until a printable key is pressed, returns the ASCII value of that
// key being pressed
BOSS_API int getch (void)
{
	// TO DO: remove once done testing
//	static int counter = 0;

	// For special keys, the getch function first returns ASCII 0. 
	// The next call will then return the special key 
	static int specialCharacter = 0;
	if(specialCharacter) {
		int retVal = specialCharacter;
		specialCharacter = 0;

		// I believe this is needed to flush out so that bioskey(0) won't 
		// return the same key twice
		keyboard.keyPressedInQueue = 0;
		return retVal;
	}

	// we now use bioskey(1) for getch() instead of SDL Key Event handling...
	//int bkey = 0;
	
//	SDL_Event event;
//	// if it was not a special character...let's empty our event queue:
//	// TO DO: I don't think this is necessary because the events are not 
//	// pumped in SDL unless you call SDL_PumpEvent, etc...
//	while(SDL_PollEvent(&event)) {
//
//		// Bug Fix:  if key up, clear the keyboard...	
//		if(event.type == SDL_KEYUP) {
//			fprintf(stderr, "Got a key up in getch()\n");
//			keyboard.lastKeyDown = 0;
//		} 
//
//	} // while(SDL_PollEvent), i.e. we still have events in there somewhere

	// spin until we get the next keydown keyboard event...
	while(true) {
		// wait indefinitely for a keypress
		while( bioskey(1) == 0) { }

//		SDL_WaitEvent(&event);

		// if it was a keydown event, we're in business
//		if(event.type == SDL_KEYDOWN) {

//				fprintf(stderr, "Key #%d pressed\n", event.key.keysym.sym);
//				fprintf(stderr, "Mod is %d\n", event.key.keysym.mod);
//				fprintf(stderr, "Scan code is %d\n", event.key.keysym.scancode);

			// handle special characters
		switch(keyboard.keyPressedInQueueKeySym.sym) {
			case SDLK_HOME:
				specialCharacter = KEY_HOME;
				return 0;
				break;
			case SDLK_UP:
				specialCharacter = KEY_UP;
				return 0;
				break;
			case SDLK_PAGEUP:
				specialCharacter = KEY_PGUP;
				return 0;
				break;
			case SDLK_LEFT:
				specialCharacter = KEY_LEFT;
				return 0;
				break;
			// no "CENTER"...
			case SDLK_RIGHT:
				specialCharacter = KEY_RIGHT;
				return 0;
				break;
			case SDLK_END:
				specialCharacter = KEY_END;
				return 0;
				break;
			case SDLK_DOWN:
				specialCharacter = KEY_DOWN;
				return 0;
				break;
			case SDLK_PAGEDOWN:
				specialCharacter = KEY_PGDN;
				return 0;
				break;
			case SDLK_INSERT:
				specialCharacter = KEY_INSERT;
				return 0;
				break;
			case SDLK_DELETE:
				specialCharacter = KEY_DELETE;
				return 0;
				break;
			case SDLK_F1:
				specialCharacter = KEY_F1;
				return 0;
				break;
			case SDLK_F2:
				specialCharacter = KEY_F2;
				return 0;
				break;
			case SDLK_F3:
				specialCharacter = KEY_F3;
				return 0;
				break;
			case SDLK_F4:
				specialCharacter = KEY_F4;
				return 0;
				break;
			case SDLK_F5:
				specialCharacter = KEY_F5;
				return 0;
				break;
			case SDLK_F6:
				specialCharacter = KEY_F6;
				return 0;
				break;
			case SDLK_F7:
				specialCharacter = KEY_F7;
				return 0;
				break;
			case SDLK_F8:
				specialCharacter = KEY_F8;
				return 0;
				break;
			case SDLK_F9:
				specialCharacter = KEY_F9;
				return 0;
				break;
			default:
				break;
		} // switch(keyboard.keyPressedInQueueSym) 

		// else, call RawGetASCII and get ASCII value
		int ascii = RawGetASCII(keyboard.keyPressedInQueueKeySym);
		
		// I believe this is needed to flush out so that bioskey(0) won't 
		// return the same key twice
		keyboard.keyPressedInQueue = 0;

//			fprintf(stderr, "Ascii value came out as %d\n", ascii);
		if(ascii != -1) {
			return ascii;
		}

		// else, it was a non-printable character, so keep spinning
//		} // if key down

//		++counter;
//		if(counter % 500000 == 0) {
//			fprintf(stderr, "spinning in getch()\n");
//		}
		// otherwise, keep spinning
	} // while(true)

	return 0;
} // getch

/// Performs a check of the keyboard buffer.
///
/// \return This returns non-zero if there is a key in the keyboard buffer, zero otherwise.
/// Use getch() to retrieve the key.
///
/// BOSS-specific:  This is identical to calling bioskey(1);
BOSS_API int kbhit(void)
{
	return Bios::bioskey(1);
}

} // namespace Conio

namespace Bios {

const int TIMER_KEYREPEAT = 250; // milliseconds
const int TIMER_FASTKEYREPEAT = 100; // milliseconds

BOSS_API int bioskey(int command)
{
	int nResult = 0;

	switch(command) {
		// command = 00h 
		// Returns the next key pressed. The value returned is a combination 
		// of the key's scan code in the high 8 bits and its ASCII code in the 
		// low 8 bits. For non-alphanumeric keys, such as the arrow keys, the 
		// low 8 bits are zeroed.  This is "almost" like the behavior of getch()
		// but without all the fancy handling of special keys.
		case 0x00:
		{

			// spin until we get the next keydown keyboard event...
//			bool bKeepSpinning = true;
//			while(bKeepSpinning) {
			if(keyboard.keyPressedInQueue) {
				nResult = keyboard.keyPressedInQueue;
			}
			else {
				while( (nResult = bioskey(1)) == 0) { 
					// spin...
				}
			}
			// I believe this is needed to flush out so that bioskey(0) won't 
			// return the same key twice
			keyboard.keyPressedInQueue = 0;

			break;
		} // end command 00h

		// command = 01h 
		// Checks the keyboard, returns zero if no key pressed, else the key. 
		// Does not dequeue the key from the keyboard buffer. The value returned 
		// when a key was pressed is a combination of the key's scan code in the 
		// high 8 bits and either its ASCII code or zero in the low 8 bits. If the 
		// Ctrl-BREAK key was pressed, returns -1. 
		case 0x01:
		{
			// now if a previous call to bioskey(1) has returned a key
			// but we haven't called bioskey(0) or getch() yet, then
			// simply return this result
			if(keyboard.keyPressedInQueue) {
				return keyboard.keyPressedInQueue;
			}

			// else, let's check the SDL event queue

			// pump any pending events and put them in the queue
			SDL_PumpEvents();
			
			// our peeked event...
			SDL_Event peekedEvent;

			// now peek into the queue and determine if they are any
			// key-down events...
			int err = 0;
//			fprintf(stderr, "Same call to bioskey(1), lastKeyDown = %d\n", lastKeyDown);
			while( (err = SDL_PeepEvents( &peekedEvent, 1, 
							SDL_GETEVENT, 
							SDL_EVENTMASK(SDL_KEYDOWN) |
							SDL_EVENTMASK(SDL_KEYUP) ) ) != 0) 
			{

//				fprintf(stderr, "err = %d\n", err);
				// if it was an error, report it
				if(err  == -1) {
					fprintf(stderr, "SDL_PeepEvents returned an error in bioskey(1): %s\n", SDL_GetError());
					nResult = 0;
				}
				// else if we have a keydown/keyup event in our queue, get the event data
				else if(err > 0) {
					if(peekedEvent.type == SDL_KEYDOWN) {
						// get scancode and put in high 8 bits
						keyboard.lastKeyDown = ((unsigned char)peekedEvent.key.keysym.scancode << 8);
						int rawascii = (int)RawGetASCII(peekedEvent.key.keysym);
						if(rawascii >= 0) {
							keyboard.lastKeyDown |= (unsigned char)(rawascii);
						}
#if defined(_DEBUG)
						fprintf(stderr, "lastKeyDown=%d\n", keyboard.lastKeyDown);
#endif
						keyboard.keyPressedInQueueKeySym = peekedEvent.key.keysym;

						// we keep track of the last key down and (re)start the timer
						nResult = keyboard.lastKeyDown;
						keyboard.timerStart = SDL_GetTicks();
						keyboard.timerOverFlow = TIMER_KEYREPEAT;

//						fprintf(stderr, "Got a KEY DOWN = %d, nResult = %d\n", 
//								RawGetASCII(peekedEvent.key.keysym), nResult);

					} // if(event.type == SDL_KEYDOWN)
					else if(peekedEvent.type == SDL_KEYUP) {
						// if the key that just went up was the last key down, 
						// then we can reset our keyboard state...
						if(peekedEvent.key.keysym.scancode == ((keyboard.lastKeyDown >> 8) & 0xFF)) {
							keyboard.lastKeyDown = 0;
						}

//						fprintf(stderr, "Got a KEY UP = %d, nResult = %d\n", 
//								RawGetASCII(peekedEvent.key.keysym), nResult);
//						}
					} // else if(event.type == SDL_KEYUP)

				} // else if(err > 0)
			} // while(we have key events)

//				else {
	//				fprintf(stderr, "1, lastKeyDown = %d\n", (lastKeyDown & 0xFF));
					// check if there is any key still down
			if(keyboard.lastKeyDown) {
	//					fprintf(stderr, "2, timerStart is %ld, now is %ld\n", timerStart, SDL_GetTicks());
				// compare our timer start value with current time 
				if(SDL_GetTicks() - keyboard.timerStart > keyboard.timerOverFlow) {
//					fprintf(stderr, "Key repeat (%d)\n", (lastKeyDown & 0xFF));
					nResult = keyboard.lastKeyDown;
					keyboard.timerStart = SDL_GetTicks();
					keyboard.timerOverFlow = TIMER_FASTKEYREPEAT;

				 } // if timer is tripped
			} // if(lastKeyDown)

			// after all is said and done, the "contract" for bioskey(1) cannot 
			// dequeue the key from the buffer, so we cheat a lil:
			keyboard.keyPressedInQueue = nResult;

			break;
		} // end command 01h

		// command = 02h 
		// Returns the keyboard shift state: 
		// 7654 3210  Meaning
		// ---- ---X  Right shift key down
		// ---- --X-  Left shift key down
		// ---- -X--  Ctrl key down
		// ---- X---  Alt key down
		// ---X ----  Scroll lock on
		// --X- ----  Num lock on
		// -X-- ----  Caps lock on
		// X--- ----  Insert on
		case 0x02:
		{
			int mods = SDL_GetModState();
			if( (mods & KMOD_RSHIFT) > 0) {
				nResult |= 0x1;
			}
			if( (mods & KMOD_LSHIFT) > 0) {
				nResult |= 0x2;
			}
			if( (mods & KMOD_LCTRL) > 0 || (mods & KMOD_RCTRL) > 0) {
				nResult |= 0x4;
			}
			if( (mods & KMOD_LALT) > 0 || (mods & KMOD_RALT) > 0) {
				nResult |= 0x8;
			}
			// cannot support SCROLL LOCK .... :(
//			if( (mods & KMOD_SCROLLLOCK) > 0) {
//				nResult |= 0x10;
//			}
			if( (mods & KMOD_NUM) > 0) {
				nResult |= 0x20;
			}
			if( (mods & KMOD_CAPS) > 0) {
				nResult |= 0x40;
			}
			// cannot support INSERT ... :(
//			if( (mods & KMOD_INSERT) > 0) {
//				nResult |= 0x80;
//			}

			break;
		} // end command 02h

		// command = 12h 
		// Returns the two status bytes of the enhanced keyboard, packed in the 
		// low 16 bits of the return value. The individual bits of the return value 
		// are defined in the following table: 
		// FEDC BA98  7654 3210  Meaning
		// ---- ----  ---- ---X  Right SHIFT is pressed
		// ---- ----  ---- --X-  Left SHIFT is pressed
		// ---- ----  ---- -X--  CTRL is pressed
		// ---- ----  ---- X---  ALT is pressed
		// ---- ----  ---X ----  Scroll Lock locked
		// ---- ----  --X- ----  Num Lock locked
		// ---- ----  -X-- ----  Caps Lock locked
		// ---- ----  X--- ----  Insert locked
		// 
		// ---- ---X  ---- ----  Left CTRL is pressed
		// ---- --X-  ---- ----  Left ALT is pressed
		// ---- -X--  ---- ----  Right CTRL is pressed
		// ---- X---  ---- ----  Right ALT is pressed
		// ---X ----  ---- ----  Scroll Lock is pressed
		// --X- ----  ---- ----  Num Lock is pressed
		// -X-- ----  ---- ----  Caps Lock is pressed
		// X--- ----  ---- ----  SysReq is pressed
		case 0x12:
		{
			int mods = SDL_GetModState();
			if( (mods & KMOD_RSHIFT) > 0) {
				nResult |= 0x1;
			}
			if( (mods & KMOD_LSHIFT) > 0) {
				nResult |= 0x2;
			}
			if( (mods & KMOD_LCTRL) > 0 || (mods & KMOD_RCTRL) > 0) {
				nResult |= 0x4;
			}
			if( (mods & KMOD_LALT) > 0 || (mods & KMOD_RALT) > 0) {
				nResult |= 0x8;
			}
			// cannot support SCROLL LOCK .... :(
//			if( (mods & KMOD_SCROLLLOCK) > 0) {
//				nResult |= 0x10;
//			}
			if( (mods & KMOD_NUM) > 0) {
				nResult |= 0x20;
			}
			if( (mods & KMOD_CAPS) > 0) {
				nResult |= 0x40;
			}
			// cannot support INSERT ... :(
//			if( (mods & KMOD_INSERT) > 0) {
//				nResult |= 0x80;
//			}
			if( (mods & KMOD_LCTRL) > 0 ) {
				nResult |= 0x100;
			}
			if( (mods & KMOD_LALT) > 0 ) {
				nResult |= 0x200;
			}
			if( (mods & KMOD_RCTRL) > 0 ) {
				nResult |= 0x400;
			}
			if( (mods & KMOD_RALT) > 0 ) {
				nResult |= 0x800;
			}
			// cannot support SCROLL LOCK .... :(
//			if( (mods & KMOD_SCROLLLOCK) > 0) {
//				nResult |= 0x1000;
//			}
			if( (mods & KMOD_NUM) > 0) {
				nResult |= 0x2000;
			}
			if( (mods & KMOD_CAPS) > 0) {
				nResult |= 0x4000;
			}
			// cannot support SYSREQ ... :(
//			if( (mods & KMOD_SYSREQ) > 0) {
//				nResult |= 0x8000;
//			}

			break;
		} // end command 12h

		// unsupported modes
		case 0x10:
		{
		} // end command 10h, fall-through

		case 0x11:
		{
		} // end command 11h, fall-through
		default:
			fprintf(stderr, "bioskey(%d) function not implemented yet\n", command);
			break;
	} // switch(command)

	return nResult;
} // bioskey

} // namespace Bios

namespace Audio {

const int DEFAULT_MIXING_CHANNELS = 16;

struct AudioVars {
	bool bInitialized;
	int numberMixingChannels;
	int musicStatus;
};

AudioVars aVars = {0};

void initAudio()
{
	if(aVars.bInitialized) { return; }

	if ( SDL_Init( SDL_INIT_AUDIO ) == -1 ) {
		fprintf(stderr, "Could not initialize SDL Audio libraries, Error=%s\n", SDL_GetError());
		return;
	} // if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )

	/// current wav files are 44 kHz, 8-bit, 1 channel...
	if( Mix_OpenAudio(44100, AUDIO_S8, 1, 8192) == -1 ) {
		fprintf(stderr, "Could not Open the SDL_Mixer, Error=%s\n", SDL_GetError());
		return;
	}
	atexit(Mix_CloseAudio);

	// set up a total of 16 possible mixing channels (i.e. no more than 16
	// sounds can be playing at one time)
	aVars.numberMixingChannels = Mix_AllocateChannels(DEFAULT_MIXING_CHANNELS);

	// set up the volume for all the channels to be max
	Mix_Volume(-1,MIX_MAX_VOLUME);

	aVars.bInitialized = true;
}

/// This function queues up a WAV file to be played and stores
/// it in the sound pointer.  
/// NOTE:  The filename MUST be a WAV file, not a VOC file.
/// 
/// The sound pointer cannot be NULL.
///
/// If translate_useless is non-zero, then Sound_Translate is called
/// (which does nothing).
///
/// It returns non-zero if success and zero if failure
BOSS_API int Sound_Load(const char *filename, _sound* the_sound, int translate_useless)
{
	if(!aVars.bInitialized) { initAudio(); }

	assert(the_sound);
//	fprintf(stderr, "Sound_Load('%s', ...)\n", filename);

	// if the sound to be filled in has a chunk already loaded, free it
	if(the_sound->pChunk) {
		// if currently playing, stop it
		if(Mix_Playing(the_sound->mixChannelNum)) {
			Mix_HaltChannel(the_sound->mixChannelNum);
		}

		Mix_FreeChunk(the_sound->pChunk);
		the_sound->pChunk = NULL;
	}

	// now that the Audio Mixer is setup, we load the WAV file...
	the_sound->pChunk = Mix_LoadWAV(filename);
	if(! (the_sound->pChunk)) {
		fprintf(stderr, "Mix_LoadWAV: %s\n", Mix_GetError());
		return 0;
	}

	// this sets up the wav information and buffer into the sound structure
//	if( SDL_LoadWAV( filename, &(sound->wav_spec), &(sound->wav_buffer), &(sound->wav_length) ) == NULL) {
//		fprintf(stderr, "Could not load '%s' file\n", filename);
//		return 0;
//	}

	if(translate_useless) {
		Sound_Translate(the_sound);
	}

	return 1;

//	fprintf(stderr, "Sound_Load Function not implemented yet\n");
//	return 0;
}

/// This function unloads the chunk from the given sound
BOSS_API void Sound_Unload(_sound* the_sound)
{
	if(!aVars.bInitialized) { initAudio(); }
	assert(the_sound);
	if(the_sound->pChunk) {
		// if the sound is currently playing, stop it
		if(Mix_Playing(the_sound->mixChannelNum)) {
			Mix_HaltChannel(the_sound->mixChannelNum);
		}
		Mix_FreeChunk(the_sound->pChunk);
		the_sound->pChunk = NULL;
	}
//	assert(the_sound);
//	assert(the_sound->wav_buffer);
//
//	SDL_PauseAudio(1);
//	SDL_FreeWAV(the_sound->wav_buffer);
//	fprintf(stderr, "Sound_Unload Function not implemented yet\n");
}

/// This function doesn't do anything anymore, the sound data
/// is already translated for the sound card.
BOSS_API void Sound_Translate(_sound* the_sound)
{
	if(!aVars.bInitialized) { initAudio(); }
	assert(the_sound);
//	fprintf(stderr, "Sound_Translate Function not implemented yet\n");
}

// This function plays the listed sound and returns the channel
// it is playing on, or -1 otherwise
BOSS_API int Sound_Play(_sound* the_sound, int repeat, int volume)
{
	if(!aVars.bInitialized) { initAudio(); }
	assert(the_sound);

	// if the chunk for this sound hasn't been allocated, then 
	// print an error and do nothing
	if(!the_sound->pChunk) {
		fprintf(stderr, "sound chunk pointer not set in Sound_Play()\n");
		return -1;
	}

	// now we find a non-playing mixing channel and play the sound on that channel
	// if we cannot find a mixing channel, then we simply allocate another channel
	// (this effectively means we will always be able to play a sound)
	int mix_chan = 0;
	for(mix_chan = 0; mix_chan < aVars.numberMixingChannels; ++mix_chan) {
		if(Mix_Playing(mix_chan) == 0) {
			break;
		}
	}

	if(mix_chan == aVars.numberMixingChannels) {
		aVars.numberMixingChannels = Mix_AllocateChannels(aVars.numberMixingChannels + 1);
	}

	// now we have a mixing channel to play the sound once (no loops)
	Mix_VolumeChunk(the_sound->pChunk,volume);
	if( Mix_PlayChannel(mix_chan, the_sound->pChunk, repeat) == -1 ) {
		fprintf(stderr, "Mix_PlayChannel() returned an error = %s\n", Mix_GetError());
	}
	else {
		the_sound->mixChannelNum = mix_chan;
	}

	return mix_chan;
}

/// This function returns either SOUND_PLAYING if ANY sound is playing
/// or SOUND_STOPPED.  Since we're using SDL_Mixer, we don't need to 
/// every worry about if the sound is playing or not...calls to this
/// function are probably obsolete and should be removed.
///
/// \param channel The channel to return status on.  By default channel is -1
/// which returns the status after checking all channels.
///
/// \return SOUND_PLAYING if a sound is playing.  SOUND_STOPPED otherwise.
BOSS_API int Sound_Status(int channel)
{
	if(!aVars.bInitialized) { initAudio(); }
	if(channel == -1) {
		for(int mix_chan = 0; mix_chan < aVars.numberMixingChannels; ++mix_chan) {
			if(Mix_Playing(mix_chan)) {
				return SOUND_PLAYING;
			}
		}
		return SOUND_STOPPED;
	}

	// else, check the specific channel
	if(Mix_Playing(channel)) {
		return SOUND_PLAYING;
	}
	return SOUND_STOPPED;
}

/// By default, This function stops all sounds on all mixing channels
BOSS_API void Sound_Stop(int channel)
{
	if(!aVars.bInitialized) { initAudio(); }
	// stop all mix channels immediately
	Mix_HaltChannel(channel);
}

// Done SOUND Functions

// returns 0 if error, 1 if success
BOSS_API int Music_Load(const char *filename, _music* the_music)
{
	if(!aVars.bInitialized) { initAudio(); }

	if(!the_music) {
		fprintf(stderr, "Music System - music pointer was NULL in Music_Load\n");
		return 0;
	}

	the_music->pMusic = Mix_LoadMUS(filename);

	if(!the_music->pMusic) {
		fprintf(stderr, "Music System - Couldn't open %s\n",filename);
		return 0;
	}

	return Music_Register(the_music);
}


BOSS_API void Music_Unload(_music* the_music)
{
	if(!aVars.bInitialized) { initAudio(); }

	if(the_music && the_music->pMusic) {
		Mix_FreeMusic(the_music->pMusic);
	}
}

/// This function doesn't do anything anymore
BOSS_API int Music_Register(_music* the_music)
{
	if(!aVars.bInitialized) { initAudio(); }
	return 1;
}

BOSS_API void Music_Stop(void)
{
	if(!aVars.bInitialized) { initAudio(); }
	if(Mix_PlayingMusic()) {
		aVars.musicStatus = SEQUENCE_STOPPED;
	}
	Mix_HaltMusic();
//	fprintf(stderr, "Music_Stop Function not implemented yet\n");
}

/// sequence has no meaning here
/// returns 0 if unavailable, 1 if successful
BOSS_API int Music_Play(_music* the_music,int sequence, int loops, int volume)
{
	if(!aVars.bInitialized) { initAudio(); }
	if(!the_music || !the_music->pMusic) {
		fprintf(stderr, "Music_Play Function called with NULL music pointer\n");
		return 0;
	}

	Mix_VolumeMusic(volume);
	if( (Mix_PlayMusic(the_music->pMusic, loops)) != 0) {
		fprintf(stderr, "Music_Play returned an error: %s\n", Mix_GetError());
		aVars.musicStatus = SEQUENCE_UNAVAILABLE;
		return 0;
	}

	aVars.musicStatus = SEQUENCE_PLAYING;
	return 1;
}

BOSS_API void Music_Resume(void)
{
	if(!aVars.bInitialized) { initAudio(); }
	Mix_ResumeMusic();
	if(Mix_PlayingMusic()) {
		aVars.musicStatus = SEQUENCE_PLAYING;
	}
	else {
		aVars.musicStatus = SEQUENCE_COMPLETE;
	}
//	fprintf(stderr, "Music_Resume Function not implemented yet\n");
}

BOSS_API int Music_Status(void)
{
	if(!aVars.bInitialized) { initAudio(); }
	// if we think it's playing, check to make sure it's not complete
	if(aVars.musicStatus == SEQUENCE_PLAYING) {
		if(!Mix_PlayingMusic()) {
			aVars.musicStatus = SEQUENCE_COMPLETE;
		}
	}

	return aVars.musicStatus;
//	fprintf(stderr, "Music_Status Function not implemented yet\n");
//	return 0;
}

BOSS_API void nosound()
{
	fprintf(stderr, "nosound Function not implemented yet\n");
}

BOSS_API void sound(int)
{
	fprintf(stderr, "sound Function not implemented yet\n");
}

} // namespace Audio

namespace Random {

BOSS_API void randomize()
{
	srand( (unsigned)time( NULL ) );
}

BOSS_API int random(int x)
{
	return (rand() % x);
}

} // namespace Random 

namespace Mouse {

	MouseVars mVars;

	/// This is a substitute for INT 33h, Function 00h
	/// This tells SDL to ignore the mouse events, callers use
	/// GetMouseState to retrieve the state of the mouse.  
	BOSS_API void InitializeMouse(bool bInit)
	{
		SDL_EventState(SDL_MOUSEMOTION, (bInit ? SDL_IGNORE : SDL_ENABLE));
		SDL_EventState(SDL_MOUSEBUTTONDOWN, (bInit ? SDL_IGNORE : SDL_ENABLE));
		SDL_EventState(SDL_MOUSEBUTTONUP, (bInit ? SDL_IGNORE : SDL_ENABLE));
	}

	/// This is a substitute for INT 33h, Function 01h
	BOSS_API void EnableMouseCursor()
	{
		SDL_ShowCursor(SDL_ENABLE);
	}

	/// This is a substitute for INT 33h, Function 02h
	BOSS_API void DisableMouseCursor()
	{
		SDL_ShowCursor(SDL_DISABLE);
	}

	BOSS_API Uint8 GetMouseState(int* x, int* y)
	{
		Uint8 b = 0;

		SDL_PumpEvents();
		Uint8 butts = SDL_GetMouseState(x,y);
	
		// need to swap middle and right buttons here...
		if(SDL_BUTTON(1) & butts) b |= 1;
		if(SDL_BUTTON(3) & butts) b |= 2;
		if(SDL_BUTTON(2) & butts) b |= 4;

		return b;
	}

} // namespace Mouse

} // namespace Boss

using namespace Boss::Graphics::BGI;
using namespace Boss::Graphics::Raw;
using namespace Boss::Graphics::BGI::Not_Implemented;
using namespace Boss::Graphics;
using namespace Boss::Conio;
using namespace Boss::Bios;
using namespace Boss::Audio;
using namespace Boss::Random;
using namespace Boss::Mouse;
using namespace Boss;

