/* ---------------------------------------------------------------------- *
 * cliglobs.c
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * ---------------------------------------------------------------------- */
#include "cliglobs.h"
#include "common.h"
#include "geometry.h"
#include "lcintl.h"
#include "lcsdl.h"

Screen_Geometry scr;
short mappointoldtype[WORLD_SIDE_LEN][WORLD_SIDE_LEN];

int lc_mouse_type;
char progbox[(PROGBOXW + 16) * (PROGBOXH + 16)];

/* Preferences */
int overwrite_transport_flag = 0;
int suppress_popups = 0;
int time_multiplex_stats = 0;
int no_init_help = 0;
int confine_flag = 0;	/* Only used for X11 */
int skip_splash_screen = 0;
int suppress_firsttime_module_help = 0;

int main_screen_flag = MAIN_SCREEN_NORMAL_FLAG;

int mappoint_stats_flag = 0, mappoint_stats_time = 0;
int mini_screen_flags = MINI_SCREEN_NORMAL_FLAG;
int mini_screen_time = 0;
int mini_screen_port_x, mini_screen_port_y;

float gamma_correct_red = 0.0, gamma_correct_green = 0.0, gamma_correct_blue = 0.0;

int mouse_initialized = 0;
int cs_mouse_x, cs_mouse_y, cs_mouse_button;	/* current mouse status */
int cs_shift_held = 0, cs_ctrl_held=0;
int cs_mouse_shifted = 0;	/* shift key pressed with mouse. */
int cs_mouse_xmax, cs_mouse_ymax, omx, omy, mox = 10, moy = 10;

int mouse_hide_count;
int kmousex, kmousey, kmouse_val, reset_mouse_flag, mt_flag = 0;

char under_square_mouse_pointer_top[20 * 2 * 4];
char under_square_mouse_pointer_left[18 * 2 * 4];
char under_square_mouse_pointer_right[18 * 2 * 4];
char under_square_mouse_pointer_bottom[20 * 2 * 4];
int mouse_type = MOUSE_TYPE_NORMAL;

int mt_start_posx, mt_start_posy, mt_current_posx, mt_current_posy;

char *months[] =
{N_("Jan"), N_("Feb"), N_("Mar"), N_("Apr"),
N_("May"), N_("Jun"), N_("Jul"), N_("Aug"),
N_("Sep"), N_("Oct"), N_("Nov"), N_("Dec")};

int pause_flag = 0, slow_flag = 0, med_flag = 0, fast_flag = 0;

char market_cb_gbuf[MARKET_CB_W * MARKET_CB_H];
int market_cb_flag = 0, mcbx, mcby, market_cb_drawn_flag = 0;
int port_cb_flag = 0, port_cb_drawn_flag = 0;

/* GCS -- this one is difficult to deal with.
   LinCity will give your money back if you bulldoze
   the most recently constructed building (i.e. if 
   you make a mistake.  How does this fit into a 
   multiplayer game?  Not at all, I suppose... */

int help_flag, numof_help_buttons, help_history_count, help_return_val;
int block_help_exit;
int help_button_x[MAX_NUMOF_HELP_BUTTONS];
int help_button_y[MAX_NUMOF_HELP_BUTTONS];
int help_button_w[MAX_NUMOF_HELP_BUTTONS];
int help_button_h[MAX_NUMOF_HELP_BUTTONS];
char help_button_s[MAX_NUMOF_HELP_BUTTONS][MAX_LENOF_HELP_FILENAME];
char help_button_history[MAX_HELP_HISTORY][MAX_LENOF_HELP_FILENAME];
char *help_button_graphic, help_graphic[MAX_ICON_LEN];
char *quit_button_graphic, *load_button_graphic, *save_button_graphic;
char *menu_button_graphic;
int quit_flag, network_flag, load_flag, save_flag;
int prefs_flag = 0, prefs_drawn_flag = 0;
int monument_bul_flag, river_bul_flag, shanty_bul_flag;
int must_release_button = 0, let_one_through = 0;

int db_yesbox_x1, db_yesbox_x2, db_yesbox_y1, db_yesbox_y2;
int db_nobox_x1, db_nobox_x2, db_nobox_y1, db_nobox_y2;
int db_flag, db_yesbox_clicked, db_nobox_clicked;
char okmessage[22][74];
int db_okbox_x1, db_okbox_x2, db_okbox_y1, db_okbox_y2, db_okflag, db_okbox_clicked;
int suppress_ok_buttons;

