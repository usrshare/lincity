/* ---------------------------------------------------------------------- *
 * lcsdl.h
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * Based on lcx11.h, changes (c) usr_share 2015
 * ---------------------------------------------------------------------- */
#ifndef __lcsdl_h__
#define __lcsdl_h__

#include "lin-city.h"
#include <SDL.h>

#define TRUE 1
#define FALSE 0

#if defined (commentout)
#define MOUSE_LEFTBUTTON Button1
#define MOUSE_MIDDLEBUTTON Button2
#define MOUSE_RIGHTBUTTON Button3
#endif

#define WINWIDTH 640		/* default window width */
#define WINHEIGHT 480		/* default window height */


/* Type Definitions */
typedef struct _disp
{
    SDL_Surface *dpy;
    int screen;
    char *dname;

    //Window win;
    //Window confinewin;
    //Window root;

    unsigned int winH;
    unsigned int winW;

    long bg;			/* colors */
    SDL_Color bg_xcolor;
    SDL_Color pixcolour_gc[256];
    //Atom kill_atom, protocol_atom;
    SDL_Palette* cmap;

    int pointer_confined;
}
disp;

extern int winX, winY, mouse_button, cs_mouse_shifted;
extern disp display;

extern char *bg_color;
extern int verbose;
extern int pix_double;
extern int text_bg;
extern int text_fg;
extern int x_key_value;
extern int x_key_shifted;
extern int borderx, bordery;
extern long unsigned int colour_table[256];
extern int xclip_x1, xclip_y1, xclip_x2, xclip_y2, clipping_flag;
extern unsigned char *open_font;
extern int open_font_height;
extern int suppress_next_expose;
extern int cs_mouse_x, cs_mouse_y;
extern float gamma_correct_red, gamma_correct_green, gamma_correct_blue;
//extern Cursor pirate_cursor;

void parse_sdlargs (int, char **, char **);
void set_pointer_confinement (void);
void do_setcustompalette (SDL_Color *);
void Create_Window (char *);
void HandleError (char *, int);
void HandleEvent (SDL_Event *);
void refresh_screen (int, int, int, int);
void my_x_putchar (int, int, unsigned char);
void open_x_putchar (int, int, unsigned char);
void do_call_event (int);
void call_event (void);
void call_wait_event (void);
void open_setcustompalette (SDL_Color *);
void drag_screen(void); /* WCK */
void draw_border (void);
void init_x_mouse (void);
void unlock_window_size (void);

int lc_get_keystroke (void);
int lc_setpalettecolor(int x, int r, int g, int b);
int lc_loadpalette(uint32_t* pal);

void Fgl_blit (SDL_Surface* dst, int sx, int sy, int w, int h,
		int dx, int dy, SDL_Surface* src);


extern SDL_Surface* icon_surface[];

extern SDL_Surface* font_surface;

#endif /* __lcsdl_h__ */
