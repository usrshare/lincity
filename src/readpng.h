/* ---------------------------------------------------------------------- *
 * readpng.c
 * This file is part of lincity (see COPYRIGHT for copyright information).
 * ---------------------------------------------------------------------- */
#ifndef __readpng_h__
#define __readpng_h__
#include <stdint.h>

int load_png_graphics (char *txt_filename, char *png_filename, uint32_t* out_palette);

#endif /* __readpng_h__ */
