/* ---------------------------------------------------------------------- *
 * lcsdl.h
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * Based on lcx11.h, changes (c) usr_share 2015
 * ---------------------------------------------------------------------- */
#ifndef __lcsdl_h__
#define __lcsdl_h__

#include <stdint.h>
#include <stdio.h>
#include "lin-city.h"
#include "lintypes.h"

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
	int show_ui;
	int show_sprites;
	int screen;
	char *dname;

	int pointer_confined;
}
disp;

enum disp_layers {
	DL_DIRECT = -1, //do not use
	DL_BG = 0,
	DL_UI,
	DL_SPRITES,
};

enum text_fonts {
	TF_DEFAULT = 0,
	TF_MONO = 1,
	TF_LARGE = 2,
};

enum text_align {
	TA_LEFT,
	TA_CENTER,
	TA_RIGHT,
};

typedef struct _lcsdl_icon {

	//this is meant to represent a single graphic, either
	//in a surface of its own, or as part of a big spritesheet.

	void* _surface; //stores a pointer to an SDL_Surface.
	int x_offset;
	int y_offset;
	int width; // primarily used for icons
	int height;

} lcicon;

struct rect_struct
{
    int x;
    int y;
    int w;
    int h;
};
typedef struct rect_struct Rect;

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

extern lcicon type_icons[NUM_OF_TYPES];

extern lcicon up_pbar1_graphic, up_pbar2_graphic;
extern lcicon down_pbar1_graphic, down_pbar2_graphic, pop_pbar_graphic;
extern lcicon tech_pbar_graphic, food_pbar_graphic, jobs_pbar_graphic;
extern lcicon money_pbar_graphic, coal_pbar_graphic, goods_pbar_graphic;
extern lcicon ore_pbar_graphic, steel_pbar_graphic;
extern lcicon pause_button1_off, pause_button2_off;
extern lcicon pause_button1_on, pause_button2_on;
extern lcicon fast_button1_off, fast_button2_off;
extern lcicon fast_button1_on, fast_button2_on;
extern lcicon med_button1_off, med_button2_off;
extern lcicon med_button1_on, med_button2_on;
extern lcicon slow_button1_off, slow_button2_off;
extern lcicon slow_button1_on, slow_button2_on;
extern lcicon results_button1, results_button2;
extern lcicon toveron_button1, toveron_button2;
extern lcicon toveroff_button1, toveroff_button2;
extern lcicon ms_pollution_button_graphic, ms_normal_button_graphic;
extern lcicon ms_fire_cover_button_graphic, ms_health_cover_button_graphic;
extern lcicon ms_cricket_cover_button_graphic;
extern lcicon ms_ub40_button_graphic, ms_coal_button_graphic;
extern lcicon ms_starve_button_graphic, ms_ocost_button_graphic;
extern lcicon ms_power_button_graphic;
extern lcicon checked_box_graphic, unchecked_box_graphic;

//extern SDL_Color t_bgcolor, t_fgcolor;

void Fgl_write (int, int, char *);
void Fgl_blit (enum disp_layers l, lcicon i, int dx, int dy, int w, int h);
void Fgl_putbox (int x, int y, int w, int h, lcicon i);
void Fgl_fillbox_s (enum disp_layers l, int x1, int y1, int w, int h, int col);
void Fgl_fillbox (int, int, int, int, int);
void Fgl_hline_s (enum disp_layers l, int x1, int y1, int x2, int col);
void Fgl_line_s (enum disp_layers l, int x1, int y1, int dummy, int y2, int col);
void Fgl_hline (int, int, int, int);
void Fgl_line (int, int, int, int, int);
void Fgl_setpixel (int, int, int);
void Fgl_setfontcolors (int, int);
void Fgl_setfont (int, int, void*);
void Fgl_enableclipping (void);
void Fgl_setclippingwindow (int, int, int, int);
void Fgl_disableclipping (void);

void parse_sdlargs (int, char **, char **);
void lcsdl_load_fonts(void);
void set_pointer_confinement (void);
void Create_Window (char *);
void HandleError (char *, int);
void refresh_screen (int, int, int, int);
void refresh_rect(Rect* r);
void my_x_putchar (int, int, unsigned char);
void open_x_putchar (int, int, unsigned char);
void do_call_event (int);
void call_event (void);
void call_wait_event (void);
void drag_screen(void); /* WCK */
void draw_border (void);
void init_x_mouse (void);
void unlock_window_size (void);

int lc_get_keystroke (void);
int lc_setpalettecolor(int x, int r, int g, int b);
int lc_setpalettecolors(int n, uint8_t* colors);
int lc_loadpalette(uint32_t* pal);

int lc_loadfont(int fontid, char* filename);
int lc_txtwidth(enum text_fonts f, char *s);

void draw_small_bezel_s (enum disp_layers l, int x, int y, int w, int h, int colour);
void draw_bezel_s (enum disp_layers l, Rect r, short width, int color);

void Fgl_write3 (enum disp_layers l, enum text_fonts f, int x, int y, int w, char *s, enum text_align align);
void Fgl_write2_s (enum disp_layers l, int x, int y, int w, char *s, enum text_align align);
void Fgl_write2 (int x, int y, int w, char *s, enum text_align align);

void init_icon_pixmap (short type);

lcicon load_icon(char* filename, int w, int h); // load an icon from a separate PNG file
int free_icon(lcicon i);

int load_tilemap (char* png_filename, uint32_t* out_palette); //load the tilemap
int load_tilemap_graphic (short type, short group, char* id, FILE* txt_fp); //load an icon from the tilemap


#endif /* __lcsdl_h__ */
