/**
  ******************************************************************************
  * @file    sevenseg.h
  * @author  CrazyFingers
  * @version V0.1
  * @date    12-15-2014
  * @brief   Header for sevenseg.c module
  ******************************************************************************

  ******************************************************************************
  */



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SEVENSEG_H
#define __SEVENSEG_H

#include "stm32f4xx.h"

typedef enum
{
	SOUT = 0,
	SRCLR = 1,
	EN = 2,
	RCK = 3,
	SRCK = 4
} ShiftRegPin_TypeDef;

typedef enum
{
	D1 = 0,
	D2 = 1,
	D3 = 2,
	D4 = 3
} DigPin_TypeDef;

#define CTRLn   5
#define DIGITn  4

//Seven segment driver gpio initializations

#define SOUT_GPIO_PIN                     GPIO_Pin_11
#define SOUT_GPIO_PORT                    GPIOE
#define SOUT_GPIO_CLK                     RCC_AHB1Periph_GPIOE

#define SRCLR_GPIO_PIN                    GPIO_Pin_12
#define SRCLR_GPIO_PORT                   GPIOE
#define SRCLR_GPIO_CLK                    RCC_AHB1Periph_GPIOE

#define EN_GPIO_PIN                       GPIO_Pin_13
#define EN_GPIO_PORT                      GPIOE
#define EN_GPIO_CLK                       RCC_AHB1Periph_GPIOE

#define RCK_GPIO_PIN                      GPIO_Pin_14
#define RCK_GPIO_PORT                     GPIOE
#define RCK_GPIO_CLK                      RCC_AHB1Periph_GPIOE


//SRCLK timer & pin defs

#define SRCK_GPIO_PIN                     GPIO_Pin_12
#define SRCK_GPIO_PORT                    GPIOD
#define SRCK_GPIO_CLK                     RCC_AHB1Periph_GPIOD

#define SRCK_TIM						  TIM4
#define SRCK_TIM_PINSRC                   GPIO_PinSource12
#define SRCK_TIM_CLK                      RCC_APB1Periph_TIM4



//SR FREQ DEFINITIONS

#define SR_TIM_CNTCLK    1000000
#define SR_TIM_OUTCLK    10000
#define SR_TIM_DUTY      50



//7 SEGMENT PIN DEFS

#define NDIGITS 4

#define DIG1_GPIO_PIN                     GPIO_Pin_7
#define DIG1_GPIO_PORT                    GPIOE
#define DIG1_GPIO_CLK                     RCC_AHB1Periph_GPIOE

#define DIG2_GPIO_PIN                     GPIO_Pin_8
#define DIG2_GPIO_PORT                    GPIOE
#define DIG2_GPIO_CLK                     RCC_AHB1Periph_GPIOE

#define DIG3_GPIO_PIN                     GPIO_Pin_9
#define DIG3_GPIO_PORT                    GPIOE
#define DIG3_GPIO_CLK                     RCC_AHB1Periph_GPIOE

#define DIG4_GPIO_PIN                     GPIO_Pin_10
#define DIG4_GPIO_PORT                    GPIOE
#define DIG4_GPIO_CLK                     RCC_AHB1Periph_GPIOE

//bitmasks for serializing tx byte
#define  MASK0  0x01
#define  MASK1  0x02
#define  MASK2  0x04
#define  MASK3  0x08
#define  MASK4  0x10
#define  MASK5  0x20
#define  MASK6  0x40
#define  MASK7  0x80

//Segment bitmasks

#define  SEGA  0x01
#define  SEGB  0x02
#define  SEGC  0x04
#define  SEGD  0x08
#define  SEGE  0x10
#define  SEGF  0x20
#define  SEGG  0x40
#define  SEGP  0x80

extern const uint16_t BCD[10];

// Pin enums



/*
//Digit selector frame type
typedef struct
{
	volatile uint8_t DigSelVector[NDIGITS];
	uint32_t
};
*/



extern __IO uint16_t srBusy;
extern volatile uint8_t DigSelVector[NDIGITS];

void Sevenseg_PinInit(ShiftRegPin_TypeDef);
void Sevenseg_Setup(void);
void Sevenseg_PinHigh(ShiftRegPin_TypeDef);
void Sevenseg_PinLow(ShiftRegPin_TypeDef);
void Sevenseg_PinToggle(ShiftRegPin_TypeDef);
void Dig_PinInit(DigPin_TypeDef);
void Dig_PinLow(DigPin_TypeDef);
void Dig_PinHigh(DigPin_TypeDef);
void Dig_PinToggle(DigPin_TypeDef);
void Sevenseg_On(void);
void Sevenseg_Off(void);
void Sevenseg_Update(uint32_t);
void SelectDigit(DigPin_TypeDef);
void GenerateDigits(volatile uint8_t*,uint32_t);
void Sevenseg_Send(DigPin_TypeDef,uint16_t);
void Sevenseg_TimInit(void);



#endif //sevenseg
