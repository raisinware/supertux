//  $Id$
//
//  SuperTux -  A Jump'n Run
//  Copyright (C) 2000 Bill Kendrick <bill@newbreedsoftware.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>

#ifndef WIN32
#include <sys/types.h>
#include <ctype.h>
#endif

#include "defines.h"
#include "globals.h"
#include "screen.h"
#include "setup.h"
#include "type.h"

/* Needed for line calculations */
#define SGN(x) ((x)>0 ? 1 : ((x)==0 ? 0:(-1)))
#define ABS(x) ((x)>0 ? (x) : (-x))

/* --- CLEAR Globals::screen --- */

void clearscreen(int r, int g, int b)
{

    SDL_FillRect(Globals::screen, NULL, SDL_MapRGB(Globals::screen->format, r, g, b));
}

/* --- DRAWS A VERTICAL GRADIENT --- */

void drawgradient(Color top_clr, Color bot_clr)
{

    for(float y = 0; y < 480; y += 2)
      fillrect(0, (int)y, 640, 2,
                     (int)(((float)(top_clr.red-bot_clr.red)/(0-480)) * y + top_clr.red),
                     (int)(((float)(top_clr.green-bot_clr.green)/(0-480)) * y + top_clr.green),
                     (int)(((float)(top_clr.blue-bot_clr.blue)/(0-480)) * y + top_clr.blue), 255);
/* calculates the color for each line, based in the generic equation for functions: y = mx + b */

}

/* --- FADE IN --- */

/** Fades the given surface into a black one. If fade_out is true, it will fade out, else
it will fade in */

void fade(Surface *surface, int seconds, bool fade_out);

void fade(const std::string& surface, int seconds, bool fade_out)
{
Surface* sur = new Surface(Globals::datadir + surface, IGNORE_ALPHA);
fade(sur, seconds, fade_out);
delete sur;
}

void fade(Surface *surface, int seconds, bool fade_out)
{
  float alpha;
  if (fade_out)
    alpha = 0;
  else
    alpha = 255;

  int cur_time, old_time;
  cur_time = SDL_GetTicks();

  while(alpha >= 0 && alpha < 256)
    {
    surface->draw(0,0,(int)alpha);
    flipscreen();

    old_time = cur_time;
    cur_time = SDL_GetTicks();

    /* Calculate the next alpha value */
    float calc = (float) ((cur_time - old_time) / seconds);
    if(fade_out)
      alpha += 255 * calc;
    else
      alpha -= 255 * calc;
    }
}

/* 'Stolen' from the SDL documentation.
 * Set the pixel at (x, y) to the given value
 * NOTE: The surface must be locked before calling this!
 */
void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
  int bpp = surface->format->BytesPerPixel;
  /* Here p is the address to the pixel we want to set */
  Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

  switch(bpp)
    {
    case 1:
      *p = pixel;
      break;

    case 2:
      *(Uint16 *)p = pixel;
      break;

    case 3:
      if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
          p[0] = (pixel >> 16) & 0xff;
          p[1] = (pixel >> 8) & 0xff;
          p[2] = pixel & 0xff;
        }
      else
        {
          p[0] = pixel & 0xff;
          p[1] = (pixel >> 8) & 0xff;
          p[2] = (pixel >> 16) & 0xff;
        }
      break;

    case 4:
      *(Uint32 *)p = pixel;
      break;
    }
}

/* Draw a single pixel on the Globals::screen. */
void drawpixel(int x, int y, Uint32 pixel)
{
  /* Lock the Globals::screen for direct access to the pixels */
  if ( SDL_MUSTLOCK(Globals::screen) )
    {
      if ( SDL_LockSurface(Globals::screen) < 0 )
        {
          fprintf(stderr, "Can't lock Globals::screen: %s\n", SDL_GetError());
          return;
        }
    }

  if(!(x < 0 || y < 0 || x > Globals::screen->w || y > Globals::screen->h))
    putpixel(Globals::screen, x, y, pixel);

  if ( SDL_MUSTLOCK(Globals::screen) )
    {
      SDL_UnlockSurface(Globals::screen);
    }
  /* Update just the part of the display that we've changed */
#ifndef RES320X240
  update_rect(Globals::screen, x, y, 1, 1);
#else
  update_rect(Globals::screen, x/2, y/2, 1, 1);
#endif
}

void drawline(int x1, int y1, int x2, int y2, int r, int g, int b, int a)
{
#ifdef RES320X240
 x1=x1/2;
 x2=x2/2;
 y1=y1/2;
 y2=y2/2;
#endif


      /* Basic unantialiased Bresenham line algorithm */
      int lg_delta, sh_delta, cycle, lg_step, sh_step;
      Uint32 color = SDL_MapRGBA(Globals::screen->format, r, g, b, a);

      lg_delta = x2 - x1;
      sh_delta = y2 - y1;
      lg_step = SGN(lg_delta);
      lg_delta = ABS(lg_delta);
      sh_step = SGN(sh_delta);
      sh_delta = ABS(sh_delta);
      if (sh_delta < lg_delta)
        {
          cycle = lg_delta >> 1;
          while (x1 != x2)
            {
              drawpixel(x1, y1, color);
              cycle += sh_delta;
              if (cycle > lg_delta)
                {
                  cycle -= lg_delta;
                  y1 += sh_step;
                }
              x1 += lg_step;
            }
          drawpixel(x1, y1, color);
        }
      cycle = sh_delta >> 1;
      while (y1 != y2)
        {
          drawpixel(x1, y1, color);
          cycle += lg_delta;
          if (cycle > sh_delta)
            {
              cycle -= sh_delta;
              x1 += lg_step;
            }
          y1 += sh_step;
        }
      drawpixel(x1, y1, color);
}

/* --- FILL A RECT --- */

void fillrect(float x, float y, float w, float h, int r, int g, int b, int a)
{
if(w < 0)
	{
	x += w;
	w = -w;
	}
if(h < 0)
	{
	y += h;
	h = -h;
	}
	
#ifdef RES320X240
    x=x;
    y=y/2;
    w=w/2;
    h=h/2;
#endif

      SDL_Rect src, rect;
      SDL_Surface *temp = NULL;

      rect.x = (int)x;
      rect.y = (int)y;
      rect.w = (int)w;
      rect.h = (int)h;

      if(a != 255)
        {
          temp = SDL_CreateRGBSurface(0, rect.w, rect.h, Globals::screen->format->BitsPerPixel,
                                      Globals::screen->format->Rmask,
                                      Globals::screen->format->Gmask,
                                      Globals::screen->format->Bmask,
                                      Globals::screen->format->Amask);


          src.x = 0;
          src.y = 0;
          src.w = rect.w;
          src.h = rect.h;

          SDL_FillRect(temp, &src, SDL_MapRGB(Globals::screen->format, r, g, b));

          SDL_UpperBlit(temp,0,Globals::screen,&rect);

          SDL_FreeSurface(temp);
        }
      else
        SDL_FillRect(Globals::screen, &rect, SDL_MapRGB(Globals::screen->format, r, g, b));

}


/* --- UPDATE Globals::screen --- */

void updatescreen(void)
{
  update_rect(Globals::screen, 0, 0, Globals::screen->w, Globals::screen->h);
}

void flipscreen(void)
{
  SDL_UpdateWindowSurface(Globals::window);
}

void fadeout()
{
  clearscreen(0, 0, 0);
  Globals::white_text->draw_align("Loading...", Globals::screen->w/2, Globals::screen->h/2, A_HMIDDLE, A_TOP);
  flipscreen();
}

void update_rect(SDL_Surface *scr, Sint32 x, Sint32 y, Sint32 w, Sint32 h)
{
  SDL_Rect rect = {
    .x = x,
    .y = y,
    .w = w,
    .h = h
  };
  SDL_UpdateWindowSurfaceRects(Globals::window, &rect, 1);
}

