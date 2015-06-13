/* ---------------------------------------------------------------------- *
 * lcconfig.h
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * ---------------------------------------------------------------------- */
#ifndef __lcconfig_h__
#define __lcconfig_h__

#include <stdint.h>

#if defined (HAVE_CONFIG_H)
#include "config.h"
#elif defined (WIN32)
#include "confw32.h"
#endif


typedef int32_t Int32;

typedef int16_t Int16;

#endif	/* __lcconfig_h__ */
