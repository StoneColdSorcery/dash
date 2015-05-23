/*
 * chardisplay.h
 *
 *  Created on: Feb 12, 2015
 *      Author: Crazyfingers
 */

#ifndef CHARDISPLAY_H_
#define CHARDISPLAY_H_

#include "stm32f4xx.h"

#define LCDPINNUM 8

typedef enum
{
	LCD_RS = 0,
	LCD_RW = 1,
	LCD_EN = 2,
	LCD_SOUT = 3,
	LCD_SRCLR = 4,
	LCD_OE = 5,
	LCD_RCK = 6,
	LCD_SRCK = 7
} LCDPin_TypeDef;



#define LCD_RS_GPIO_PIN                     GPIO_Pin_0
#define LCD_RS_GPIO_PORT                    GPIOD
#define LCD_RS_GPIO_CLK                     RCC_AHB1Periph_GPIOD

#define LCD_RW_GPIO_PIN                     GPIO_Pin_1
#define LCD_RW_GPIO_PORT                    GPIOD
#define LCD_RW_GPIO_CLK                     RCC_AHB1Periph_GPIOD

#define LCD_EN_GPIO_PIN                     GPIO_Pin_2
#define LCD_EN_GPIO_PORT                    GPIOD
#define LCD_EN_GPIO_CLK                     RCC_AHB1Periph_GPIOD


#define LCD_SOUT_GPIO_PIN                     GPIO_Pin_1
#define LCD_SOUT_GPIO_PORT                    GPIOC
#define LCD_SOUT_GPIO_CLK                     RCC_AHB1Periph_GPIOC

#define LCD_SRCLR_GPIO_PIN                    GPIO_Pin_2
#define LCD_SRCLR_GPIO_PORT                   GPIOC
#define LCD_SRCLR_GPIO_CLK                    RCC_AHB1Periph_GPIOC

#define LCD_OE_GPIO_PIN                       GPIO_Pin_4
#define LCD_OE_GPIO_PORT                      GPIOC
#define LCD_OE_GPIO_CLK                       RCC_AHB1Periph_GPIOC

#define LCD_RCK_GPIO_PIN                      GPIO_Pin_5
#define LCD_RCK_GPIO_PORT                     GPIOC
#define LCD_RCK_GPIO_CLK                      RCC_AHB1Periph_GPIOC

#define LCD_SRCK_GPIO_PIN                     GPIO_Pin_7
#define LCD_SRCK_GPIO_PORT                    GPIOC
#define LCD_SRCK_GPIO_CLK                     RCC_AHB1Periph_GPIOC

#define LCD_BUSY_PIN                     	  GPIO_Pin_8
#define LCD_BUSY_PORT                         GPIOA
#define LCD_BUSY_CLK                     	  RCC_AHB1Periph_GPIOA


//================================================
//LCD Instruction Set Bitmasks
//================================================

//DISPLAY OPTIONS
#define BLINK_ON 0x01
#define CURSOR_ON 0x02
#define DISPLAY_ON 0x04


//================================================
//LCD Address Defs
//================================================

#define ROW2START 0x40




void LCD_PinInit(LCDPin_TypeDef);
void LCD_BusyPinInit(void);
void LCD_PinHigh(LCDPin_TypeDef);
void LCD_PinLow(LCDPin_TypeDef);
void LCD_PinToggle(LCDPin_TypeDef);
void LCD_SetPin(LCDPin_TypeDef,uint16_t);
void LCD_SetCtrl(uint16_t, uint16_t);
void LCD_Pulse(LCDPin_TypeDef);
void LCD_PulseEn(void);
void LCD_GotoAddr(uint8_t);
void LCD_ReturnHome(void);
void LCD_ScreenClear(void);
void LCD_InsertChar(char);
void LCD_SetData(char);
void LCD_ScreenClear();
void LCD_BusyWait(void);
void LCD_Setup(void);

void LCD_BusyWait(void);
void LCD_DoFunc(char,char,char);
void LCD_OnSequence();
void LCD_TimingDelay_Decrement(void);
void LCD_Delay(__IO uint32_t);


#endif /* CHARDISPLAY_H_ */


