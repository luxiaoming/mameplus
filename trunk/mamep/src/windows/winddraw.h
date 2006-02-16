//============================================================
//
//  winddraw.h - Win32 DirectDraw code
//
//  Copyright (c) 1996-2006, Nicola Salmoria and the MAME Team.
//  Visit http://mamedev.org for licensing and usage restrictions.
//
//============================================================

#ifndef __WIN32_DDRAW__
#define __WIN32_DDRAW__

#include "window.h"


//============================================================
//  PROTOTYPES
//============================================================

int win_ddraw_init(int width, int height, int depth, int attributes, const win_effect_data *effect);
void win_ddraw_kill(void);
int win_ddraw_draw(mame_bitmap *bitmap, const rectangle *bounds, void *vector_dirty_pixels, int update);
void win_ddraw_fullscreen_margins(DWORD desc_width, DWORD desc_height, RECT *margins);



#endif
