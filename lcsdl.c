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
#include <SDL_ttf.h>
#include "lin-city.h"
#include "lctypes.h"
#include "cliglobs.h"
#include "lcsdl.h"
#include "pixmap.h"
#include "mouse.h"
#include "screen.h"

void HandleEvent (SDL_Event *);

//SDL_Surface* font_surface = NULL;
TTF_Font* eight_font = NULL;
TTF_Font* sixteen_font = NULL;

TTF_Font* curfont = NULL;

SDL_Color t_bgcolor, t_fgcolor;

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif

#define USE_IMAGES 1

#define DEBUG_X11_MOUSE
#undef DEBUG_X11_MOUSE

disp display;
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
int xclip_x1, xclip_y1, xclip_x2, xclip_y2, clipping_flag = 0;
//long unsigned int colour_table[256];
unsigned char *open_font;
int open_font_height, suppress_next_expose = 0;

SDL_Surface* icon_surface[NUM_OF_TYPES];
char icon_surface_flag[NUM_OF_TYPES];

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

void open_setcustompalette (SDL_Color* inpal) {
	do_setcustompalette (inpal);
}

void do_setcustompalette (SDL_Color* inpal) {
	SDL_SetPalette((display.dpy), SDL_LOGPAL, display.pixcolour_gc, 0, 256);
	SDL_SetPalette((display.dpy), SDL_PHYSPAL, display.pixcolour_gc, 0, 256);
	SDL_SetPalette((display.bg), SDL_LOGPAL, display.pixcolour_gc, 0, 256);
	SDL_SetPalette((display.sprites), SDL_LOGPAL, display.pixcolour_gc, 0, 256);
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
	t_bgcolor = display.pixcolour_gc[bg];
	t_fgcolor = display.pixcolour_gc[fg];
}

	void

Fgl_setfont (int fw, int fh, void *fp)
{
	curfont = fp;
}

	void
parse_sdlargs (int argc, char **argv, char **geometry)
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
	//display.screen = DefaultScreen (display.dpy);
	//display.root = RootWindow (display.dpy, display.screen);

	winW = WINWIDTH;
	winH = WINHEIGHT;
	
}


void Create_Window (char *geometry)
{

	display.dpy = SDL_SetVideoMode(winW, winH, 8, SDL_HWSURFACE | SDL_RESIZABLE);
	
	if (display.dpy == 0) { fprintf(stderr,"Unable to create screen surface.\n"); return; }
	
	display.bg = SDL_CreateRGBSurface(SDL_HWSURFACE, winW, winH, 8, 0,0,0,0);
	
	if (display.bg == 0) { fprintf(stderr,"Unable to create background surface.\n"); return; }

	display.sprites = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, winW, winH, 8, 0,0,0,0);
	
	if (display.sprites == 0) { fprintf(stderr,"Unable to create sprite surface.\n"); return; }


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

	assert ((display.bg)->format->BytesPerPixel == 1);

	uint8_t* p = (uint8_t *)(display.bg)->pixels + y * (display.bg)->pitch + x;
	*p = col;
}

int Fgl_getpixel (int x, int y)
{

	int bpp = (display.bg)->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)(display.bg)->pixels + y * (display.bg)->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}

}

	void
Fgl_hline (int x1, int y1, int x2, int col)
{
	col &= 0xff;
	//pixmap_hline (x1, y1, x2, col);

	struct SDL_Rect dstrect = {.x=x1, .y=y1, .w=x2-x1+1, .h=1};
	SDL_FillRect(display.bg,&dstrect,col);
}

	void
Fgl_line (int x1, int y1, int dummy, int y2, int col)
	/* vertical lines only. */
{
	col &= 0xff;
	//pixmap_vline (x1, y1, y2, col);

	struct SDL_Rect dstrect = {.x=x1, .y=y1, .w=1, .h=y2-y1+1};
	SDL_FillRect(display.bg,&dstrect,col);
}


void Fgl_write2 (int x, int y, int w, char *s, enum text_align align){

	SDL_Surface* textsurf = TTF_RenderUTF8_Shaded(curfont,s,t_fgcolor,t_bgcolor);


	if (textsurf == 0) {fprintf(stderr,"Unable to create text surface.\n"); return;}
	
	int tw = textsurf->w;
	int th = textsurf->h;
	
	SDL_Rect o_rect = {.x = x, .y = y, .w = 0, .h = 0};

	switch (align) {
		case TA_LEFT:
			break;
		case TA_CENTER:
			o_rect.x += (w - tw) / 2; break;
		case TA_RIGHT:
			o_rect.x += (w - tw); break;
	}

	SDL_BlitSurface(textsurf,NULL,display.bg,&o_rect);

	SDL_FreeSurface(textsurf);
}

void Fgl_write (int x, int y, char *s){

	return Fgl_write2(x,y,0,s,TA_LEFT);
}

void Fgl_fillbox (int x1, int y1, int w, int h, int col)
{
	if (clipping_flag) {
		if (x1 < xclip_x1)
			x1 = xclip_x1;
		if (x1 + w - 1 > xclip_x2)
			w = xclip_x2 - x1 + 1;
		if (y1 < xclip_y1)
			y1 = xclip_y1;
		if (y1 + h - 1 > xclip_y2)
			h = xclip_y2 - y1 + 1;
	}
	col &= 0xff;
	//pixmap_fillbox (x1, y1, w, h, col);

	SDL_Rect dstrect = {.x=x1,.y=y1,.w=w,.h=h};
	SDL_FillRect(display.bg, &dstrect, col);
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

//these look like wrappers for SDL_BlitSurface
void Fgl_putbox_low (SDL_Surface* dst, int x0, int y0, int x1, int y1,
		int w, int h, uint8_t* src, int bpl,
		int src_x, int src_y) {


	//SDL_Rect srcrect = (SDL_Rect){.x = src_x, .y = src_y, .w = w, .h = h};
	//SDL_Rect dstrect = (SDL_Rect){.x = x0+x1, .y = y0+y1, .w = w, .h = h};
	SDL_LockSurface((display.bg));

	for (int iy = 0; iy < h; iy++)
		for (int ix = 0; ix < w; ix++)
			Fgl_setpixel_s(dst,x0+x1+ix,y0+y1+iy,src[(src_y + iy) * bpl + (src_x + ix)]);

	SDL_UnlockSurface((display.bg));
	//SDL_BlitSurface(src,&srcrect,dst,&dstrect);
}

void Fgl_blit (SDL_Surface* dst, int sx, int sy, int w, int h,
		int dx, int dy, SDL_Surface* src) {

	SDL_Rect srcrect = (SDL_Rect){.x = sx, .y = sy, .w = w, .h = h};
	SDL_Rect dstrect = (SDL_Rect){.x = dx, .y = dy, .w = w, .h = h};

	SDL_BlitSurface(src,&srcrect,dst,&dstrect);
}

void Fgl_putbox (int x, int y, int w, int h, void* buf) {

	int c_x0 = clipping_flag ? xclip_x1 : 0;
	int c_x1 = clipping_flag ? xclip_x2 : winW - 1;
	int c_y0 = clipping_flag ? xclip_y1 : 0;
	int c_y1 = clipping_flag ? xclip_y2 : winH - 1;
	int x1 = clamp (x, c_x0, c_x1);
	int y1 = clamp (y, c_y0, c_y1);
	int x2 = clamp (x + w, c_x0, c_x1 + 1);
	int y2 = clamp (y + h, c_y0, c_y1 + 1);


	if (x2 > x1 && y2 > y1)
		Fgl_putbox_low (display.bg, 0, 0, x1, y1, x2 - x1, 
				y2 - y1, (uint8_t*) buf, w, x1 - x, y1 - y);

}


void Fgl_getbox (int x1, int y1, int w, int h, void *buf)
{
	unsigned char *b;
	int x, y;
	b = (unsigned char *) buf;
	for (y = y1; y < y1 + h; y++)
		for (x = x1; x < x1 + w; x++)
			*(b++) = (unsigned char) Fgl_getpixel (x, y);
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
				while (XCheckMaskEvent(display.dpy,ExposureMask,&loop_ev)) {
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

				display.dpy = SDL_SetVideoMode(ev.w,ev.h,8,SDL_HWSURFACE | SDL_RESIZABLE);
				SDL_FreeSurface(display.bg);
				display.bg = SDL_CreateRGBSurface(SDL_HWSURFACE, ev.w, ev.h, 8, 0,0,0,0);
				SDL_FreeSurface(display.sprites);
				display.sprites = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCCOLORKEY, ev.w, ev.h, 8, 0,0,0,0);

				winW = ev.w; winH = ev.h;
				do_setcustompalette(NULL);	
				resize_geometry (ev.w, ev.h);

			}
			break;
		case SDL_QUIT:
			{
				SDL_Quit();
				exit(0);
				break;
			}
		default: {
				 fprintf(stderr,"Unknown event type %d\n",event->type);
				 return;
			 }
	}
	refresh_screen (0,0,winW,winH); // I don't get when exactly should the screen refresh. Neither the X11 nor SVGAlib code are giving me any hints. 
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

	SDL_BlitSurface(display.bg,&orect,display.dpy,&drect);

	if (display.show_sprites) SDL_BlitSurface(display.sprites,&orect,display.dpy,&drect);
	SDL_UpdateRect(display.dpy,x1,y1,x2-x1,y2-y1);
}

	void
Fgl_enableclipping (void)
{
	clipping_flag = 1;
}

	void
Fgl_setclippingwindow (int x1, int y1, int x2, int y2)
{
	xclip_x1 = x1;
	xclip_y1 = y1;
	xclip_x2 = x2;
	xclip_y2 = y2;
}

	void
Fgl_disableclipping (void)
{
	clipping_flag = 0;
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

int lc_setpalettecolor(int x, int r, int g, int b) {

	display.pixcolour_gc[x] = (SDL_Color){.r = r, .g=g,.b=b};
	SDL_SetPalette(display.dpy,SDL_LOGPAL,display.pixcolour_gc,0,256);
	SDL_SetPalette(display.dpy,SDL_PHYSPAL,display.pixcolour_gc,0,256);
	SDL_SetPalette(display.bg,SDL_LOGPAL,display.pixcolour_gc,0,256);
	SDL_SetPalette(display.sprites,SDL_LOGPAL,display.pixcolour_gc,0,256);
	return 0;
}

int lc_loadpalette(uint32_t* pal) {

	if (!pal) return 1;

	for (int i=0; i < 256; i++) {
		
		uint32_t col = pal[i];

		display.pixcolour_gc[i] = (SDL_Color){.r = (col & 0xff0000) >> 16, .g = (col & 0xff00) >> 8, .b = (col & 0xff)};
		do_setcustompalette(NULL);
	}
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


void init_icon_pixmap (short type) {
	unsigned char *g;

	int grp;

	grp = get_group_of_type(type);

	g = (unsigned char *) main_types[type].graphic;
	if (g == NULL) return;

	icon_surface[type] = SDL_CreateRGBSurface(SDL_HWSURFACE,main_groups[grp].size * 16, main_groups[grp].size * 16, 8,0,0,0,0);

	SDL_SetPalette(icon_surface[type], SDL_LOGPAL, display.pixcolour_gc, 0, 256);	
	SDL_SetPalette(icon_surface[type], SDL_PHYSPAL, display.pixcolour_gc, 0, 256);	


	SDL_LockSurface(icon_surface[type]);

	Fgl_putbox_low (icon_surface[type],
			0, 0, 0, 0, main_groups[grp].size * 16, 
			main_groups[grp].size * 16,
			g, main_groups[grp].size * 16,
			0, 0);

	SDL_UnlockSurface(icon_surface[type]);
}

