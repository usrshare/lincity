/* ---------------------------------------------------------------------- *
 * cliglobs.h
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * ---------------------------------------------------------------------- */
#ifndef __cliglobs_h__
#define __cliglobs_h__

#include "common.h"
#include "lin-city.h"
#include "geometry.h"

extern Screen_Geometry scr;
extern short mappointoldtype[WORLD_SIDE_LEN][WORLD_SIDE_LEN];

extern int lc_mouse_type;
extern char progbox[(PROGBOXW + 16) * (PROGBOXH + 16)];

/* Preferences */
extern int time_multiplex_stats;
extern int suppress_popups;
extern int overwrite_transport_flag;
extern int confine_flag;
extern int skip_splash_screen;
extern int no_init_help;
extern int suppress_firsttime_module_help;

extern int main_screen_flag;
extern int mappoint_stats_flag, mappoint_stats_time;
extern int mini_screen_flags, mini_screen_time;
extern int mini_screen_port_x, mini_screen_port_y;

extern float gamma_correct_red, gamma_correct_green, gamma_correct_blue;

extern int mouse_initialized; /* wcoreyk */
extern int cs_mouse_x, cs_mouse_y, cs_mouse_button;	/* current mouse status */
extern int cs_shift_held, cs_ctrl_held;
extern int cs_mouse_shifted;	/* shift key pressed with mouse. */
extern int cs_mouse_xmax, cs_mouse_ymax, omx, omy, mox, moy;

extern int mouse_hide_count;
extern int kmousex, kmousey, kmouse_val, reset_mouse_flag, mt_flag;
extern int modern_windmill_flag;

extern char under_square_mouse_pointer_top[20 * 2 * 4];
extern char under_square_mouse_pointer_left[18 * 2 * 4];
extern char under_square_mouse_pointer_right[18 * 2 * 4];
extern char under_square_mouse_pointer_bottom[20 * 2 * 4];
extern int mouse_type;

extern int mt_start_posx, mt_start_posy, mt_current_posx, mt_current_posy;
extern int main_screen_originx, main_screen_originy;

extern long real_time;
extern int screen_refresh_flag;

extern int total_money, income_tax_rate, coal_tax_rate;
extern int dole_rate, transport_cost_rate;
extern int goods_tax_rate;
extern int export_tax_rate, import_cost_rate;
extern int tech_level, highest_tech_level, unnat_deaths;

extern char *months[];

extern int monthgraph_size;
extern int *monthgraph_pop;
extern int *monthgraph_starve;
extern int *monthgraph_nojobs;
extern int *monthgraph_ppool;

extern int pause_flag, slow_flag, med_flag, fast_flag;
extern int prefs_flag;
extern int prefs_drawn_flag;

extern char market_cb_gbuf[MARKET_CB_W * MARKET_CB_H];
extern int market_cb_flag, mcbx, mcby, market_cb_drawn_flag;
extern int port_cb_flag, port_cb_drawn_flag;

#if defined (commentout)
extern int diffgraph_power[MAPPOINT_STATS_W], diffgraph_coal[MAPPOINT_STATS_W];
extern int diffgraph_goods[MAPPOINT_STATS_W], diffgraph_ore[MAPPOINT_STATS_W];
extern int diffgraph_population[MAPPOINT_STATS_W], diff_old_population;
#endif

/* GCS -- this one is difficult to deal with.
   LinCity will give your money back if you bulldoze
   the most recently constructed building (i.e. if 
   you make a mistake.  How does this fit into a 
   multiplayer game?  Not at all, I suppose... */
extern int last_built_x, last_built_y;

extern int help_flag, numof_help_buttons, help_history_count, help_return_val;
extern int block_help_exit;
extern int help_button_x[MAX_NUMOF_HELP_BUTTONS];
extern int help_button_y[MAX_NUMOF_HELP_BUTTONS];
extern int help_button_w[MAX_NUMOF_HELP_BUTTONS];
extern int help_button_h[MAX_NUMOF_HELP_BUTTONS];
extern char help_button_s[MAX_NUMOF_HELP_BUTTONS][MAX_LENOF_HELP_FILENAME];
extern char help_button_history[MAX_HELP_HISTORY][MAX_LENOF_HELP_FILENAME];
extern char *help_button_graphic, help_graphic[MAX_ICON_LEN];
extern char *quit_button_graphic, *load_button_graphic, *save_button_graphic;
extern char *menu_button_graphic;
extern int quit_flag, network_flag, load_flag, save_flag, cheat_flag;
extern int monument_bul_flag, river_bul_flag, shanty_bul_flag;
extern int must_release_button, let_one_through;

extern int db_yesbox_x1, db_yesbox_x2, db_yesbox_y1, db_yesbox_y2;
extern int db_nobox_x1, db_nobox_x2, db_nobox_y1, db_nobox_y2;
extern int db_flag, db_yesbox_clicked, db_nobox_clicked;
extern char okmessage[22][74];
extern int db_okbox_x1, db_okbox_x2, db_okbox_y1, db_okbox_y2, db_okflag, db_okbox_clicked;
extern int suppress_ok_buttons;

extern int pbar_pops[12], pbar_pop_oldtot, pop_diff, pbar_pop_olddiff;
extern int pbar_techs[12], pbar_tech_oldtot, tech_diff, pbar_tech_olddiff;
extern int pbar_foods[12], pbar_food_oldtot, food_diff, pbar_food_olddiff;
extern int pbar_jobs[12], pbar_jobs_oldtot, jobs_diff, pbar_jobs_olddiff;
extern int pbar_coal[12], pbar_coal_oldtot, coal_diff, pbar_coal_olddiff;
extern int pbar_goods[12], pbar_goods_oldtot, goods_diff, pbar_goods_olddiff;
extern int pbar_ore[12], pbar_ore_oldtot, ore_diff, pbar_ore_olddiff;
extern int pbar_steel[12], pbar_steel_oldtot, steel_diff, pbar_steel_olddiff;
extern int pbar_money[12], pbar_money_oldtot, money_diff, pbar_money_olddiff;

extern int askdir_lines;
extern char *askdir_path[4];
extern int lc_save_dir_len;


extern struct _disp display;
extern int borderx, bordery;

extern int command_line_debug;

#if defined (LC_SDL)
struct SDL_Surface;
extern SDL_Surface* icon_surface[NUM_OF_TYPES];
#endif

#endif /* __cliglobs_h__ */
