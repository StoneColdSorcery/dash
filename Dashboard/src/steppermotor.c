/*
 * steppermotor.c
 *
 *  Created on: Feb 12, 2015
 *      Author: Crazyfingers
 */

#include "steppermotor.h"

GPIO_TypeDef* SM_GPIO_PORT[6] = {SM_1A_GPIO_PORT,SM_2A_GPIO_PORT,SM_3A_GPIO_PORT,SM_4A_GPIO_PORT,
		SM_EN12_GPIO_PORT,  SM_EN34_GPIO_PORT};

const uint16_t SM_GPIO_PIN[6] = {SM_1A_GPIO_PIN,SM_2A_GPIO_PIN,SM_3A_GPIO_PIN,SM_4A_GPIO_PIN,
		SM_EN12_GPIO_PIN,  SM_EN34_GPIO_PIN};

const uint32_t SM_GPIO_CLK[6] = {SM_1A_GPIO_CLK,SM_2A_GPIO_CLK,SM_3A_GPIO_CLK,SM_4A_GPIO_CLK,
		SM_EN12_GPIO_CLK,  SM_EN34_GPIO_CLK};

const uint16_t Statemap[6] = {STATE1,STATE2,STATE3,STATE4,STATE5,STATE6};

__IO uint16_t currentState;
__IO uint32_t Stepper_PrescalerValue;
__IO uint16_t Stepper_ForwardFlag = 0;
__IO uint16_t Stepper_BackwardFlag = 0;

uint32_t SMClk_T;
uint32_t SMClk_pulse;

void SM_PinInit(SMPin_TypeDef cPin)
{


  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(SM_GPIO_CLK[cPin], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = SM_GPIO_PIN[cPin];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SM_GPIO_PORT[cPin], &GPIO_InitStructure);

}

void SM_Setup(void)
{
	SM_PinInit(SM_1A);
	SM_PinInit(SM_2A);
	SM_PinInit(SM_3A);
	SM_PinInit(SM_4A);
	SM_PinInit(SM_EN12);
	SM_PinInit(SM_EN34);

	SM_PinHigh(SM_EN12);
	SM_PinHigh(SM_EN34);

	SM_TimInit();

	GPIO_ResetBits(SM_1A_GPIO_PORT ,DBPINS);
	currentState = 1;

	SM_Enable();
}

void SM_TimInit(void)
{



	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_SMTimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	//GPIO CONFIG--------------------------------------------------

	  /* Enable the GPIO_LED Clock */
	 // RCC_AHB1PeriphClockCmd(SRCK_GPIO_CLK, ENABLE);


	//NVIC CONFIG----------------------------------------

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	//TIMER CONFIG---------------------------------------------

	RCC_APB1PeriphClockCmd(SM_TIM_CLKSRC, ENABLE);

	/* Compute the prescaler value */
	Stepper_PrescalerValue = (uint16_t) (SystemCoreClock /(1 * SM_TIM_CNTCLK) - 1);


	//Calculate timer and CCR periods from frequency and prescaler value
	SMClk_T = (SM_TIM_CNTCLK / SM_TIM_OUTCLK) - 1;
	//SMClk_pulse =  (SMClk_T / ( (uint32_t)100 / (uint32_t)SM_TIM_DUTY )) - 1;

	/* Time base configuration */
	TIM_SMTimeBaseStructure.TIM_Period = SMClk_T;
	TIM_SMTimeBaseStructure.TIM_Prescaler = Stepper_PrescalerValue;
	TIM_SMTimeBaseStructure.TIM_ClockDivision = 0;
	TIM_SMTimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(SM_TIM, &TIM_SMTimeBaseStructure);

		/* Enable TIM4 Preload register on ARR */
	TIM_ARRPreloadConfig(SM_TIM, ENABLE);



	TIM_ITConfig(SM_TIM, TIM_IT_Update, ENABLE);

	//disable OC output
	//SRCK_TIM->CCER &= (uint16_t)~TIM_CCER_CC1E;







	//TIM_Cmd(SRCK_TIM, ENABLE);
}

void SM_Enable()
{
	SM_PinHigh(SM_EN12);
	SM_PinHigh(SM_EN34);
	TIM_Cmd(SM_TIM, ENABLE);
}

void SM_Disable()
{
	SM_PinLow(SM_EN12);
	SM_PinLow(SM_EN34);
	Stepper_ForwardFlag = 0;
	Stepper_BackwardFlag = 0;
	//TIM_Cmd(SRCK_TIM, DISABLE);
}

void SM_TimerStop(void)
{
	TIM_Cmd(SM_TIM, DISABLE);
}

void SM_TimerForward(void)
{
	Stepper_ForwardFlag = 1;
	Stepper_BackwardFlag = 0;
}

void SM_TimerBackward(void)
{
	Stepper_ForwardFlag = 0;
	Stepper_BackwardFlag = 1;

}

void SM_PinHigh(SMPin_TypeDef cPin)
{
	SM_GPIO_PORT[cPin]->BSRRL = SM_GPIO_PIN[cPin];
}


void SM_PinLow(SMPin_TypeDef cPin)
{
	SM_GPIO_PORT[cPin]->BSRRH = SM_GPIO_PIN[cPin];
}

void SM_PinToggle(SMPin_TypeDef cPin)
{
	SM_GPIO_PORT[cPin]->ODR ^= SM_GPIO_PIN[cPin];
}

void SM_StepForward(void)
{
	GPIO_ResetBits(SM_1A_GPIO_PORT ,DBPINS);
	currentState =  (currentState + 1) % 6;
	GPIO_SetBits(SM_1A_GPIO_PORT ,Statemap[currentState]);
}

void SM_StepBackward(void)
{
	GPIO_ResetBits(SM_1A_GPIO_PORT ,DBPINS);
	if(currentState == 0) currentState = 5;
	else currentState =  currentState - 1;
	GPIO_SetBits(SM_1A_GPIO_PORT ,Statemap[currentState]);
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(SM_TIM, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(SM_TIM, TIM_IT_Update);
		if(Stepper_ForwardFlag == 1)
		{
			SM_StepForward();
		}
		else if(Stepper_BackwardFlag == 1)
		{
			SM_StepBackward();
		}
	}

}
