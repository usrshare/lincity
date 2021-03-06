/* ---------------------------------------------------------------------- *
 * rocket_pad.c
 * This file is part of lincity.
 * Lincity is copyright (c) I J Peters 1995-1997, (c) Greg Sharp 1997-2001.
 * (c) Corey Keasling, 2004
 * ---------------------------------------------------------------------- */

#include <stdlib.h>
#include <lin-city.h>
#include <lctypes.h>
#include <lcintl.h>
#include <lcconfig.h>
#include <engglobs.h>
#include <cliglobs.h>
#include <stats.h>
#include <screen.h>
#include <mouse.h> /* XXX: for fire_area! */
#include <mps.h>
#include "rocket_pad.h"


void
do_rocket_pad (int x, int y)
{
    /*
      // You need ROCKET_PAD_JOBS, ROCKET_PAD_GOODS and ROCKET_PAD_STEEL 
      // to add 1 to % of ready to fire.
      // int_1 is the stored jobs
      // int_2 is the stored goods
      // int_3 is the stored steel
      // int_4 is the count which gets to ROCKET_PAD_LAUNCH to fire.
      // int_5 is the time of the next animation frame, when waiting for launch.
      */
    if (MP_TYPE(x,y) == CST_ROCKET_FLOWN)
	return;			/* The rocket has been launched. */

    /* get some jobs */
    if (MP_INFO(x,y).int_1 < ROCKET_PAD_JOBS_STORE)
    {
	if (get_jobs (x, y, ROCKET_PAD_JOBS + 10) != 0)
	    MP_INFO(x,y).int_1 += ROCKET_PAD_JOBS;
    }
    /* get goods */
    if (MP_INFO(x,y).int_2 < ROCKET_PAD_GOODS_STORE)
    {
	if (get_goods (x, y, ROCKET_PAD_GOODS + 10) != 0)
	    MP_INFO(x,y).int_2 += ROCKET_PAD_GOODS;
	else if (get_goods (x, y, ROCKET_PAD_GOODS / 10) != 0)
	    MP_INFO(x,y).int_2 += ROCKET_PAD_GOODS / 5;
	else if (get_goods (x, y, ROCKET_PAD_GOODS / 50) != 0)
	    MP_INFO(x,y).int_2 += ROCKET_PAD_GOODS / 20;
    }
    /* get steel */
    if (MP_INFO(x,y).int_3 < ROCKET_PAD_STEEL_STORE)
    {
	if (get_steel (x, y, ROCKET_PAD_STEEL + 10) != 0)
	    MP_INFO(x,y).int_3 += ROCKET_PAD_STEEL + 10;
	else if (get_steel (x, y, ROCKET_PAD_STEEL / 5) != 0)
	    MP_INFO(x,y).int_3 += ROCKET_PAD_STEEL / 5;
	else if (get_steel (x, y, ROCKET_PAD_STEEL / 20) != 0)
	    MP_INFO(x,y).int_3 += ROCKET_PAD_STEEL / 20;
    }
#ifdef DEBUG_ROCKETS
    MP_INFO(x,y).int_4++;
#else
    /* now build the rocket.  Unlike uni's need a full store to make +1% */
    if (MP_TYPE(x,y) < CST_ROCKET_5
	&& MP_INFO(x,y).int_1 >= ROCKET_PAD_JOBS_STORE
	&& MP_INFO(x,y).int_2 >= ROCKET_PAD_GOODS_STORE
	&& MP_INFO(x,y).int_3 >= ROCKET_PAD_STEEL_STORE)
    {
	MP_INFO(x,y).int_1 -= ROCKET_PAD_JOBS_STORE;
	MP_INFO(x,y).int_2 -= ROCKET_PAD_GOODS_STORE;
	MP_INFO(x,y).int_3 -= ROCKET_PAD_STEEL_STORE;
	MP_INFO(x,y).int_4++;
	goods_used += ROCKET_PAD_GOODS_STORE;

    }
#endif
    rocket_pad_cost += ROCKET_PAD_RUNNING_COST;
    /* animate and return if already said no to launch */
    if (MP_TYPE(x,y) >= CST_ROCKET_5
	&& MP_INFO(x,y).int_4 >= (100 * ROCKET_PAD_LAUNCH) / 100)
    {
	if (real_time >= MP_INFO(x,y).int_5)
	{
	    MP_INFO(x,y).int_5 = real_time + ROCKET_ANIMATION_SPEED;
	    switch (MP_TYPE(x,y))
	    {
	    case (CST_ROCKET_5):
		MP_TYPE(x,y) = CST_ROCKET_6;
		break;
	    case (CST_ROCKET_6):
		MP_TYPE(x,y) = CST_ROCKET_7;
		break;
	    case (CST_ROCKET_7):
		MP_TYPE(x,y) = CST_ROCKET_5;
		break;
	    }
	}
	return;
    }
    /* now choose a graphic */
    if (MP_INFO(x,y).int_4 < (25 * ROCKET_PAD_LAUNCH) / 100)
	MP_TYPE(x,y) = CST_ROCKET_1;
    else if (MP_INFO(x,y).int_4 < (60 * ROCKET_PAD_LAUNCH) / 100)
	MP_TYPE(x,y) = CST_ROCKET_2;
    else if (MP_INFO(x,y).int_4 < (90 * ROCKET_PAD_LAUNCH) / 100)
	MP_TYPE(x,y) = CST_ROCKET_3;
    else if (MP_INFO(x,y).int_4 < (100 * ROCKET_PAD_LAUNCH) / 100)
	MP_TYPE(x,y) = CST_ROCKET_4;
    else if (MP_INFO(x,y).int_4 >= (100 * ROCKET_PAD_LAUNCH) / 100) {
	MP_TYPE(x,y) = CST_ROCKET_5;
	update_main_screen (0);
	if (ask_launch_rocket_now (x,y)) {
	    launch_rocket (x,y);
	}
	/* so we don't get get our money back when we bulldoze. */
	if (x == last_built_x && y == last_built_y) {
	    x = 0;
	    y = 0;
	}
    }
}

void
launch_rocket (int x, int y)
{
    int i, r, xx, yy, xxx, yyy;
    rockets_launched++;
    MP_TYPE(x,y) = CST_ROCKET_FLOWN;
    update_main_screen (0);
    r = rand () % MAX_TECH_LEVEL;
    if (r > tech_level || rand () % 100 > (rockets_launched * 15 + 25)) {
	/* the launch failed */
	display_rocket_result_dialog (ROCKET_LAUNCH_BAD);
	rockets_launched_success = 0;
	xx = ((rand () % 40) - 20) + x;
	yy = ((rand () % 40) - 20) + y;
	for (i = 0; i < 20; i++) {
	    xxx = ((rand () % 20) - 10) + xx;
	    yyy = ((rand () % 20) - 10) + yy;
	    if (xxx > 0 && xxx < WORLD_SIDE_LEN	
		&& yyy > 0 && yyy < WORLD_SIDE_LEN) {
		/* don't crash on it's own area */
		if (xxx >= x && xxx < (x + 4) && yyy >= y && yyy < (y + 4))
		    continue;
		fire_area (xxx, yyy);
		/* make a sound perhaps */
	    }
	}
    } else {
	rockets_launched_success++;
	if (rockets_launched_success > 5) {
	    remove_people (1000);
	    if (people_pool || housed_population)
	      display_rocket_result_dialog (ROCKET_LAUNCH_EVAC);
	} else {
	    display_rocket_result_dialog (ROCKET_LAUNCH_GOOD);
	}
    }
}

void
mps_rocket (int x, int y)
{
    int i = 0;

    mps_store_title(i++,_("Rocket Pad"));
    i++;

    mps_store_title(i++,_("Completion"));
    mps_store_fp(i++, MP_INFO(x,y).int_4 * 100.0 / ROCKET_PAD_LAUNCH);    
    i++;
    mps_store_title(i++,_("Inventory"));
    mps_store_sfp(i++,_("Jobs"), 
		  MP_INFO(x,y).int_1 * 100.0 / ROCKET_PAD_JOBS_STORE);
    mps_store_sfp(i++,_("Goods"),
		  MP_INFO(x,y).int_2 * 100.0 / ROCKET_PAD_GOODS_STORE);
    mps_store_sfp(i++,_("Steel"),
		  MP_INFO(x,y).int_3 * 100.0 / ROCKET_PAD_STEEL_STORE);

}
