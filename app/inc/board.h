/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar>.
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
 * @file   : board.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef BOARD_INC_BOARD_H_
#define BOARD_INC_BOARD_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/
#define NUCLEO_F103RC		(0)
#define NUCLEO_F401RE		(1)
#define NUCLEO_F446RE		(2)
#define NUCLEO_F429ZI		(3)
#define NUCLEO_F439ZI		(4)
#define NUCLEO_F413ZH		(5)
#define STM32F429I_DISCO1	(6)

#define BOARD (NUCLEO_F103RC)

/* STM32 Nucleo Boards - 64 Pins */
#if ((BOARD == NUCLEO_F103RC) || (BOARD == NUCLEO_F401RE) || (BOARD == NUCLEO_F446RE))

#define BTN_A_PIN		B1_Pin
#define BTN_A_PORT		B1_GPIO_Port
#define BTN_A_PRESSED	GPIO_PIN_RESET
#define BTN_A_HOVER		GPIO_PIN_SET

#define BTN_GET_IN_GET_ON_PIN       D2_SEN_B1_Pin
#define BTN_GET_IN_GET_ON_PORT		D2_SEN_B1_GPIO_Port
#define BTN_GET_IN_GET_ON_PRESSED	GPIO_PIN_RESET
#define BTN_GET_IN_GET_ON_HOVER		GPIO_PIN_SET

#define BTN_ENT_PIN		D10_SEN_B2_Pin
#define BTN_ENT_PORT	D10_SEN_B2_GPIO_Port
#define BTN_ENT_PRESSED	GPIO_PIN_RESET
#define BTN_ENT_HOVER	GPIO_PIN_SET

#define BTN_NEX_PIN		D11_SEN_B3_Pin
#define BTN_NEX_PORT	D11_SEN_B3_GPIO_Port
#define BTN_NEX_PRESSED	GPIO_PIN_RESET
#define BTN_NEX_HOVER	GPIO_PIN_SET

#define BTN_ESC_PIN		D12_SEN_B4_Pin
#define BTN_ESC_PORT	D12_SEN_B4_GPIO_Port
#define BTN_ESC_PRESSED	GPIO_PIN_RESET
#define BTN_ESC_HOVER	GPIO_PIN_SET

#define BTN_H1_PIN		PC5_SEN_H1_Pin
#define BTN_H1_PORT		PC5_SEN_H1_GPIO_Port
#define BTN_H1_PRESSED	GPIO_PIN_RESET
#define BTN_H1_HOVER	GPIO_PIN_SET

#define BTN_H2_PIN		PC6_SEN_H2_Pin
#define BTN_H2_PORT		PC6_SEN_H2_GPIO_Port
#define BTN_H2_PRESSED	GPIO_PIN_RESET
#define BTN_H2_HOVER	GPIO_PIN_SET

#define BTN_H3_PIN		PC8_SEN_H3_Pin
#define BTN_H3_PORT		PC8_SEN_H3_GPIO_Port
#define BTN_H3_PRESSED	GPIO_PIN_RESET
#define BTN_H3_HOVER	GPIO_PIN_SET

#define LED_MOD_NORMAL_PIN		PC12_ACT2_Pin
#define LED_MOD_NORMAL_PORT		PC12_ACT2_GPIO_Port
#define LED_MOD_NORMAL_ON		GPIO_PIN_RESET
#define LED_MOD_NORMAL_OFF		GPIO_PIN_SET

#define LED_MOD_CONF_PIN		PC10_ACT1_Pin
#define LED_MOD_CONF_PORT		PC10_ACT1_GPIO_Port
#define LED_MOD_CONF_ON		    GPIO_PIN_RESET
#define LED_MOD_CONF_OFF		GPIO_PIN_SET

#define LED_A_PIN		LD2_Pin
#define LED_A_PORT		LD2_GPIO_Port
#define LED_A_ON		GPIO_PIN_SET
#define LED_A_OFF		GPIO_PIN_RESET

#endif/* STM32 Nucleo Boards - 144 Pins */

#if ((BOARD == NUCLEO_F429ZI) || (BOARD == NUCLEO_F439ZI) || (BOARD == NUCLEO_F413ZH))

#define BTN_A_PIN		USER_Btn_Pin
#define BTN_A_PORT		USER_Btn_GPIO_Port
#define BTN_A_PRESSED	GPIO_PIN_SET
#define BTN_A_HOVER		GPIO_PIN_RESET

#define LED_A_PIN		LD1_Pin
#define LED_A_PORT		LD1_GPIO_Port
#define LED_A_ON		GPIO_PIN_SET
#define LED_A_OFF		GPIO_PIN_RESET

#endif

/* STM32 Discovery Kits */
#if (BOARD == STM32F429I_DISCO1)

#define BTN_A_PIN		B1_Pin
#define BTN_A_PORT		B1_GPIO_Port
#define BTN_A_PRESSED	GPIO_PIN_SET
#define BTN_A_HOVER		GPIO_PIN_RESET

#define LED_A_PIN		LD3_Pin
#define LED_A_PORT		LD3_GPIO_Port
#define LED_A_ON		GPIO_PIN_SET
#define LED_A_OFF		GPIO_PIN_RESET

#endif

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* BOARD_INC_BOARD_H_ */

/********************** end of file ******************************************/
