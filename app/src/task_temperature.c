#include "main.h"
#include "logger.h"
// ********************** macros and definitions *******************************
#define V25 1.43F
#define AVG_SLOPE 0.0043F

// ********************** external data declaration ****************************
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

// ********************** internal functions declaration ***********************
HAL_StatusTypeDef ADC_Read_Sequence(ADC_HandleTypeDef *hadc, uint16_t *buffer, uint32_t num_channels);
HAL_StatusTypeDef ADC_Read_Single(ADC_HandleTypeDef *hadc, uint16_t *value, uint32_t channel);

// ********************** external functions definition ************************

float ADC_SEC1_Temperature() {
    uint16_t values[2];

    if (HAL_OK == ADC_Read_Sequence(&hadc1, values, 2)) {
        float vsense = (values[1] * 3.3f) / 4095.0f; // Rank 2 -> Channel 0
        float temperature = ((vsense - V25) / AVG_SLOPE) + 25.0f;
        return temperature;
    }

    LOGGER_LOG("ERROR: Lectura de temperatura interna fallida\n");
    return -1;
}

float ADC_SEC2_Temperature() {
    uint16_t value;

    if (HAL_OK == ADC_Read_Single(&hadc2, &value, ADC_CHANNEL_1)) {
        float vsense = (value * 3.3f) / 4095.0f;
        float temperature = ((vsense - V25) / AVG_SLOPE) + 25.0f;
        return temperature;
    }

    LOGGER_LOG("ERROR: Lectura de temperatura interna fallida\n");
    return -1;
}

float ADC_SEC3_Temperature() {
    uint16_t values[2];

    if (HAL_OK == ADC_Read_Sequence(&hadc1, values, 2)) {
        float vsense = (values[0] * 3.3f) / 4095.0f; // Rank 1 -> Channel 4
        float temperature = ((vsense - V25) / AVG_SLOPE) + 25.0f;
        return temperature;
    }

    LOGGER_LOG("ERROR: Lectura de temperatura interna fallida\n");
    return -1;
}

// ********************** internal functions definition ***********************

HAL_StatusTypeDef ADC_Read_Sequence(ADC_HandleTypeDef *hadc, uint16_t *buffer, uint32_t num_channels) {
    HAL_StatusTypeDef res;

    res = HAL_ADC_Start(hadc);
    if (res != HAL_OK) {
        LOGGER_LOG("ERROR: Fallo al iniciar ADC\n");
        return res;
    }

    for (uint32_t i = 0; i < num_channels; i++) {
        res = HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
        if (res == HAL_OK) {
            buffer[i] = HAL_ADC_GetValue(hadc);
        } else {
            LOGGER_LOG("ERROR: Fallo en conversión ADC (Rank %lu)\n", i+1);
            HAL_ADC_Stop(hadc);
            return res;
        }
    }

    HAL_ADC_Stop(hadc);
    return HAL_OK;
}


HAL_StatusTypeDef ADC_Read_Single(ADC_HandleTypeDef *hadc, uint16_t *value, uint32_t channel) {
    HAL_StatusTypeDef res;
    ADC_ChannelConfTypeDef sConfig = {0};

    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;


    res = HAL_ADC_ConfigChannel(hadc, &sConfig);
    if (res != HAL_OK) {
        LOGGER_LOG("ERROR: Configuración de canal ADC fallida\n");
        return res;
    }

    res = HAL_ADC_Start(hadc);
    if (res == HAL_OK) {
        res = HAL_ADC_PollForConversion(hadc, HAL_MAX_DELAY);
        if (res == HAL_OK) {
            *value = HAL_ADC_GetValue(hadc);
        } else {
            LOGGER_LOG("ERROR: Fallo en conversión ADC\n");
        }
    } else {
        LOGGER_LOG("ERROR: Fallo al iniciar ADC\n");
    }

    HAL_ADC_Stop(hadc);
    return res;
}
