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

/** @addtogroup STM32F401_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup TIM_PWM_Output
  * @{
  */ 



/* Private typedef -----------------------------------------------------------*/
struct {
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

__IO RingBuffer TxRingBuf;



/* Private function prototypes -----------------------------------------------*/
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

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  USART_Cmd(USART2, ENABLE);
  //Delay(1);
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

  while (1)
  {
	  GPIO_ResetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1);
	  GPIO_ResetBits(GPIOA,GPIO_Pin_0);

	  Delay(10);
	  //GPIO_ToggleBits(GPIOB, GPIO_Pin_0);
	  GPIO_SetBits(GPIOB,GPIO_Pin_0 | GPIO_Pin_1);
	  GPIO_SetBits(GPIOA,GPIO_Pin_0);
	  //USART_SendData(USART2,0x0051);
	  //USART_SendData(USART2,0x003b);
  }
}

/**
  * @brief  Configure the TIM3 Ouput Channels.
  * @param  None
  * @retval None
  */
static void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOC and GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* GPIOC Configuration: TIM3 CH1 (PC6) and TIM3 CH2 (PC7) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
  
  /* GPIOB Configuration:  TIM3 CH3 (PB0) and TIM3 CH4 (PB1) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

  /* Connect TIM3 pins to AF2 */  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3); 
  //GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
  //GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);
}

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
		USART_SendData(USART2,rxval);
		//USART_SendData(USART2,'a');
		//USART_SendData(USART2,'b');
		//USART_SendData(USART2,'c');
	}

}

void USART2_Transmit(uint8_t txbyte)
{
	//Check if transmit reg is empty
	if(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == SET)
	{
		//tx reg is empty so send this byte
		USART_SendData(USART2,txbyte);
	}
	else //tx reg is full, so place txbyte in buffer
	{


	};
};


void AddtoBuffer(RingBuffer* Ringbuf, uint8_t inbyte)
{
	 NewEnd = Ringbuf->end + 1;
	 Ringbuf->buffer[NewEnd] = inbyte;
};


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
