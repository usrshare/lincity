/* ---------------------------------------------------------------------- *
 * splash.c
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * ---------------------------------------------------------------------- */
#include "lcconfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "lcstring.h"

#include "common.h"
#include "lctypes.h"
#include "lin-city.h"
#include "cliglobs.h"
#include "engglobs.h"
#include "screen.h"
#include "fileutil.h"
#include "lcsdl.h"

#define SI_BLACK 252
#define SI_RED 253
#define SI_GREEN 254
#define SI_YELLOW 255

/* ---------------------------------------------------------------------- *
 * Private Fn Prototypes
 * ---------------------------------------------------------------------- */


/* ---------------------------------------------------------------------- *
 * Private Global Variables
 * ---------------------------------------------------------------------- */


/* ---------------------------------------------------------------------- *
 * Public Functions
 * ---------------------------------------------------------------------- */
void
load_start_image (void)
{
  long x, y, l, r, g, b;
  FILE *fp;

  fp = fopen_read_gzipped (opening_pic);
  if (fp == NULL) {
      return;
  }

  for (x = 0; x < 7; x++)
    l = fgetc (fp);
  l &= 0xff;
  if (l == 0)
    l = 256;

  uint8_t colors[l * 3];

  for (x = 0; x < l; x++)
    {
      colors[x * 3] = fgetc (fp) * 4;
      colors[x*3+1] = fgetc (fp) * 4;
      colors[x*3+2] = fgetc (fp) * 4;
    }
 
  lc_setpalettecolors(256,colors);
  /* use last 4 colours for text */
  lc_setpalettecolor (SI_BLACK, 0, 0, 0);
  lc_setpalettecolor (SI_RED, 240, 0, 0);
  lc_setpalettecolor (SI_GREEN, 0, 240, 0);
  lc_setpalettecolor (SI_YELLOW, 240, 240, 0);
  for (y = 0; y < 480; y++)
    for (x = 0; x < 640; x++)
      {
	l = fgetc (fp);
	/*
	   //      printf("l=%d x=%d y=%d ",l,x,y);
	   // octree doesn't seem to want to generate images with 252 colours!
	   // So the next best thing (well the easyest) is to just map the pixels
	   // coloured as the last 4 colours to the 4 before that.
	   // If it looks OK, leave it.
	 */
	if (l == SI_BLACK)
	  l = SI_BLACK - 4;
	if (l == SI_RED)
	  l = SI_RED - 4;
	if (l == SI_GREEN)
	  l = SI_GREEN - 4;
	if (l == SI_YELLOW)
	  l = SI_YELLOW - 4;
	Fgl_setpixel (x, y, l);
      }

  fclose_read_gzipped (fp);
  
  refresh_screen(0,0,0,0);

  start_image_text ();
}

void
start_image_text (void)
{
  Fgl_setfontcolors (SI_BLACK, SI_RED);
  si_scroll_text ();
}

void
si_scroll_text (void)
{
  char s[LC_PATH_MAX], line1[100], line2[100], line3[100], c;
  int i, l1c = 0, l2c = 0, l3c = 0;
  long t;
  FILE *inf1, *inf2, *inf3;
  Fgl_enableclipping ();
  sprintf (s, "%s%c%s", opening_path, PATH_SLASH, "text1");
  if ((inf1 = fopen (s, "rb")) == NULL)
    do_error ("Can't open opening/text1");
  for (i = 0; i < 52; i++)
    line1[i] = si_next_char (inf1);
  line1[52] = 0;
  sprintf (s, "%s%c%s", opening_path, PATH_SLASH, "text2");
  if ((inf2 = fopen (s, "rb")) == NULL)
    do_error ("Can't open opening/text2");
  for (i = 0; i < 52; i++)
    line2[i] = si_next_char (inf2);
  line2[52] = 0;
  sprintf (s, "%s%c%s", opening_path, PATH_SLASH, "text3");
  if ((inf3 = fopen (s, "rb")) == NULL)
    do_error ("Can't open opening/text3");
  for (i = 0; i < 52; i++)
    line3[i] = si_next_char (inf3);
  line3[52] = 0;
  do
    {
      get_real_time ();
      t = real_time + SPLASH_SCROLL_DELAY;
      c = lc_get_keystroke ();
      if (l1c >= 8)
	{
	  for (i = 0; i < 51; i++)
	    line1[i] = line1[i + 1];
	  line1[51] = si_next_char (inf1);
	  l1c = 0;
	}
      Fgl_setclippingwindow (120, 30, 520, 40);
      Fgl_setfontcolors (SI_BLACK, SI_RED);
      Fgl_write3 (DL_BG, TF_DEFAULT, 120 - l1c, 31, 0, line1, TA_LEFT);
      refresh_screen(120,30,520,40);
      l1c++;

      if (l2c >= 8)
	{
	  for (i = 0; i < 51; i++)
	    line2[i] = line2[i + 1];
	  line2[51] = si_next_char (inf2);
	  l2c = 0;
	}
      Fgl_setclippingwindow (120, 55, 520, 73);
      Fgl_setfontcolors (SI_BLACK, SI_GREEN);
      Fgl_write3 (DL_BG, TF_LARGE, 120 - l2c, 57, 0, line2, TA_LEFT);
      refresh_screen(120,55,520,73);
      l2c += 2;

      if (l3c >= 8)
	{
	  for (i = 0; i < 51; i++)
	    line3[i] = line3[i + 1];
	  line3[51] = si_next_char (inf3);
	  l3c = 0;
	}
      Fgl_setclippingwindow (120, 88, 520, 106);
      Fgl_setfontcolors (SI_BLACK, SI_YELLOW);
      Fgl_write3 (DL_BG, TF_LARGE, 120 - l3c, 90, 0, line3, TA_LEFT);
      refresh_screen(120,88,520,106);
      l3c += 2;
      while (real_time < t)
	{
	  lc_usleep(1000*(t-real_time));
	  get_real_time ();
	}
    }
  while (c == 0);
  fclose (inf1);
  fclose (inf2);
  fclose (inf3);
  Fgl_disableclipping ();
}

char
si_next_char (FILE * inf)
{
  char c;
  if (feof (inf) != 0)
    fseek (inf, 0L, SEEK_SET);
  c = fgetc (inf);
  if (c == 0xa || c == 0xd)
    c = ' ';
  return (c);
}

