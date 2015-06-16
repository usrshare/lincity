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
#include <SDL_ttf.h>

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
	SDL_Surface *dpy; //screen
	SDL_Surface *bg; //background
	SDL_Surface *ui; //background
	SDL_Surface *sprites; //square mouse.

	int show_ui;
	int show_sprites;
	int screen;
	char *dname;

	SDL_Color bg_xcolor;
	SDL_Color pixcolour_gc[256];
	SDL_Palette* cmap;

	int pointer_confined;
}
disp;

enum text_align {
	TA_LEFT,
	TA_CENTER,
	TA_RIGHT,
};

extern int winW, winH;
extern int mouse_button, cs_mouse_shifted;
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

extern SDL_Surface *up_pbar1_graphic, *up_pbar2_graphic;
extern SDL_Surface *down_pbar1_graphic, *down_pbar2_graphic, *pop_pbar_graphic;
extern SDL_Surface *tech_pbar_graphic, *food_pbar_graphic, *jobs_pbar_graphic;
extern SDL_Surface *money_pbar_graphic, *coal_pbar_graphic, *goods_pbar_graphic;
extern SDL_Surface *ore_pbar_graphic, *steel_pbar_graphic;
extern SDL_Surface *pause_button1_off, *pause_button2_off;
extern SDL_Surface *pause_button1_on, *pause_button2_on;
extern SDL_Surface *fast_button1_off, *fast_button2_off;
extern SDL_Surface *fast_button1_on, *fast_button2_on;
extern SDL_Surface *med_button1_off, *med_button2_off;
extern SDL_Surface *med_button1_on, *med_button2_on;
extern SDL_Surface *slow_button1_off, *slow_button2_off;
extern SDL_Surface *slow_button1_on, *slow_button2_on;
extern SDL_Surface *results_button1, *results_button2;
extern SDL_Surface *toveron_button1, *toveron_button2;
extern SDL_Surface *toveroff_button1, *toveroff_button2;
extern SDL_Surface *ms_pollution_button_graphic, *ms_normal_button_graphic;
extern SDL_Surface *ms_fire_cover_button_graphic, *ms_health_cover_button_graphic;
extern SDL_Surface *ms_cricket_cover_button_graphic;
extern SDL_Surface *ms_ub40_button_graphic, *ms_coal_button_graphic;
extern SDL_Surface *ms_starve_button_graphic, *ms_ocost_button_graphic;
extern SDL_Surface *ms_power_button_graphic;
extern SDL_Surface *checked_box_graphic, *unchecked_box_graphic;

void Fgl_write (int, int, char *);
void open_write (int, int, char *);
//extern void Fgl_getbox (int, int, int, int, void *);
//extern void Fgl_putbox (int, int, int, int, void *);
void Fgl_putbox (int x, int y, int w, int h, SDL_Surface* surf);
void Fgl_fillbox_s (SDL_Surface* surf, int x1, int y1, int w, int h, int col);
void Fgl_fillbox (int, int, int, int, int);
void Fgl_hline_s (SDL_Surface* surf, int x1, int y1, int x2, int col);
void Fgl_line_s (SDL_Surface* surf, int x1, int y1, int dummy, int y2, int col);
void Fgl_hline (int, int, int, int);
void Fgl_line (int, int, int, int, int);
void Fgl_setpixel (int, int, int);
void Fgl_setfontcolors (int, int);
void Fgl_setfont (int, int, void*);
void Fgl_enableclipping (void);
void Fgl_setclippingwindow (int, int, int, int);
void Fgl_disableclipping (void);

void parse_sdlargs (int, char **, char **);
void set_pointer_confinement (void);
void do_setcustompalette (SDL_Color *);
void Create_Window (char *);
void HandleError (char *, int);
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
void init_icon_pixmap (short type);

int lc_get_keystroke (void);
int lc_setpalettecolor(int x, int r, int g, int b);
int lc_loadpalette(uint32_t* pal);

int lc_loadfont(int fontid, char* filename);
int lc_txtwidth(char *s);

void draw_small_bezel_s (SDL_Surface* surf, int x, int y, int w, int h, int colour);
void draw_bezel_s (SDL_Surface* surf, Rect r, short width, int color);

void Fgl_blit (SDL_Surface* dst, int sx, int sy, int w, int h,
		int dx, int dy, SDL_Surface* src);

void Fgl_write2_s (SDL_Surface* surf, int x, int y, int w, char *s, enum text_align align);
void Fgl_write2 (int x, int y, int w, char *s, enum text_align align);

extern TTF_Font* eight_font;
extern TTF_Font* sixteen_font;
extern SDL_Surface* icon_surface[NUM_OF_TYPES];
extern char icon_surface_flag[NUM_OF_TYPES];

SDL_Surface* load_graphic(char *s, int w, int h);

#endif /* __lcsdl_h__ */
