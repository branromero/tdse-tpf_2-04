/* Project includes. */
#include "main.h"

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "task_menu_interface.h"
#include "eeprom.h"
#include "display.h"

/*
PAGINA 1 CONTIENE:

Byte 0 → sector_cfg[0].humo_en
Byte 1 → sector_cfg[0].temp_en
Byte 2 → sector_cfg[0].temp_max

Byte 3 → sector_cfg[1].humo_en
Byte 4 → sector_cfg[1].temp_en
Byte 5 → sector_cfg[1].temp_max

Byte 6 → sector_cfg[2].humo_en
Byte 7 → sector_cfg[2].temp_en
Byte 8 → sector_cfg[2].temp_max

Byte 9 → (reservado de seguridad)
*/
sector_config_t sector_cfg[SECTOR_COUNT];
uint8_t security_byte;
bool rom_corrupcion_flag = false;
uint8_t buffer[10];

//uint8_t entre=0;
//  CRC8
uint8_t calcular_crc8(uint8_t *data, uint8_t length) {
    uint8_t crc = 0x00;
    for (uint8_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x07;
            else
                crc <<= 1;
        }
    }
    return crc;
}

void eeprom_load_sector_config(void)
{
    uint8_t buffer[10];
    EEPROM_Read(1, 0, buffer, 10);  // Lee los 10 bytes en la página 1

    uint8_t crc_calculado = calcular_crc8(buffer, 9);

    if (crc_calculado == buffer[9]) {
        //entre++;
        for (int i = 0; i < SECTOR_COUNT; i++) {
            sector_cfg[i].humo_en  = buffer[i * 3 + 0];
            sector_cfg[i].temp_en  = buffer[i * 3 + 1];
            sector_cfg[i].temp_max = buffer[i * 3 + 2];
        }
        security_byte = buffer[9];  // Guardar CRC si es válido
        rom_corrupcion_flag = false;
    } else {
        rom_corrupcion_flag = true;  // Indica corrupción
        displayCharPositionWrite(0, 0);
        displayStringWrite("MEMORIA CORRUPTA");
        displayCharPositionWrite(0, 1);
        displayStringWrite("ESC = RESET_ROM ");
    }
}


void eeprom_save_sector_config(void)
{
    for (int i = 0; i < SECTOR_COUNT; i++) {
        buffer[i * 3 + 0] = sector_cfg[i].humo_en;
        buffer[i * 3 + 1] = sector_cfg[i].temp_en;
        buffer[i * 3 + 2] = sector_cfg[i].temp_max;
    }
    buffer[9] = calcular_crc8(buffer, 9);
    security_byte = buffer[9];
    EEPROM_Write(1, 0, buffer, 10); // Escribir los 10 bytes en la página 1

}
