/* ---------------------------------------------------------------------- *
 * common.h
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * ---------------------------------------------------------------------- */
#ifndef __common_h__
#define __common_h__

#ifdef LC_X11
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include "lin-city.h"
#include "lcx11.h"

#elif defined LC_SDL
#include <SDL.h>
#include "lcsdl.h"

#endif

#endif	/* __common_h__ */
