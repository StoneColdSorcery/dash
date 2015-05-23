/*
 * chardisplay.c
 *
 *  Created on: Feb 12, 2015
 *      Author: Crazyfingers
 */

#include "chardisplay.h"

GPIO_TypeDef* LCD_GPIO_PORT[LCDPINNUM] = {LCD_RS_GPIO_PORT, LCD_RW_GPIO_PORT, LCD_EN_GPIO_PORT, LCD_SOUT_GPIO_PORT,
		LCD_SRCLR_GPIO_PORT, LCD_OE_GPIO_PORT, LCD_RCK_GPIO_PORT, LCD_SRCK_GPIO_PORT};

const uint16_t LCD_GPIO_PIN[LCDPINNUM] = {LCD_RS_GPIO_PIN, LCD_RW_GPIO_PIN, LCD_EN_GPIO_PIN, LCD_SOUT_GPIO_PIN,
		LCD_SRCLR_GPIO_PIN,	LCD_OE_GPIO_PIN, LCD_RCK_GPIO_PIN, LCD_SRCK_GPIO_PIN};

const uint32_t LCD_GPIO_CLK[LCDPINNUM] = {LCD_RS_GPIO_CLK, LCD_RW_GPIO_CLK, LCD_EN_GPIO_CLK, LCD_SOUT_GPIO_CLK,
		LCD_SRCLR_GPIO_CLK, LCD_OE_GPIO_CLK, LCD_RCK_GPIO_CLK, LCD_SRCK_GPIO_CLK};


__IO uint32_t LCD_TimingDelay;

void LCD_PinInit(LCDPin_TypeDef cPin)
{


  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(LCD_GPIO_CLK[cPin], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = LCD_GPIO_PIN[cPin];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_GPIO_PORT[cPin], &GPIO_InitStructure);

}

void LCD_BusyPinInit()
{


  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(LCD_BUSY_CLK, ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = LCD_BUSY_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LCD_BUSY_PORT, &GPIO_InitStructure);

}

void LCD_Setup(void)
{
	LCD_PinInit(LCD_RS);
	LCD_PinInit(LCD_RW);
	LCD_PinInit(LCD_EN);

	LCD_PinInit(LCD_SOUT);
	LCD_PinInit(LCD_SRCLR);
	LCD_PinInit(LCD_OE);
	LCD_PinInit(LCD_RCK);
	LCD_PinInit(LCD_SRCK);

	LCD_BusyPinInit();

	LCD_PinLow(LCD_RS);
	LCD_PinLow(LCD_RW);
	LCD_PinLow(LCD_EN);

	LCD_PinLow(LCD_SOUT);
	LCD_PinHigh(LCD_SRCLR);
	LCD_PinLow(LCD_OE);
	LCD_PinLow(LCD_RCK);
	LCD_PinLow(LCD_SRCK);

	LCD_PinHigh(LCD_SOUT);
	LCD_PinLow(LCD_SOUT);

	/*
	LCD_PinLow(LCD_OE);
	LCD_PinHigh(LCD_SOUT);
	LCD_PinHigh(LCD_SRCK);
*/
	LCD_OnSequence();
	//LCD_PinLow(LCD_SRCLR);

	LCD_GotoAddr(ROW2START);
}

void LCD_SetData(char w)
{

	int i;
	for(i= 0; i < 8; i++)
	{
		LCD_SetPin(LCD_SOUT, ( (w >> i) & 0x01));
		//set sout to ith bit of w
		LCD_Pulse(LCD_SRCK);
		//LCD_Pulse(LCD_RCK);
	}
	LCD_Pulse(LCD_RCK);
}

void LCD_SetCtrl(uint16_t _rs, uint16_t _rw)
{
	LCD_SetPin(LCD_RS,_rs);
	LCD_SetPin(LCD_RW,_rw);
}

void LCD_DoFunc(char _rs, char _rw, char _data)
{
	LCD_BusyWait();
	LCD_SetCtrl( _rs, _rw);
	LCD_SetData(_data);
	LCD_PinHigh(LCD_EN);
	LCD_PinLow(LCD_EN);
	//LCD_Delay(10);
	//LCD_PulseEn();
}

void LCD_BusyWait()
{
	uint32_t LCD_busytimeout = 10000;
	LCD_PinHigh(LCD_OE);
	LCD_SetCtrl( 0x00, 0x01);
	LCD_SetData(0x00);
	LCD_PinHigh(LCD_EN);
	LCD_PinLow(LCD_EN);
	while((GPIO_ReadInputDataBit(LCD_BUSY_PORT, LCD_BUSY_PIN) > 0) && (LCD_busytimeout > 0))
	{
		LCD_busytimeout--;
	};
	LCD_PinLow(LCD_OE);
}
void LCD_OnSequence()
{

	LCD_DoFunc(0,0,0x3f);
	//LCD_Delay(10);
	LCD_DoFunc(0,0,0x3f);
	//LCD_Delay(10);
	LCD_DoFunc(0,0,0x3f);
	//LCD_Delay(10);
	LCD_DoFunc(0,0,0x3f);
	//LCD_Delay(10);
	LCD_DoFunc(0,0,0x08);
	//LCD_Delay(10);
	LCD_DoFunc(0,0,0x01);
	//LCD_Delay(10);
	LCD_DoFunc(0,0,0x06);
	LCD_DoFunc(0,0,0x0f);



}

void LCD_InsertChar(char c)
{
	//LCD_BusyWait();
	//LCD_DoFunc(0,0,0x17);
	LCD_DoFunc(1,0,c);
	//LCD_DoFunc(0,0,0x1);


}


void LCD_ScreenClear()
{
	LCD_DoFunc(0,0,0x1);

}

void LCD_ReturnHome()
{
	LCD_DoFunc(0,0,0x03);

}

void LCD_GotoAddr(uint8_t _addr)
{
	uint8_t addrCmd =  0x80 + _addr;
	LCD_DoFunc(0,0,addrCmd);

}

void LCD_PulseEn(void)
{
	LCD_PinHigh(LCD_EN);
	LCD_PinLow(LCD_EN);
}

void LCD_Pulse(LCDPin_TypeDef cPin)
{
	LCD_PinHigh(cPin);
	LCD_PinLow(cPin);
}

void LCD_SetPin(LCDPin_TypeDef cPin,uint16_t val)
{
	if (val == 1)
		LCD_GPIO_PORT[cPin]->BSRRL = LCD_GPIO_PIN[cPin];
	else
		LCD_GPIO_PORT[cPin]->BSRRH = LCD_GPIO_PIN[cPin];
}

void LCD_PinHigh(LCDPin_TypeDef cPin)
{
	LCD_GPIO_PORT[cPin]->BSRRL = LCD_GPIO_PIN[cPin];
}

void LCD_PinLow(LCDPin_TypeDef cPin)
{
	LCD_GPIO_PORT[cPin]->BSRRH = LCD_GPIO_PIN[cPin];
}

void LCD_PinToggle(LCDPin_TypeDef cPin)
{
	LCD_GPIO_PORT[cPin]->ODR ^= LCD_GPIO_PIN[cPin];
}

void LCD_Delay(__IO uint32_t nTime)
{
  LCD_TimingDelay = nTime;

    while(LCD_TimingDelay != 0);
}

void LCD_TimingDelay_Decrement(void)
{
  if (LCD_TimingDelay != 0x00)
  {
    LCD_TimingDelay--;
  }
}
