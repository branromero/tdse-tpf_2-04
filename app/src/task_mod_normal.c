/********************** inclusions *******************************************/
/* Project includes. */
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "task_actuator_attribute.h"
#include "task_normal_interface.h"
#include "task_menu_interface.h"
#include "display.h"
#include "task_temperature.h"
/********************** macros and definitions *******************************/
#define G_TASK_MEN_CNT_INI            0ul
#define G_TASK_MEN_TICK_CNT_INI       0ul

#define DEL_NOR_XX_MIN                0ul
#define DEL_NOR_XX_MED                30ul
#define DEL_NOR_XX_MAX                75ul
#define SECTOR_1					  1ul
#define SECTOR_2					  2ul
#define SECTOR_3					  3ul
/********************** internal data declaration ****************************/

/********************** internal data definition *****************************/
const char *p_task_normal 		= "Task Normal (Interactive Menu)";
const char *p_task_normal_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_normal_cnt_ADC;
uint32_t g_task_normal_cnt_refresh_menu;
uint8_t sector;
volatile uint32_t g_task_normal_tick_cnt;

void task_normal_init(void *parameters)
{
    /* Print out: Task Initialized */
    LOGGER_LOG("  %s is running\r\n", GET_NAME(task_normal_init));
    LOGGER_LOG("  %s is a normal task\r\n", GET_NAME(task_normal));

    g_task_normal_cnt_refresh_menu = G_TASK_MEN_CNT_INI;
    g_task_normal_cnt_ADC= G_TASK_MEN_CNT_INI;
    cycle_counter_init();
    cycle_counter_reset();

    g_task_normal_tick_cnt = G_TASK_MEN_TICK_CNT_INI;
    sector=SECTOR_1;
}

void task_normal_update(void *parameters)
{

	 if (any_event_task_normal())
	    {
	      task_system_ev_t event = get_event_task_normal();
	      if(event ==EV_MEN_H1_ACTIVE || event ==EV_MEN_H2_ACTIVE || event ==EV_MEN_H3_ACTIVE )
	         {
	           displayCharPositionWrite(0, 0);
	           displayStringWrite("                ");
	           displayCharPositionWrite(0, 0);
	           displayStringWrite("alarma ");

	           displayCharPositionWrite(0, 1);
	           displayStringWrite("                ");
	           displayCharPositionWrite(0, 1);
	           displayStringWrite("activada");
	          }
	     }

	    if (g_task_normal_cnt_ADC > DEL_NOR_XX_MIN) {
	    	g_task_normal_cnt_ADC--;
	    } else {
	    	float tem_sec1=ADC_SEC1_Temperature() ;
			float tem_sec2=ADC_SEC2_Temperature() ;
			float tem_sec3=ADC_SEC3_Temperature() ;

			if (tem_sec1 >= sector_cfg[0].temp_max ||
				     tem_sec2 >= sector_cfg[1].temp_max ||
				     tem_sec3 >= sector_cfg[2].temp_max)
				 {
				     displayCharPositionWrite(0, 0);
				     displayStringWrite("                ");
				     displayCharPositionWrite(0, 0);
				     displayStringWrite("alarma ");

				     displayCharPositionWrite(0, 1);
				     displayStringWrite("                ");
				     displayCharPositionWrite(0, 1);
				     displayStringWrite("activada");

				 }else{
					 float tem;
					     if (g_task_normal_cnt_refresh_menu > DEL_NOR_XX_MIN) {
					     	g_task_normal_cnt_refresh_menu--;
					     } else {
					         const char *sector_label = NULL;

					         switch (sector) {
					             case SECTOR_1:
					                 sector_label = "sector1";
					                 //tem=sector_cfg[sector-1].temp_max;
					                 tem=tem_sec1;
					                 sector = SECTOR_2;
					                 break;

					             case SECTOR_2:
					             	//tem=sector_cfg[sector-1].temp_max;
					             	tem=tem_sec2;
					                 sector = SECTOR_3;
					                 sector_label = "sector2";
					                 break;

					             case SECTOR_3:
					             default:
					             	//tem=sector_cfg[sector-1].temp_max;
					             	tem=tem_sec3;
					                 sector = SECTOR_1;
					                 sector_label = "sector3";
					                 break;
					         }

					         g_task_normal_cnt_refresh_menu = DEL_NOR_XX_MAX;

					         displayCharPositionWrite(0, 0);
					         displayStringWrite("                ");
					         displayCharPositionWrite(0, 0);
					         displayStringWrite(sector_label);


					         char line1[17] = {0};
					         snprintf(line1, sizeof(line1), "temp=%.1f", tem);
					         displayCharPositionWrite(0, 1);
					         displayStringWrite("                ");
					         displayCharPositionWrite(0, 1);
					         displayStringWrite(line1);


					     }
				 }
			g_task_normal_cnt_ADC=DEL_NOR_XX_MED;
	    }
}

