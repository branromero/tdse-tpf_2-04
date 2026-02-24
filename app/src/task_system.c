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
 * @file   : task_system.c
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
#include "task_actuator_interface.h"
#include "task_mod_menu.h"
#include "task_menu_interface.h"
#include "task_normal_interface.h"
#include "display.h"
#include "task_mod_normal.h"
#include "eeprom_interface.h"
/********************** macros and definitions *******************************/
#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul
#define TEMP_MAX                    100
#define DEL_SYS_XX_MIN				0ul
#define DEL_SYS_XX_MED				50ul
#define DEL_SYS_XX_MAX				500ul
/********************** internal data declaration ****************************/
task_system_dta_t task_system_dta =
	{DEL_SYS_XX_MIN, ST_SYS_MOD_NORMAL, EV_MEN_GET_IN_GET_ON_IDLE, false};
sector_config_t sector_normal_start_cfg = {false,false,TEMP_MAX};
#define SYSTEM_DTA_QTY	(sizeof(task_system_dta)/sizeof(task_system_dta_t))
/********************** internal functions declaration ***********************/
//uint32_t errores;
/********************** internal data definition *****************************/
const char *p_task_system 		= "Task System (System Statechart)";
const char *p_task_system_ 		= "Non-Blocking & Update By Time Code";
/********************** external data declaration ****************************/
uint32_t g_task_system_cnt;
volatile uint32_t g_task_system_tick_cnt;
/********************** external functions definition ************************/
void task_system_init(void *parameters)
{
	task_system_dta_t 	*p_task_system_dta;
	task_system_st_t	state;
	task_system_ev_t	event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_LOG("  %s is running - %s\r\n", GET_NAME(task_system_init), p_task_system);
	LOGGER_LOG("  %s is a %s\r\n", GET_NAME(task_system), p_task_system_);

	g_task_system_cnt = G_TASK_SYS_CNT_INI;

	/* Print out: Task execution counter */
	LOGGER_LOG("   %s = %lu\r\n", GET_NAME(g_task_system_cnt), g_task_system_cnt);

	init_queue_event_task_system();

	/* Update Task Actuator Configuration & Data Pointer */
	p_task_system_dta = &task_system_dta;

	/* Print out: Task execution FSM */
	state = p_task_system_dta->state;
	LOGGER_LOG("   %s = %lu", GET_NAME(state), (uint32_t)state);

	event = p_task_system_dta->event;
	LOGGER_LOG("   %s = %lu", GET_NAME(event), (uint32_t)event);

	b_event = p_task_system_dta->flag;
	LOGGER_LOG("   %s = %s\r\n", GET_NAME(b_event), (b_event ? "true" : "false"));

	task_normal_init(NULL);
	displayInit( DISPLAY_CONNECTION_GPIO_4BITS );
	g_task_system_tick_cnt = G_TASK_SYS_TICK_CNT_INI;
	eeprom_load_sector_config();
}


void task_system_update(void *parameters)
{
	task_system_dta_t *p_task_system_dta;
	bool b_time_update_required = false;
	/* Update Task System Counter */
	g_task_system_cnt++;

	/* Protect shared resource (g_task_system_tick) */
	__asm("CPSID i");	/* disable interrupts*/
    if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt)
    {
    	g_task_system_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts*/

    while (b_time_update_required)
    {
		/* Protect shared resource (g_task_system_tick) */
		__asm("CPSID i");	/* disable interrupts*/
		if (G_TASK_SYS_TICK_CNT_INI < g_task_system_tick_cnt)
		{
			g_task_system_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts*/

    	/* Update Task System Data Pointer */
		p_task_system_dta = &task_system_dta;

		if (true == any_event_task_system())
		{
			p_task_system_dta->flag = true;
			p_task_system_dta->event = get_event_task_system();
		}
		if(rom_corrupcion_flag == true){
			//errores++;
			if(p_task_system_dta->event==EV_MEN_ESC_ACTIVE){
				p_task_system_dta->flag = false;
				rom_corrupcion_flag = false;
			    for (uint8_t index = 0; index < SECTOR_COUNT; index++)
			    {
			        sector_cfg[index] = sector_normal_start_cfg;
			    }
				eeprom_save_sector_config();
			}
		}else{
		switch (p_task_system_dta->state)
		{
		/***************************** ESTADOS DEL PROGRAMA ********************************/
		case ST_SYS_MOD_NORMAL:

				if ((true == p_task_system_dta->flag) && (EV_MEN_GET_IN_GET_ON_ACTIVE == p_task_system_dta->event))
				{
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_LED_XX_OFF, ID_LED_MOD_NORMAL);
					put_event_task_actuator(EV_LED_XX_BLINK, ID_LED_MOD_CONF);
					task_menu_init(NULL);
					p_task_system_dta->state = ST_SYS_MOD_MENU;
					eeprom_load_sector_config();
				}else{
					put_event_task_actuator(EV_LED_XX_ON, ID_LED_MOD_NORMAL);
					put_event_task_normal( p_task_system_dta->event);
					task_normal_update(NULL);
				}

				break;
			case ST_SYS_MOD_MENU:

				if ((true == p_task_system_dta->flag) && (EV_MEN_GET_IN_GET_ON_ACTIVE == p_task_system_dta->event))
				{
					p_task_system_dta->flag = false;
					put_event_task_actuator(EV_LED_XX_NOT_BLINK, ID_LED_MOD_CONF);
					eeprom_save_sector_config();
					p_task_system_dta->state = ST_SYS_MOD_NORMAL;

				}
				 if ((true == p_task_system_dta->flag) && (EV_MEN_GET_IN_GET_ON_ACTIVE != p_task_system_dta->event))
				{
						p_task_system_dta->flag = false;
						put_event_task_menu(p_task_system_dta->event);
						task_menu_update(NULL);

				}
				else if (false == p_task_system_dta->flag)
				{
						task_menu_update(NULL);
				}

				break;
	   }
	}
  }
}

/********************** end of file ******************************************/
