#ifndef APP_INC_EEPROM_H_
#define APP_INC_EEPROM_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/
extern I2C_HandleTypeDef hi2c1;

/********************** external functions declaration ***********************/
void EEPROM_Write(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void EEPROM_Read(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size);
void EEPROM_PageErase(uint16_t page);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* APP_INC_EEPROM_H_ */

/********************** end of file ******************************************/
