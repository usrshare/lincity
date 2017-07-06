/* ---------------------------------------------------------------------- *
 * lcsdl.c
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * Based on lcx11.c, changes (c) usr_share 2015
 * ---------------------------------------------------------------------- */
#include "lcconfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "lcstring.h"
#include "lcintl.h"
#include "fileutil.h"
#include "lclib.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "lin-city.h"
#include "lctypes.h"
#include "cliglobs.h"
#include "lcsdl.h"
#include "mouse.h"
#include "screen.h"

void HandleEvent (SDL_Event *);

SDL_Surface* tilemap = NULL;
//SDL_Surface* font_surface = NULL;
TTF_Font* eight_font = NULL;
TTF_Font* eight_mono_font = NULL;
TTF_Font* sixteen_font = NULL;

SDL_Color t_bgcolor, t_fgcolor;

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

#define USE_IMAGES 1

#define DEBUG_X11_MOUSE
#undef DEBUG_X11_MOUSE

disp display;

typedef struct _disp_private {
	SDL_Surface *dpy; //screen
	SDL_Surface *bg; //background
	SDL_Surface *ui; //background
	SDL_Surface *sprites; //square mouse.
	
	SDL_Color bg_xcolor;
	SDL_Color pixcolour_gc[256];
	SDL_Palette* cmap;
} disp_private;
disp_private display_p;

int borderx, bordery;

int command_line_debug = 0;

int winW, winH, mouse_button;

char *bg_color = NULL;
int verbose = FALSE;		// display settings if TRUE
int stay_in_front = FALSE;	// Try to stay in clear area of the screen.
int text_bg = 0;
int text_fg = 255;
int x_key_value;
int x_key_shifted = 0;	//Is the key shifted?
SDL_Rect cliprect;
int clipping_flag = 0;
//long unsigned int colour_table[256];
unsigned char *open_font;
int open_font_height, suppress_next_expose = 0;

lcicon up_pbar1_graphic, up_pbar2_graphic;
lcicon down_pbar1_graphic, down_pbar2_graphic, pop_pbar_graphic;
lcicon tech_pbar_graphic, food_pbar_graphic, jobs_pbar_graphic;
lcicon money_pbar_graphic, coal_pbar_graphic, goods_pbar_graphic;
lcicon ore_pbar_graphic, steel_pbar_graphic;
lcicon pause_button1_off, pause_button2_off;
lcicon pause_button1_on, pause_button2_on;
lcicon fast_button1_off, fast_button2_off;
lcicon fast_button1_on, fast_button2_on;
lcicon med_button1_off, med_button2_off;
lcicon med_button1_on, med_button2_on;
lcicon slow_button1_off, slow_button2_off;
lcicon slow_button1_on, slow_button2_on;
lcicon results_button1, results_button2;
lcicon toveron_button1, toveron_button2;
lcicon toveroff_button1, toveroff_button2;
lcicon ms_pollution_button_graphic, ms_normal_button_graphic;
lcicon ms_fire_cover_button_graphic, ms_health_cover_button_graphic;
lcicon ms_cricket_cover_button_graphic;
lcicon ms_ub40_button_graphic, ms_coal_button_graphic;
lcicon ms_starve_button_graphic, ms_ocost_button_graphic;
lcicon ms_power_button_graphic;
lcicon checked_box_graphic, unchecked_box_graphic;

static SDL_Surface* surface_by_layer(enum disp_layers l) {
	switch(l) {
		case DL_DIRECT: return display_p.dpy;
		case DL_BG: return display_p.bg;
		case DL_UI: return display_p.ui;
		case DL_SPRITES: return display_p.sprites;
		default: return NULL;
	}
}

static TTF_Font* getsdlfont(enum text_fonts f) {
	switch(f) {
		case TF_DEFAULT: return eight_font;
		case TF_MONO: return eight_mono_font;
		case TF_LARGE: return sixteen_font;
		default: return NULL;
	}
}

void set_pointer_confinement (void)
{
	if (confine_flag) {
		SDL_WM_GrabInput(SDL_GRAB_ON);
	} else {
		SDL_WM_GrabInput(SDL_GRAB_OFF);
	}
}

int confine_pointer (int x, int y, int w, int h) 
{
	SDL_WM_GrabInput(SDL_GRAB_ON);
	display.pointer_confined = 1;
	return 0;
}


void unconfine_pointer (void)
{

	SDL_WM_GrabInput(SDL_GRAB_OFF);
	display.pointer_confined = 0;
	return;
}

void setcustompalette (void)
{
	fprintf(stderr,"setcustompalette() has been removed. Palettes are loaded from icons.png now.\n");
	return;
}

void do_setcustompalette (SDL_Color* inpal) {
	SDL_SetPalette((display_p.dpy), SDL_LOGPAL, display_p.pixcolour_gc, 0, 256);
	SDL_SetPalette((display_p.dpy), SDL_PHYSPAL, display_p.pixcolour_gc, 0, 256);
	SDL_SetPalette((display_p.bg), SDL_LOGPAL, display_p.pixcolour_gc, 0, 256);
	SDL_SetPalette((display_p.ui), SDL_LOGPAL, display_p.pixcolour_gc, 0, 256);
	SDL_SetColorKey(display_p.ui,SDL_SRCCOLORKEY,0);
	SDL_SetPalette((display_p.sprites), SDL_LOGPAL, display_p.pixcolour_gc, 0, 256);
	SDL_SetColorKey(display_p.sprites,SDL_SRCCOLORKEY,0);
}

void open_setcustompalette (SDL_Color* inpal) {
	do_setcustompalette (inpal);
}

#if defined (commentout)
	void
initfont ()
{
	int i;
	FILE *finf;
	if ((finf = fopen (fontfile, "r")) == 0)
		HandleError ("Can't open the font file", FATAL);
	for (i = 0; i < 256 * 8; i++)
		myfont[i] = fgetc (finf);
	fclose (finf);
}
#endif

	void
Fgl_setfontcolors (int bg, int fg)
{
	t_bgcolor = display_p.pixcolour_gc[bg];
	t_fgcolor = display_p.pixcolour_gc[fg];
}

void parse_sdlargs (int argc, char **argv, char **geometry)
{
	int option;
	extern char *optarg;

	int r = SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO); 
	if (r == -1) {
		fprintf(stderr,"Unable to initialize SDL.\n"); exit(1);}

	r = TTF_Init();
	if (r == -1) {
		fprintf(stderr,"Unable to initialize SDL_ttf.\n"); exit(1);}

	SDL_EnableUNICODE(1);

	char* option_string = "vbrng:wR:G:B:D";
	while ((option = getopt (argc, argv, option_string)) != EOF) {
		switch (option)
		{
			case 'v':
				verbose = TRUE;
				break;
			case 'g':
				*geometry = optarg;
				break;
			case 'b':
				borderx = 0;
				bordery = 0;
				break;
			case 'r':
				borderx = 1;
				bordery = 1;
				break;
			case 'n':
				no_init_help = TRUE;
				break;
			case 'w':
				gamma_correct_red = GAMMA_CORRECT_RED;
				gamma_correct_green = GAMMA_CORRECT_GREEN;
				gamma_correct_blue = GAMMA_CORRECT_BLUE;
				break;
			case 'R':
				sscanf (optarg, "%f", &gamma_correct_red);
				break;
			case 'G':
				sscanf (optarg, "%f", &gamma_correct_green);
				break;
			case 'B':
				sscanf (optarg, "%f", &gamma_correct_blue);
				break;
			case 'D':
				command_line_debug = 1;
				break;
		}
	}
	if (verbose)
		printf (_("Version %s\n"), VERSION);

	// Record the screen number and root window.
	//display.screen = DefaultScreen (display_p.dpy);
	//display.root = RootWindow (display_p.dpy, display.screen);

	winW = WINWIDTH;
	winH = WINHEIGHT;

}

void lcsdl_load_fonts(void) {
    eight_font = TTF_OpenFont(fontfile_sans,11);
    if (eight_font == NULL) HandleError("Can't open the small font file", FATAL);
    eight_mono_font = TTF_OpenFont(fontfile_mono,11);
    if (eight_mono_font == NULL) HandleError("Can't open the monospaced font file", FATAL);
    TTF_SetFontKerning(eight_mono_font,0);
    TTF_SetFontHinting(eight_mono_font,TTF_HINTING_MONO);
    sixteen_font = TTF_OpenFont(fontfile_sans,16);
    if (eight_font == NULL) HandleError("Can't open the large font file", FATAL);
}


void Create_Window (char *geometry)
{

	display_p.dpy = SDL_SetVideoMode(winW, winH, 8, SDL_HWSURFACE | SDL_RESIZABLE);

	if (display_p.dpy == 0) { fprintf(stderr,"Unable to create screen surface.\n"); return; }

	display_p.bg = SDL_CreateRGBSurface(SDL_HWSURFACE, winW, winH, 8, 0,0,0,0);

	if (display_p.bg == 0) { fprintf(stderr,"Unable to create background surface.\n"); return; }

	display_p.sprites = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, winW, winH, 8, 0,0,0,0);

	if (display_p.sprites == 0) { fprintf(stderr,"Unable to create sprite surface.\n"); return; }

	SDL_SetColorKey(display_p.sprites,SDL_SRCCOLORKEY,0);
	
	display_p.ui = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, winW, winH, 8, 0,0,0,0);

	if (display_p.ui == 0) { fprintf(stderr,"Unable to create sprite surface.\n"); return; }

	SDL_SetColorKey(display_p.ui,SDL_SRCCOLORKEY,0);


	char wname[256];	/* Window Name */
	char iname[256];	/* Window Name */

	sprintf (wname,
			_("xlincity, Version %s, "
				"(Copyright) IJ Peters - copying policy GNU GPL"),
			VERSION);

	sprintf (iname, "xlincity");

	SDL_WM_SetCaption(wname, iname);

}

	void 
unlock_window_size (void) 
{
}

void HandleError (char *description, int degree) {

	fprintf (stderr,
			_("An error has occurred.  The description is below...\n"));
	fprintf (stderr, "%s\n", description);

	if (degree == FATAL) {
		fprintf (stderr, _("Program aborting...\n"));
		exit (-1);
	}
}

void Fgl_setpixel_s (SDL_Surface* s, int x, int y, int col) {

	assert (s->format->BytesPerPixel == 1);
	int bpl = s->pitch;

	uint8_t* p = (uint8_t *)s->pixels + (y * bpl) + x;
	*p = col;
}

void Fgl_setpixel (int x, int y, int col) {

	assert ((display_p.bg)->format->BytesPerPixel == 1);

	uint8_t* p = (uint8_t *)(display_p.bg)->pixels + y * (display_p.bg)->pitch + x;
	*p = col;
}

void Fgl_hline_s (enum disp_layers l, int x1, int y1, int x2, int col) {
	col &= 0xff;
	//pixmap_hline (x1, y1, x2, col);

	struct SDL_Rect dstrect = {.x=x1, .y=y1, .w=x2-x1+1, .h=1};
	SDL_FillRect(surface_by_layer(l),&dstrect,col);
}

void Fgl_line_s (enum disp_layers l, int x1, int y1, int dummy, int y2, int col) {
	//vertical lines only
	col &= 0xff;
	//pixmap_vline (x1, y1, y2, col);

	struct SDL_Rect dstrect = {.x=x1, .y=y1, .w=1, .h=y2-y1+1};
	SDL_FillRect(surface_by_layer(l),&dstrect,col);
}

void Fgl_hline (int x1, int y1, int x2, int col) {
	return Fgl_hline_s(DL_BG,x1,y1,x2,col);
}

void Fgl_line (int x1, int y1, int dummy, int y2, int col) {
	return Fgl_line_s(DL_BG,x1,y1,dummy,y2,col);
}

int lc_txtwidth(enum text_fonts f, char *s) {
	int w,h;
	TTF_SizeUTF8(getsdlfont(f),s,&w,&h);
	return w;
}

void Fgl_write3 (enum disp_layers l, enum text_fonts f, int x, int y, int w, char *s, enum text_align align){

	if (strlen(s) == 0) return;

	SDL_Surface* textsurf = TTF_RenderUTF8_Shaded(getsdlfont(f),s,t_fgcolor,t_bgcolor);

	if (textsurf == 0) {fprintf(stderr,"Unable to create text surface (%s).\n",SDL_GetError()); return;}

	int tw = textsurf->w;

	SDL_Rect o_rect = {.x = x, .y = y, .w = 0, .h = 0};

	switch (align) {
		case TA_LEFT:
			break;
		case TA_CENTER:
			o_rect.x += (w - tw) / 2; break;
		case TA_RIGHT:
			o_rect.x += (w - tw); break;
	}

	SDL_BlitSurface(textsurf,NULL,surface_by_layer(l),&o_rect);
	SDL_FreeSurface(textsurf);
}

void Fgl_write2_s (enum disp_layers l, int x, int y, int w, char *s, enum text_align align){

	if (strlen(s) == 0) return;

	SDL_Surface* textsurf = TTF_RenderUTF8_Shaded(eight_font,s,t_fgcolor,t_bgcolor);

	if (textsurf == 0) {fprintf(stderr,"Unable to create text surface (%s).\n",SDL_GetError()); return;}

	int tw = textsurf->w;

	SDL_Rect o_rect = {.x = x, .y = y, .w = 0, .h = 0};

	switch (align) {
		case TA_LEFT:
			break;
		case TA_CENTER:
			o_rect.x += (w - tw) / 2; break;
		case TA_RIGHT:
			o_rect.x += (w - tw); break;
	}

	SDL_BlitSurface(textsurf,NULL,surface_by_layer(l),&o_rect);
	SDL_FreeSurface(textsurf);
}

void Fgl_write2 (int x, int y, int w, char *s, enum text_align align){

	if (strlen(s) == 0) return;

	SDL_Surface* textsurf = TTF_RenderUTF8_Shaded(eight_font,s,t_fgcolor,t_bgcolor);

	if (textsurf == 0) {fprintf(stderr,"Unable to create text surface (%s).\n",SDL_GetError()); return;}

	int tw = textsurf->w;

	SDL_Rect o_rect = {.x = x, .y = y, .w = 0, .h = 0};

	switch (align) {
		case TA_LEFT:
			break;
		case TA_CENTER:
			o_rect.x += (w - tw) / 2; break;
		case TA_RIGHT:
			o_rect.x += (w - tw); break;
	}

	SDL_BlitSurface(textsurf,NULL,display_p.bg,&o_rect);

	SDL_FreeSurface(textsurf);
}

void Fgl_write (int x, int y, char *s){

	return Fgl_write2(x,y,0,s,TA_LEFT);
}

void Fgl_fillbox_s (enum disp_layers l, int x1, int y1, int w, int h, int col)
{
	/*
	if (clipping_flag) {
		if (x1 < xclip_x1)
			x1 = xclip_x1;
		if (x1 + w - 1 > xclip_x2)
			w = xclip_x2 - x1 + 1;
		if (y1 < xclip_y1)
			y1 = xclip_y1;
		if (y1 + h - 1 > xclip_y2)
			h = xclip_y2 - y1 + 1;
	}*/
	col &= 0xff;
	//pixmap_fillbox (x1, y1, w, h, col);

	SDL_Rect dstrect = {.x=x1,.y=y1,.w=w,.h=h};
	SDL_FillRect(surface_by_layer(l), &dstrect, col);
}

void Fgl_fillbox (int x1, int y1, int w, int h, int col)
{
	return Fgl_fillbox_s(DL_BG,x1,y1,w,h,col);
}

/*
 * Instead of transfering a pixmap pixel by pixel, it is much more
 * efficient to build an XImage in core and send it in one piece off
 * to the X server.
 *
 * -- GB.
 */

	static int
clamp (int x, int low, int high)
	/* clamp x to the interval [low, high] */
{
	if (x < low)
		x = low;
	else if (x > high)
		x = high;
	return x;
}

/*
 * Copy the the sub-image (src_x, src_y, w, h) to the screen at 
 * (x0 + x1, y0 + y1).
 * `src' is a pointer to the array of pixels.
 * `bpl' is the width of that array.
 * No clipping performed.
 */

void Fgl_blit (enum disp_layers l, lcicon i, int dx, int dy, int w, int h) {

	SDL_Rect srcrect = (SDL_Rect){.x = i.x_offset, .y = i.y_offset,
		.w = w ? w : i.width, .h = h ? h : i.height};
	SDL_Rect dstrect = (SDL_Rect){.x = dx, .y = dy,
		.w = w ? w : i.width, .h = h ? h : i.height};
	SDL_BlitSurface(i._surface,&srcrect,surface_by_layer(l),&dstrect);
}

void Fgl_putbox (int x, int y, int w, int h, lcicon i) {

	return Fgl_blit(DL_BG,i,x,y,w,h);
}

void HandleEvent (SDL_Event *event)
{

	SDL_Event loop_ev; /* for clearing the queue of events */
	switch (event->type)
	{
		case SDL_KEYDOWN:
		case SDL_KEYUP:
			{
				SDL_KeyboardEvent key_event = event->key;
				SDL_keysym ks = key_event.keysym;

				x_key_shifted = key_event.keysym.mod & (KMOD_RSHIFT | KMOD_LSHIFT);

				switch (ks.sym) {
					case SDLK_LEFT:
						x_key_value = 1;
						break;
					case SDLK_DOWN:
						x_key_value = 2;
						break;
					case SDLK_UP:
						x_key_value = 3;
						break;
					case SDLK_RIGHT:
						x_key_value = 4;
						break;
					case SDLK_BACKSPACE:
					case SDLK_DELETE:
						x_key_value = 127;
						break;
					default:
						x_key_value = ( ks.unicode < 127 ? ks.unicode : 0);
						break;
				}
			}
			break;

		case SDL_MOUSEMOTION: 
			{
				SDL_MouseMotionEvent ev = event->motion;

				int dx = ev.xrel; int dy = ev.yrel;

				while (SDL_PeepEvents(&loop_ev,1,SDL_GETEVENT,SDL_MOUSEMOTIONMASK) > 0) {
					event = &loop_ev; ev = event->motion;
					dx += ev.xrel; dy += ev.yrel;
				}

				cs_mouse_handler (0, dx, dy);  

				cs_mouse_x = ev.x;
				cs_mouse_y = ev.y;

				if (ev.state & SDL_BUTTON(2))
					drag_screen();
			}
			break;

		case SDL_MOUSEBUTTONDOWN:
			{
				SDL_MouseButtonEvent ev = event->button;
				if (cs_shift_held != 0)
					cs_mouse_shifted = 1;
				else
					cs_mouse_shifted = 0;

				switch (ev.button) {
					case SDL_BUTTON_LEFT:
						mouse_button = LC_MOUSE_LEFTBUTTON | LC_MOUSE_PRESS;
						break;
					case SDL_BUTTON_MIDDLE:
						mouse_button = LC_MOUSE_MIDDLEBUTTON | LC_MOUSE_PRESS;
						break;
					case SDL_BUTTON_RIGHT:
						mouse_button = LC_MOUSE_RIGHTBUTTON | LC_MOUSE_PRESS;
						break;

						/* Wheel mouse support 
						   Move further for Shift (in main.c: process_keystrokes() ),
						   left to right instead of up and down for Control */

					case 4:  /* Up (3); Left (1) if Control */
						x_key_shifted = cs_shift_held;
						x_key_value = (cs_ctrl_held) ? 1 : 3; 
						break; 
					case 5: /* Down (4); Right (2) if control */
						x_key_shifted = cs_shift_held;
						x_key_value = (cs_ctrl_held) ? 4 : 2;
						break;

						/* XFree86-3 only supports 5 buttons, no Button6 or higher */

				}
				cs_mouse_handler (mouse_button, 0, 0);
				mouse_button = 0;
			}
			break;

		case SDL_MOUSEBUTTONUP:
			{
				SDL_MouseButtonEvent ev = event->button;
				mouse_button = ev.button; 

				switch (ev.button) {
					case SDL_BUTTON_LEFT:
						mouse_button = LC_MOUSE_LEFTBUTTON | LC_MOUSE_RELEASE;
						break;
					case SDL_BUTTON_MIDDLE:
						mouse_button = LC_MOUSE_MIDDLEBUTTON | LC_MOUSE_RELEASE;
						break;
					case SDL_BUTTON_RIGHT:
						mouse_button = LC_MOUSE_RIGHTBUTTON | LC_MOUSE_RELEASE;
						break;
				}
				cs_mouse_handler (mouse_button, 0, 0);
				mouse_button = 0;
			}
			break;

		case SDL_VIDEOEXPOSE:
			{

				while (SDL_PeepEvents(&loop_ev,1,SDL_GETEVENT,SDL_VIDEOEXPOSEMASK) > 0) {
					event = &loop_ev;
				}
				/*
				   XExposeEvent *ev = (XExposeEvent *) event;
				   int gx1,gy1,gx2,gy2;
				   gx1 = ev->x;
				   gy1 = ev->y;
				   gx2 = ev->x + ev->width;
				   gy2 = ev->y + ev->height;

				// Coalesce waiting exposes into single redraw
				while (XCheckMaskEvent(display_p.dpy,ExposureMask,&loop_ev)) {
				ev = (XExposeEvent *) &loop_ev;
				gx1 = min_int (gx1,ev->x);
				gy1 = min_int (gy1,ev->y);
				gx2 = max_int (gx2,ev->x + ev->width);
				gy2 = max_int (gy2,ev->y + ev->height);
				}
				if (suppress_next_expose) {
				suppress_next_expose = 0;
				break;
				}
				 */
				refresh_screen (0,0,winW,winH);
			}
			break;

		case SDL_VIDEORESIZE:
			{
				while (SDL_PeepEvents(&loop_ev,1,SDL_GETEVENT,SDL_VIDEORESIZEMASK) > 0) {
					event = &loop_ev;
				}

				SDL_ResizeEvent ev = event->resize;

				if (ev.w < 640) ev.w = 640;
				if (ev.h < 480) ev.h = 480;

				display_p.dpy = SDL_SetVideoMode(ev.w,ev.h,8,SDL_HWSURFACE | SDL_RESIZABLE);
				SDL_FreeSurface(display_p.bg);
				display_p.bg = SDL_CreateRGBSurface(SDL_HWSURFACE, ev.w, ev.h, 8, 0,0,0,0);
				SDL_FreeSurface(display_p.ui);
				display_p.ui = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, ev.w, ev.h, 8, 0,0,0,0);
				SDL_FreeSurface(display_p.sprites);
				display_p.sprites = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, ev.w, ev.h, 8, 0,0,0,0);

				do_setcustompalette(NULL);	

				SDL_SetColorKey(display_p.ui,SDL_SRCCOLORKEY,0);
				SDL_SetColorKey(display_p.sprites,SDL_SRCCOLORKEY,0);
				resize_geometry (ev.w, ev.h);
				winW = ev.w; winH = ev.h;

			}
			break;
		case SDL_QUIT:
			{
				SDL_Quit();
				exit(0);
				break;
			}
		default: {
				 //fprintf(stderr,"Unknown event type %d\n",event->type);
				 return;
			 }
	}
}

#undef DEBUG_X11_MOUSE

	void
refresh_screen (int x1, int y1, int x2, int y2)		/* bounds of refresh area */
{

	//if all four parameters are zeros, the entire screen is refreshed.

	if ((x1 == 0) && (x2 == 0)) x2 = winW;
	if ((y1 == 0) && (y2 == 0)) y2 = winH;

	SDL_Rect orect = {.x = x1, .y = y1, .w = x2-x1, .h = y2-y1};
	SDL_Rect drect = {.x = x1, .y = y1, .w = x2-x1, .h = y2-y1};

	SDL_BlitSurface(display_p.bg,&orect,display_p.dpy,&drect);

	if (display.show_ui) SDL_BlitSurface(display_p.ui,&orect,display_p.dpy,&drect);
	if (display.show_sprites) SDL_BlitSurface(display_p.sprites,&orect,display_p.dpy,&drect);
	SDL_UpdateRect(display_p.dpy,x1,y1,x2-x1,y2-y1);
}

void refresh_rect(Rect* r) {
	refresh_screen(r->x,r->y,r->x + r->w, r->y + r->h);
}

void Fgl_enableclipping (void)
{
	clipping_flag = 1;
	SDL_SetClipRect(display_p.dpy, clipping_flag ? &cliprect : NULL);
}

void Fgl_setclippingwindow (int x1, int y1, int x2, int y2)
{
	cliprect.x = x1;
	cliprect.y = y1;
	cliprect.w = x2 - x1;
	cliprect.h = y2 - y1;
	SDL_SetClipRect(display_p.dpy, clipping_flag ? &cliprect : NULL);
}

void Fgl_disableclipping (void)
{
	clipping_flag = 0;
	SDL_SetClipRect(display_p.dpy, clipping_flag ? &cliprect : NULL);
}

void do_call_event (int wait) {

	SDL_Event ev;
	int r = 0;
	if (wait) r= SDL_WaitEvent(&ev); else r= SDL_PollEvent(&ev);
	if (r) HandleEvent(&ev);
}

void call_event (void) {
	do_call_event (0);
}

void call_wait_event (void) {
	do_call_event (1);
}

int lc_get_keystroke (void)
{
	int q;
	call_event ();
	q = x_key_value;
	x_key_value = 0;
	return q;
}

int lc_setpalettecolors(int n, uint8_t* colors) {

	//colors is an array to n*3 uint8_t in (r,g,b) format.

	for (int i=0; i < n; i++) {
		display_p.pixcolour_gc[i] = (SDL_Color){.r = colors[i*3], .g = colors[i*3+1], .b = colors[i*3+2]}; }

	SDL_SetPalette(display_p.dpy,SDL_LOGPAL,display_p.pixcolour_gc,0,256);
	SDL_SetPalette(display_p.dpy,SDL_PHYSPAL,display_p.pixcolour_gc,0,256);
	SDL_SetPalette(display_p.bg,SDL_LOGPAL,display_p.pixcolour_gc,0,256);
	SDL_SetPalette(display_p.sprites,SDL_LOGPAL,display_p.pixcolour_gc,0,256);
	SDL_SetColorKey(display_p.sprites,SDL_SRCCOLORKEY,0);
	return 0;
}

int lc_setpalettecolor(int x, int r, int g, int b) {

	display_p.pixcolour_gc[x] = (SDL_Color){.r = r, .g=g,.b=b};
	SDL_SetPalette(display_p.dpy,SDL_LOGPAL,display_p.pixcolour_gc,0,256);
	SDL_SetPalette(display_p.dpy,SDL_PHYSPAL,display_p.pixcolour_gc,0,256);
	SDL_SetPalette(display_p.bg,SDL_LOGPAL,display_p.pixcolour_gc,0,256);
	SDL_SetPalette(display_p.sprites,SDL_LOGPAL,display_p.pixcolour_gc,0,256);
	SDL_SetColorKey(display_p.sprites,SDL_SRCCOLORKEY,0);
	return 0;
}

int lc_loadpalette(uint32_t* pal) {

	if (!pal) return 1;

	for (int i=0; i < 256; i++) {
		uint32_t col = pal[i];

		display_p.pixcolour_gc[i] = (SDL_Color){.r = (col & 0xff0000) >> 16, .g = (col & 0xff00) >> 8, .b = (col & 0xff)};
	}
	do_setcustompalette(NULL);
	return 0;
}

/* init_full_mouse is called just before the main client loop. */

/* XXX: This needs a much better name */
/* GCS: Yes it does, b/c it means something different for svgalib */
	void 
init_x_mouse (void) 
{
}

	void
draw_border (void)
{
}


void draw_small_bezel_s (enum disp_layers l, int x, int y, int w, int h, int colour){
    int i;
    for (i = 1; i < 4; i++) {
	Fgl_hline_s (l,x - 1 - i, y - 1 - i, x + w + i, colour);
	Fgl_line_s (l,x - 1 - i, y - 1 - i, x - 1 - i, y + h + i, colour + 1);
	Fgl_hline_s (l,x - 1 - i, y + h + i, x + w + i, colour + 2);
	Fgl_line_s (l,x + w + i, y - 1 - i, x + w + i, y + h + i, colour + 3);
    }
}

void draw_bezel_s (enum disp_layers l, Rect r, short width, int color) {
  int i;
  int c;
  for (i = 0; i < width; i++)
    {
      c = color + (width - i);
      Fgl_hline_s (l,r.x + i, r.y + i, r.x + r.w - i - 1, c);
      Fgl_hline_s (l,r.x + i, r.y + r.h - i - 1, r.x + r.w - i - 1, c);
      Fgl_line_s (l,r.x + i, r.y + i, r.x + i, r.y + r.h - i - 1, c);
      Fgl_line_s (l,r.x + r.w - i - 1, r.y + i, r.x + r.w - i - 1, 
		r.y + r.h - i - 1, c);
    }
}

lcicon type_icons[NUM_OF_TYPES];

void init_icon_pixmap (short type) {
}

lcicon load_icon(char* filename, int w, int h) {
	
	lcicon newicon;

	char ss[LC_PATH_MAX];
	strcpy(ss,graphic_path);
	strcat(ss,filename);

	SDL_Surface* newsurf = IMG_Load(ss);
	if (!newsurf) {
		strcat(ss," -- UNABLE TO LOAD");
		do_error(ss);
	}

	SDL_Surface* convsurf = SDL_ConvertSurface(newsurf,display_p.dpy->format,0);
	if (!convsurf) {
		strcat(ss," -- UNABLE TO CONVERT");
		do_error(ss);
	}
	SDL_FreeSurface(newsurf);

	SDL_SetPalette(convsurf,SDL_LOGPAL,display_p.pixcolour_gc,0,256);

	newicon._surface = convsurf;
	newicon.x_offset = newicon.y_offset = 0;
	newicon.width = w ? w : convsurf->w;
	newicon.height = h ? h : convsurf->h;

	return newicon;
}
int free_icon (lcicon i) {
	SDL_FreeSurface(i._surface);
	return 0;
}

int load_tilemap_graphic (short type, short group, char* id, FILE* txt_fp) {

	char buf[128];
	char *id_p, *row_p, *col_p;
	int row, col;

	while (!feof(txt_fp)) {
		fgets(buf,128,txt_fp);
		id_p = strtok(buf, " \t");
		
		if ((!id_p[0]) || (id_p[0] == '#')) continue; //ignore empty lines and everything that starts with a hash
		if (id_p[0] == '@') break; //at sign means end of list

		row_p = strtok(NULL, " \t");
		if (!row_p) continue;
		col_p = strtok(NULL, " \t");
		if (!col_p) continue;

		row = atoi(row_p);
		col = atoi(col_p);

		if (strcmp(id_p, id) == 0) {
			type_icons[type]._surface = tilemap;
			type_icons[type].x_offset = col * 16;
			type_icons[type].y_offset = row * 16;
			return 0; //everything OK
		} else {
			fprintf (stderr, "Wrong ID string -- expected %s, got %s\n",id,id_p);
			exit(-1);
		}
	}

	return 0;
}
int load_tilemap (char* png_filename, uint32_t* out_palette) {

	SDL_Surface* newtm = IMG_Load(png_filename);
	if (!newtm) {
		return 1;
	}

	if (newtm->format->BitsPerPixel != 8) {
		fprintf(stderr,"Tilemap file is not 8-bit indexed.\n");
		SDL_FreeSurface(newtm);
		return 1;
	}

	SDL_Palette* imgpal = newtm->format->palette;
	if (!imgpal) {
		fprintf(stderr,"Tilemap file does not have a palette.\n");
		SDL_FreeSurface(newtm);
		return 1;
	}
	if (imgpal->ncolors < 256) {
		fprintf(stderr,"Tilemap file's palette is too short (%d colors instead of 256).\n",imgpal->ncolors);
		SDL_FreeSurface(newtm);
		return 1;
	}

	for (int i=0; i < 256; i++) {
		out_palette[i] = 
			((imgpal->colors[i].r) << 16) +
			((imgpal->colors[i].g) << 8) +
			((imgpal->colors[i].b));
	}

	tilemap = newtm; // keep the tilemap in memory.


	// main_t
	return 0;
}

