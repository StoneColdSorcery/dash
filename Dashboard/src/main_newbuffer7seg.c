/**
  ******************************************************************************
  * @file    TIM_PWM_Output/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    11-September-2013
  * @brief   This example shows how to configure the TIM peripheral in
  *          PWM (Pulse Width Modulation) mode
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f401_discovery.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "sevenseg.h"
#include "uartbuffer.h"
#include "chardisplay.h"
#include "steppermotor.h"

/** @addtogroup STM32F401_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup TIM_PWM_Output
  * @{
  */ 



/* Private typedef -----------------------------------------------------------*/
typedef volatile struct {
	uint8_t buffer[40];
	uint32_t start;
	uint32_t end;
	uint32_t length;
} RingBuffer;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay;
__IO uint8_t RXbyte;
__IO uint16_t rxval;
__IO uint8_t TXbuffer[20];

RingBuffer TxRingBuf;

RxBuffer RxRingBuf;


/* Private function prototypes -----------------------------------------------*/
void BufferSend( RingBuffer*);
int BufferRead( RingBuffer*);
void AddtoBuffer( RingBuffer*,uint8_t);
void BufferInit(RingBuffer*);

void USART2_SendByte(uint8_t txbyte);

static void TIM_Config(void);
void TimingDelay_Decrement(void);
void Delay(__IO uint32_t nTime);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  
  BufferInit(&TxRingBuf);
  Rx_BufInit(&RxRingBuf);


  /*!< At this stage the microcontroller clock setting is already configured, 
  this is done through SystemInit() function which is called from startup
  file (startup_stm32f401xx.s) before to branch to application main.
  To reconfigure the default setting of SystemInit() function, refer to
  system_stm32f4xx.c file
  */
  
  /* TIM Configuration */
  //TIM_Config();
  
  /* -----------------------------------------------------------------------
  TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles.
  
  In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
  since APB1 prescaler is different from 1.   
  TIM3CLK = 2 * PCLK1  
  PCLK1 = HCLK / 4 
  => TIM3CLK = HCLK / 2 = SystemCoreClock /2
  
  To get TIM3 counter clock at 14 MHz, the prescaler is computed as follows:
  Prescaler = (TIM3CLK / TIM3 counter clock) - 1
  Prescaler = ((SystemCoreClock /2) /14 MHz) - 1
  
  To get TIM3 output clock at 21 KHz, the period (ARR)) is computed as follows:
  ARR = (TIM3 counter clock / TIM3 output clock) - 1
  = 665
  
  TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
  TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
  TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
  TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
  
  Note: 
  SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
  Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
  function to update SystemCoreClock variable value. Otherwise, any configuration
  based on this variable will be incorrect.    
  ----------------------------------------------------------------------- */  
  //===========================================================
  //UART config
  //===========================================================

  USART_InitTypeDef UartHandle;
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  USART_ClockInitTypeDef USART_ClockInitStruct;
  RCC_ClocksTypeDef RCC_Clocks;
  
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 100);


  USART_StructInit(&UartHandle);

  UartHandle.USART_BaudRate = 57600;
  USART_ClockStructInit(&USART_ClockInitStruct);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);



  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  USART_Init(USART2,&UartHandle);

  USART_ClockInit(USART2,&USART_ClockInitStruct);
  USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
  USART_ITConfig(USART2, USART_IT_TC,ENABLE);


  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_Cmd(USART2, ENABLE);
  Delay(40);




  Sevenseg_Setup();
  SM_Setup();
  Delay(40);

  LCD_Setup();
  //LCD_SetData(0xff);


  USART_SendData(USART2,0xaaaa);
  //GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1);
  GPIO_SetBits(GPIOD,GPIO_Pin_0);
  //======================================================
  //----------------------------------------------------
  //========================================================






//  /* Compute the prescaler value */
//  PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;
//
//  /* Time base configuration */
//  TIM_TimeBaseStructure.TIM_Period = 5000;
//  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
//  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//
//  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//
//  /* PWM1 Mode configuration: Channel1 */
//  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//
//  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
//
//  TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
//
//
//  /* PWM1 Mode configuration: Channel2 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
//
//  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//
//  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
//
//  /* PWM1 Mode configuration: Channel3 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
//
//  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
//
//  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
//
//  /* PWM1 Mode configuration: Channel4 */
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
//
//  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
//
//  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
//
//  TIM_ARRPreloadConfig(TIM3, ENABLE);
//
//  /* TIM3 enable counter */
//  TIM_Cmd(TIM3, ENABLE);
  int bts = 0;
  uint16_t ds = 55;


  //Sevenseg_Send(0x33);

  while (1)
  {

	if(RxRingBuf.length > 2)
	{
		uint32_t b1;
		uint32_t b2;
		uint32_t b3;
		uint32_t dword;


		b1 = Rx_ReadFromBuf(&RxRingBuf);
		b2 = Rx_ReadFromBuf(&RxRingBuf);
		b3 = Rx_ReadFromBuf(&RxRingBuf);
		//for(i = 0; i < ntosend;i++)
		//{

			if((b1 & 0xf0) == 0xf0 ) // if first bit  is 7SEG header
			{

				ds = (b1 & 0x0f); // 4 LSBs of header select digit

				dword = (b2 << 8) | b3; //combine b2,b3 into 16 bit num


				if(ds == 0x01)
				{
					Sevenseg_On();
					//if(b2 < 10) Sevenseg_Send(ds,b2);
				}
				else if (ds == 0x02)
				{
					Sevenseg_Off();
				}
				else if(ds == 0x03)
				Sevenseg_Update(dword);

			}

			else if((b1 & 0xf0) == 0xa0 ) // if first bit  is LCD header
			{

				ds = (b1 & 0x0f); // 4 LSBs of header select digit
				dword = (b2 << 8) | b3; //combine b2,b3 into 16 bit num


				if(ds == 0x01)
				{
					LCD_SetData(b2);

				}
				else if (ds == 0x02)
				{
					LCD_SetCtrl(b2,b3);
				}
				else if (ds == 0x03)
				{
					//LCD_SetPin(LCD_OE,b2);
					//LCD_SetPin(LCD_SRCLR,b3);
					char rw = b2 & 0x01;
					char rs = ( (b2 & 0x02) >> 1);
					LCD_DoFunc(rs,rw,b3);

				}
				else if (ds == 0x04)
				{
					//LCD_SetPin(LCD_OE,b2);
					//LCD_SetPin(LCD_SRCLR,b3);
					LCD_InsertChar(b3);

				}
			}
			else if((b1 & 0xf0) == 0x70 ) // if first bit  is Stepper Motor header
			{

				ds = (b1 & 0x0f); // 4 LSBs of header select digit
				dword = (b2 << 8) | b3; //combine b2,b3 into 16 bit num


				if(ds == 0x01)
				{
					SM_Enable();

				}
				else if (ds == 0x02)
				{
					SM_Disable();
				}
				else if (ds == 0x03)
				{

					SM_StepForward();
				}
				else if (ds == 0x04)
				{
					SM_StepBackward();

				}
				else if (ds == 0x05)
				{
					SM_TimerBackward();

				}
				else if (ds == 0x06)
				{
					SM_TimerStop();

				}

			}




	}
	  /*
	  GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1);
	  GPIO_ResetBits(GPIOA,GPIO_Pin_0);


	  //GPIO_ToggleBits(GPIOB, GPIO_Pin_0);
	  GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1);
	  GPIO_SetBits(GPIOA,GPIO_Pin_0);
	  */
	  //USART_SendData(USART2,0x0051);
	  //USART_SendData(USART2,0x003b);
  }
}

/**
  * @brief  Configure the TIM3 Ouput Channels.
  * @param  None
  * @retval None
  */


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while (1)
  {
	  Delay(10);
	  GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
	 // USART_SendData(USART2,0x00aa);
	// if((TxRingBuf.length > 0) ) {
		//BufferSend(&TxRingBuf);
	 //};
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 
/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in 10 ms.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;

    while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}



/////////////////
//IRQ HANDLERS
/////////////////

void USART2_IRQHandler(void)
{
	// check for rx buffer flag
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		rxval = USART_ReceiveData(USART2);
		Rx_AddToBuf(&RxRingBuf,rxval);
		//USART2_SendByte(rxval);
		//USART2_SendByte((rxval + 0x01));
		//USART_SendData(USART2,'a');
		//USART_SendData(USART2,'b');
		//USART_SendData(USART2,'c');
	}

	// check for transmit complete interrupts
	if(USART_GetITStatus(USART2, USART_IT_TC) == SET)
	{
		if(TxRingBuf.length > 0)
		{
			BufferSend(&TxRingBuf);
		}

		USART_ClearITPendingBit(USART2,  USART_IT_TC);
	}


}



void USART2_SendByte(uint8_t txbyte)
{
	//AddtoBuffer(&TxRingBuf,txbyte);

	//Check if transmit reg is empty

	AddtoBuffer(&TxRingBuf,txbyte);

	if(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == SET)
	{
		//tx reg is empty so send this byte
		BufferSend(&TxRingBuf);
	}
	else //tx reg is full, so place txbyte in buffer
	{


	};

}

////
//BUFFER STUFF
////

void AddtoBuffer(RingBuffer* Ringbuf, uint8_t inbyte)
{

	if(Ringbuf->length == 0)
	{
		Ringbuf->end = 0;
		Ringbuf->start = 0;
		Ringbuf->buffer[Ringbuf->end] = inbyte;
		Ringbuf->length = 1;
	}
	else
	{
		int NewEnd = Ringbuf->end + 1;
		if(NewEnd > 39)
		{
			NewEnd = 0;
		}

		if(~((NewEnd == Ringbuf->start) && (Ringbuf->length > 39)))
		{
			 //if buffer is not full

			 Ringbuf->end = NewEnd;
			 Ringbuf->buffer[Ringbuf->end] = inbyte;
			 Ringbuf->length += 1;

		}

	}


}

void BufferSend(RingBuffer* Ringbuf)
{
	if(Ringbuf->length > 0)
	{
		USART_SendData(USART2,Ringbuf->buffer[Ringbuf->start]);
		Ringbuf->length -= 1;

		if(Ringbuf ->length > 0){
			int NewStart = Ringbuf->start + 1;
			if(NewStart > 39)
			{
				NewStart = 0;
			}

			Ringbuf->start = NewStart;
		}
	}
}

int BufferRead(RingBuffer* Ringbuf)
{
	int32_t ret = -1;
	if(Ringbuf->length > 0)
	{
		ret = Ringbuf->buffer[Ringbuf->start];
		Ringbuf->length -= 1;

		if(Ringbuf ->length > 0)
		{
			int NewStart = Ringbuf->start + 1;
			if(NewStart > 39)
			{
				NewStart = 0;
			}

			Ringbuf->start = NewStart;
		}
	}
	return ret;
}

void BufferInit(RingBuffer* Ringbuf)
{
	Ringbuf->start = 0;
	Ringbuf->end = 0;
	Ringbuf->length = 0;
	int i = 0;
	for(i = 0; i < sizeof(Ringbuf->buffer);i++)
	{
		Ringbuf->buffer[i] = 0xff;
	}

}

//////
//LED STUFF
/////




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
