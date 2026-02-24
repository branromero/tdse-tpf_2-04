/* Project includes. */
#include "main.h"
#include <math.h>

/* Demo includes. */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes. */
#include "eeprom.h"
#include <stdbool.h>

#define EEPROM_I2C        &hi2c1
#define EEPROM_ADDR       0xA0
#define PAGE_SIZE         64
#define PAGE_NUM          512

uint16_t bytestowrite(uint16_t size, uint16_t offset){
    if ((size + offset) < PAGE_SIZE) return size;
    else return PAGE_SIZE - offset;
}

void EEPROM_Write(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
{
    uint16_t pos = 0;
    uint16_t bytes_remaining = size;
    uint16_t startPage = page;

    while (bytes_remaining > 0)
    {
        uint16_t bytes_to_write = bytestowrite(bytes_remaining, offset);
        uint16_t memAddress = (startPage * PAGE_SIZE) + offset;

        // Modo bloqueante, con dirección de 16 bits
        HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, memAddress, I2C_MEMADD_SIZE_16BIT,
                          &data[pos], bytes_to_write, HAL_MAX_DELAY);

        HAL_Delay(5);  // Write cycle delay

        pos += bytes_to_write;
        bytes_remaining -= bytes_to_write;
        startPage++;
        offset = 0;
    }
}

void EEPROM_Read(uint16_t page, uint16_t offset, uint8_t *data, uint16_t size)
{
    uint16_t pos = 0;
    uint16_t bytes_remaining = size;
    uint16_t startPage = page;

    while (bytes_remaining > 0)
    {
        uint16_t bytes_to_read = bytestowrite(bytes_remaining, offset);
        uint16_t memAddress = (startPage * PAGE_SIZE) + offset;

        HAL_I2C_Mem_Read(EEPROM_I2C, EEPROM_ADDR, memAddress, I2C_MEMADD_SIZE_16BIT,
                         &data[pos], bytes_to_read, HAL_MAX_DELAY);

        pos += bytes_to_read;
        bytes_remaining -= bytes_to_read;
        startPage++;
        offset = 0;
    }
}

void EEPROM_PageErase(uint16_t page)
{
    uint8_t data[PAGE_SIZE];
    memset(data, 0xFF, PAGE_SIZE);

    uint16_t memAddress = page * PAGE_SIZE;

    HAL_I2C_Mem_Write(EEPROM_I2C, EEPROM_ADDR, memAddress, I2C_MEMADD_SIZE_16BIT,
                      data, PAGE_SIZE, HAL_MAX_DELAY);

    HAL_Delay(5);
}
