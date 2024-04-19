//  $Id$
//
//  SuperTux -  A Jump'n Run
//  Copyright (C) 2004 Michael George <mike@georgetech.com>
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

#include <stdlib.h>
#include <string>
#include "configfile.h"
#include "setup.h"
#include "globals.h"
#include "lispreader.h"
#include "player.h"

#ifdef WIN32
const char * config_filename = "/st_config.dat";
#else
const char * config_filename = "/config";
#endif

static void defaults ()
{
  /* Set defaults: */
  Globals::debug_mode = false;

  Globals::use_fullscreen = false;
  Globals::show_fps = false;

#ifndef NOSOUND
  audio_device = true;
  use_sound = true;
  use_music = true;
#else
  bool audio_device = false;
  bool use_sound = false;
  bool use_music = false;
#endif
}

void loadconfig(void)
{
  FILE * file = NULL;

  defaults();

  /* override defaults from config file */

  file = opendata(config_filename, "r");

  if (file == NULL)
    return;

  /* read config file */

  lisp_stream_t   stream;
  lisp_object_t * root_obj = NULL;

  lisp_stream_init_file (&stream, file);
  root_obj = lisp_read (&stream);

  if (root_obj->type == LISP_TYPE_EOF || root_obj->type == LISP_TYPE_PARSE_ERROR)
    return;

  if (strcmp(lisp_symbol(lisp_car(root_obj)), "supertux-config") != 0)
    return;

  LispReader reader(lisp_cdr(root_obj));

  reader.read_bool("fullscreen", &Globals::use_fullscreen);
#ifndef NOSOUND
  reader.read_bool("sound",      &use_sound);
  reader.read_bool("music",      &use_music);
#endif
  reader.read_bool("Globals::show_fps",   &Globals::show_fps);

  reader.read_int ("joystick", &Globals::joystick_num);
  if (!(Globals::joystick_num >= 0))
    Globals::use_joystick = false;
  else
    Globals::use_joystick = true;

  reader.read_int ("joystick-x", &Globals::joystick_keymap.x_axis);
  reader.read_int ("joystick-y", &Globals::joystick_keymap.y_axis);
  reader.read_int ("joystick-a", &Globals::joystick_keymap.a_button);
  reader.read_int ("joystick-b", &Globals::joystick_keymap.b_button);
  reader.read_int ("joystick-start", &Globals::joystick_keymap.start_button);
  reader.read_int ("joystick-deadzone", &Globals::joystick_keymap.dead_zone);

  reader.read_int ("keyboard-jump", &keymap.jump);
  reader.read_int ("keyboard-duck", &keymap.duck);
  reader.read_int ("keyboard-left", &keymap.left);
  reader.read_int ("keyboard-right", &keymap.right);
  reader.read_int ("keyboard-fire", &keymap.fire);

  lisp_free(root_obj);
  fclose(file);
}

void saveconfig (void)
{
  /* write settings to config file */

  FILE * config = opendata(config_filename, "w");

  if(config)
    {
      fprintf(config, "(supertux-config\n");
      fprintf(config, "\t;; the following options can be set to #t or #f:\n");
      fprintf(config, "\t(fullscreen %s)\n", Globals::use_fullscreen ? "#t" : "#f");
#ifndef NOSOUND
	  fprintf(config, "\t(sound      %s)\n", use_sound      ? "#t" : "#f");
      fprintf(config, "\t(music      %s)\n", use_music      ? "#t" : "#f");
#else
	  fprintf(config, "\t(sound      %s)\n", "#f");
      fprintf(config, "\t(music      %s)\n", "#f");
#endif
      fprintf(config, "\t(Globals::show_fps   %s)\n", Globals::show_fps       ? "#t" : "#f");

      fprintf(config, "\n\t;; either \"opengl\" or \"sdl\"\n");
      //fprintf(config, "\t(video      \"%s\")\n", "sdl");

      fprintf(config, "\n\t;; joystick number (-1 means no joystick):\n");
      fprintf(config, "\t(joystick   %d)\n", Globals::use_joystick ? Globals::joystick_num : -1);

      fprintf(config, "\t(joystick-x   %d)\n", Globals::joystick_keymap.x_axis);
      fprintf(config, "\t(joystick-y   %d)\n", Globals::joystick_keymap.y_axis);
      fprintf(config, "\t(joystick-a   %d)\n", Globals::joystick_keymap.a_button);
      fprintf(config, "\t(joystick-b   %d)\n", Globals::joystick_keymap.b_button);
      fprintf(config, "\t(joystick-start  %d)\n", Globals::joystick_keymap.start_button);
      fprintf(config, "\t(joystick-deadzone  %d)\n", Globals::joystick_keymap.dead_zone);

      fprintf(config, "\t(keyboard-jump  %d)\n", keymap.jump);
      fprintf(config, "\t(keyboard-duck  %d)\n", keymap.duck);
      fprintf(config, "\t(keyboard-left  %d)\n", keymap.left);
      fprintf(config, "\t(keyboard-right %d)\n", keymap.right);
      fprintf(config, "\t(keyboard-fire  %d)\n", keymap.fire);

      fprintf(config, ")\n");

      fclose(config);
    }

}

/* EOF */
