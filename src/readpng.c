/* ---------------------------------------------------------------------- *
 * readpng.c
 * This file is part of lincity (see COPYRIGHT for copyright information).
 * ---------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include "png.h"
#include "malloc.h"
#include "lin-city.h"
#include "lctypes.h"
#include "lintypes.h"
#include <stdint.h>

/* Read a PNG file.  You may want to return an error code if the read
 * fails (depending upon the failure).  There are two "prototypes" given
 * here - one where we are given the filename, and we need to open the
 * file, and the other where we are given an open file (possibly with
 * some or all of the magic bytes read - see comments above).
 */
#define ERROR -1
#define OK 0

/* Let errors and warnings be handled by setjmp/longjmp */
void* user_error_ptr = 0;

void user_error_fn(png_structp png_ptr, png_const_charp error_msg)
{
}

void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
{
}
