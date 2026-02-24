#ifndef APP_INC_EEPROM_INTERFACE_H_
#define APP_INC_EEPROM_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/
extern bool rom_corrupcion_flag;
/********************** external functions declaration ***********************/
void eeprom_load_sector_config(void);
void eeprom_save_sector_config(void);
uint8_t calcular_crc8(uint8_t *data, uint8_t length);
/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* APP_INC_EEPROM_INTERFACE_H_ */

/********************** end of file ******************************************/
