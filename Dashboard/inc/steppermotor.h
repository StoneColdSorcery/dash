/*
 * chardisplay.h
 *
 *  Created on: Feb 12, 2015
 *      Author: Crazyfingers
 */

#ifndef STEPPERMOTOR_H_
#define STEPPERMOTOR_H_

#include "stm32f4xx.h"


typedef enum
{
	SM_1A = 0,
	SM_2A = 1,
	SM_3A = 2,
	SM_4A = 3,
	SM_EN12 = 4,
	SM_EN34 = 5

} SMPin_TypeDef;


#define SM_EN12_GPIO_PIN                    GPIO_Pin_0
#define SM_EN12_GPIO_PORT                   GPIOB
#define SM_EN12_GPIO_CLK                    RCC_AHB1Periph_GPIOB

#define SM_EN34_GPIO_PIN                    GPIO_Pin_1
#define SM_EN34_GPIO_PORT                   GPIOB
#define SM_EN34_GPIO_CLK                    RCC_AHB1Periph_GPIOB


#define SM_1A_GPIO_PIN                    GPIO_Pin_12
#define SM_1A_GPIO_PORT                   GPIOB
#define SM_1A_GPIO_CLK                    RCC_AHB1Periph_GPIOB

#define SM_2A_GPIO_PIN                    GPIO_Pin_13
#define SM_2A_GPIO_PORT                   GPIOB
#define SM_2A_GPIO_CLK                    RCC_AHB1Periph_GPIOB

#define SM_3A_GPIO_PIN                    GPIO_Pin_14
#define SM_3A_GPIO_PORT                   GPIOB
#define SM_3A_GPIO_CLK                    RCC_AHB1Periph_GPIOB

#define SM_4A_GPIO_PIN                    GPIO_Pin_15
#define SM_4A_GPIO_PORT                   GPIOB
#define SM_4A_GPIO_CLK                    RCC_AHB1Periph_GPIOB

#define SM_TIM		  				      TIM3
#define SM_TIM_CLKSRC					  RCC_APB1Periph_TIM3
#define SM_TIM_CNTCLK					  200
#define SM_TIM_OUTCLK					  10

#define A SM_1A_GPIO_PIN
#define Abar SM_2A_GPIO_PIN
#define B SM_3A_GPIO_PIN
#define Bbar SM_4A_GPIO_PIN

#define DBPINS A + Abar + B + Bbar

#define STATE1 A + B
#define STATE2 A
#define STATE3 Bbar
#define STATE4 Abar + Bbar
#define STATE5 Abar
#define STATE6 B

void SM_PinInit(SMPin_TypeDef);
void SM_TimInit(void);
void SM_PinHigh(SMPin_TypeDef);
void SM_PinLow(SMPin_TypeDef);
void SM_Disable(void);
void SM_Enable(void);
void SM_PinToggle(SMPin_TypeDef);
void SM_StepForward(void);
void SM_StepBackward(void);
void SM_TimerStop(void);
void SM_TimerForward(void);
void SM_TimerBackward(void);



#endif /* CHARDISPLAY_H_ */
