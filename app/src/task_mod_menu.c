/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *
 * @file   : task_menu.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "task_actuator_attribute.h"
#include "task_menu_attribute.h"
#include "task_menu_interface.h"
#include "display.h"
/********************** macros and definitions *******************************/
#define G_TASK_MEN_CNT_INI            0ul
#define G_TASK_MEN_TICK_CNT_INI       0ul

#define DEL_MEN_XX_MIN                0ul
#define DEL_MEN_XX_MED                300ul
#define DEL_MEN_XX_MAX                500ul

#define TEMP_MIN                      25
#define TEMP_MAX                      100
#define TEMP_STEP                     5

/********************** internal data declaration ****************************/
task_menu_dta_t task_menu_dta =
	{DEL_MEN_XX_MIN, ST_MEN_XX_IDLE, EV_MEN_ENT_IDLE, false};

#define MENU_DTA_QTY (sizeof(task_menu_dta)/sizeof(task_menu_dta_t))

/********************** internal data definition *****************************/
const char *p_task_menu 		= "Task Menu (Interactive Menu)";
const char *p_task_menu_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_menu_cnt;
volatile uint32_t g_task_menu_tick_cnt;

/* FSM states */
typedef enum {
    ST_MENU_IDLE,
    ST_MENU_SECTOR,
    ST_MENU_HUMO,
    ST_MENU_TEMP_EN,
    ST_MENU_TEMP_SET
} menu_state_t;

menu_state_t menu_state = ST_MENU_IDLE;
uint8_t current_sector = 0;

void task_menu_init(void *parameters)
{
	task_menu_dta_t *p_task_menu_dta;
	task_menu_st_t	state;
	task_system_ev_t	event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_menu_init), p_task_menu);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_menu), p_task_menu_);

	g_task_menu_cnt = G_TASK_MEN_CNT_INI;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %lu\r\n", GET_NAME(g_task_menu_cnt), g_task_menu_cnt);

	init_queue_event_task_menu();

	/* Update Task Actuator Configuration & Data Pointer */
	p_task_menu_dta = &task_menu_dta;

	/* Print out: Task execution FSM */
	state = p_task_menu_dta->state;
	LOGGER_LOG("   %s = %lu", GET_NAME(state), (uint32_t)state);

	event = p_task_menu_dta->event;
	LOGGER_LOG("   %s = %lu", GET_NAME(event), (uint32_t)event);

	b_event = p_task_menu_dta->flag;
	LOGGER_LOG("   %s = %s\r\n", GET_NAME(b_event), (b_event ? "true" : "false"));

	cycle_counter_init();
	cycle_counter_reset();

	g_task_menu_tick_cnt = G_TASK_MEN_TICK_CNT_INI;
}

void task_menu_update(void *parameters)
{
    task_menu_dta_t *p_task_menu_dta;
    bool b_time_update_required = false;
    char line1[17] = {0};
    char line2[17] = {0};

    /* Update Task Menu Counter */
    g_task_menu_cnt++;
    /* Protect shared resource (g_task_menu_tick) */
    __asm("CPSID i");
    if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
    {
        g_task_menu_tick_cnt--;
        b_time_update_required = true;
    }
    __asm("CPSIE i");

    while (b_time_update_required)
    {
        /* Protect shared resource again */
        __asm("CPSID i");
        if (G_TASK_MEN_TICK_CNT_INI < g_task_menu_tick_cnt)
        {
            g_task_menu_tick_cnt--;
            b_time_update_required = true;
        }
        else
        {
            b_time_update_required = false;
        }
        __asm("CPSIE i");

        /* Pointer to shared task menu data */
        p_task_menu_dta = &task_menu_dta;

        if (DEL_MEN_XX_MIN < p_task_menu_dta->tick)
        {
            p_task_menu_dta->tick--;
        }
        else
        {
            p_task_menu_dta->tick = DEL_MEN_XX_MED;

            if (any_event_task_menu())
            {
                p_task_menu_dta->flag = true;
                p_task_menu_dta->event = get_event_task_menu();
            }

            switch (menu_state)
            {
                case ST_MENU_IDLE:
                    if (p_task_menu_dta->event == EV_MEN_ENT_ACTIVE)
                    {
                        menu_state = ST_MENU_SECTOR;
                        current_sector = 0;
                    }
                    snprintf(line1, sizeof(line1), "Config. Menu");
                    snprintf(line2, sizeof(line2), "<ENTER> Start");
                    break;

                case ST_MENU_SECTOR:
                    if (p_task_menu_dta->event == EV_MEN_NEX_ACTIVE)
                        current_sector = (current_sector + 1) % SECTOR_COUNT;
                    else if (p_task_menu_dta->event == EV_MEN_ENT_ACTIVE)
                        menu_state = ST_MENU_HUMO;
                    else if (p_task_menu_dta->event == EV_MEN_ESC_ACTIVE)
                        menu_state = ST_MENU_IDLE;
                    snprintf(line1, sizeof(line1), "Select Sector:");
                    snprintf(line2, sizeof(line2), "Sector: %u", current_sector + 1);
                    break;

                case ST_MENU_HUMO:
                    if (p_task_menu_dta->event == EV_MEN_NEX_ACTIVE)
                        sector_cfg[current_sector].humo_en = !sector_cfg[current_sector].humo_en;
                    else if (p_task_menu_dta->event == EV_MEN_ENT_ACTIVE)
                        menu_state = ST_MENU_TEMP_EN;
                    else if (p_task_menu_dta->event == EV_MEN_ESC_ACTIVE)
                        menu_state = ST_MENU_SECTOR;
                    snprintf(line1, sizeof(line1), "Sensor Humo :");
                    snprintf(line2, sizeof(line2), "Status: %s", sector_cfg[current_sector].humo_en ? "YES" : "NO ");
                    break;

                case ST_MENU_TEMP_EN:
                    if (p_task_menu_dta->event == EV_MEN_NEX_ACTIVE)
                        sector_cfg[current_sector].temp_en = !sector_cfg[current_sector].temp_en;
                    else if (p_task_menu_dta->event == EV_MEN_ENT_ACTIVE)
                    {
                        if (sector_cfg[current_sector].temp_en)
                            menu_state = ST_MENU_TEMP_SET;
                        else
                            menu_state = ST_MENU_HUMO;
                    }
                    else if (p_task_menu_dta->event == EV_MEN_ESC_ACTIVE)
                        menu_state = ST_MENU_HUMO;
                    snprintf(line1, sizeof(line1), "Temp Control:");
                    snprintf(line2, sizeof(line2), "Enabled: %s", sector_cfg[current_sector].temp_en ? "YES" : "NO ");
                    break;

                case ST_MENU_TEMP_SET:
                    if (p_task_menu_dta->event == EV_MEN_ENT_ACTIVE)
                    {
                        sector_cfg[current_sector].temp_max += TEMP_STEP;
                        if (sector_cfg[current_sector].temp_max > TEMP_MAX)
                            sector_cfg[current_sector].temp_max = TEMP_MIN;
                    }
                    else if (p_task_menu_dta->event == EV_MEN_NEX_ACTIVE)
                    {
                        if (sector_cfg[current_sector].temp_max > TEMP_MIN)
                            sector_cfg[current_sector].temp_max -= TEMP_STEP;
                        else
                            sector_cfg[current_sector].temp_max = TEMP_MAX;
                    }
                    else if (p_task_menu_dta->event == EV_MEN_ESC_ACTIVE)
                        menu_state = ST_MENU_TEMP_EN;

                    snprintf(line1, sizeof(line1), "Max Temp:");
                    snprintf(line2, sizeof(line2), "%u C", sector_cfg[current_sector].temp_max);
                    break;
            }


            displayCharPositionWrite(0, 0);
            displayStringWrite("                ");
            displayCharPositionWrite(0, 0);
            displayStringWrite(line1);

            displayCharPositionWrite(0, 1);
            displayStringWrite("                ");
            displayCharPositionWrite(0, 1);
            displayStringWrite(line2);


            p_task_menu_dta->flag = false;
        }
    }
}


