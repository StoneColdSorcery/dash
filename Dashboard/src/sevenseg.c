/*
 * sevenseg.c
 *
 *  Created on: Dec 17, 2014
 *      Author: Crazyfingers
 */

#include "sevenseg.h"



GPIO_TypeDef* SEVSEG_GPIO_PORT[CTRLn] = {SOUT_GPIO_PORT, SRCLR_GPIO_PORT, EN_GPIO_PORT,
		RCK_GPIO_PORT, SRCK_GPIO_PORT};

const uint16_t SEVSEG_GPIO_PIN[CTRLn] = {SOUT_GPIO_PIN, SRCLR_GPIO_PIN, EN_GPIO_PIN,
        RCK_GPIO_PIN, SRCK_GPIO_PIN};

const uint32_t SEVSEG_GPIO_CLK[CTRLn] = {SOUT_GPIO_CLK, SRCLR_GPIO_CLK, EN_GPIO_CLK,
        RCK_GPIO_CLK, SRCK_GPIO_CLK};

GPIO_TypeDef* DIG_GPIO_PORT[NDIGITS] = {DIG1_GPIO_PORT,DIG2_GPIO_PORT,DIG3_GPIO_PORT,DIG4_GPIO_PORT};

const uint32_t DIG_GPIO_PIN[NDIGITS] = {DIG1_GPIO_PIN,DIG2_GPIO_PIN,DIG3_GPIO_PIN,DIG4_GPIO_PIN};

const uint32_t DIG_GPIO_CLK[NDIGITS] = {DIG1_GPIO_CLK,DIG2_GPIO_CLK,DIG3_GPIO_CLK,DIG4_GPIO_CLK};

const uint16_t BCD[10] =
{
	0x3f, //0
	0x06, //1
	0x5b, //2
	0x4f, //3
	0x66, //4
	0x6d, //5
	0x7d, //6
	0x07, //7
	0x7f, //8
	0x67 //9
};


volatile uint8_t DigSelVector[NDIGITS] = {0,0,0,0};
volatile uint32_t curDig = 0;
volatile uint32_t nextDig = 0;

__IO uint32_t SevenSeg_PrescalerValue;
__IO uint16_t srOutByte = 0;
__IO uint16_t srTxMask = 0;
__IO uint16_t srTxTrig = 0;
__IO uint16_t srCleanup = 0;

uint32_t srClk_T;
uint32_t srClk_pulse;

//globals
__IO uint16_t srBusy = 0;


//srTxTrig = 0;

void Sevenseg_PinInit(ShiftRegPin_TypeDef cPin)
{


  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(SEVSEG_GPIO_CLK[cPin], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = SEVSEG_GPIO_PIN[cPin];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(SEVSEG_GPIO_PORT[cPin], &GPIO_InitStructure);

}

void Dig_PinInit(DigPin_TypeDef cPin)
{


  GPIO_InitTypeDef  GPIO_InitStructure;

  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(DIG_GPIO_CLK[cPin], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = DIG_GPIO_PIN[cPin];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DIG_GPIO_PORT[cPin], &GPIO_InitStructure);


}




void Dig_PinHigh(DigPin_TypeDef cPin)
{
	DIG_GPIO_PORT[cPin]->BSRRL = DIG_GPIO_PIN[cPin];
}

void Dig_PinLow(DigPin_TypeDef cPin)
{
	DIG_GPIO_PORT[cPin]->BSRRH = DIG_GPIO_PIN[cPin];
}

void Dig_PinToggle(DigPin_TypeDef cPin)
{
	DIG_GPIO_PORT[cPin]->ODR ^= DIG_GPIO_PIN[cPin];
}

void SelectDigit(DigPin_TypeDef cPin)
{
	Dig_PinLow(D1);
	Dig_PinLow(D2);
	Dig_PinLow(D3);
	Dig_PinLow(D4);

	Dig_PinHigh(cPin);
}


void Sevenseg_PinHigh(ShiftRegPin_TypeDef cPin)
{
	SEVSEG_GPIO_PORT[cPin]->BSRRL = SEVSEG_GPIO_PIN[cPin];
}

void Sevenseg_PinLow(ShiftRegPin_TypeDef cPin)
{
	SEVSEG_GPIO_PORT[cPin]->BSRRH = SEVSEG_GPIO_PIN[cPin];
}

void Sevenseg_PinToggle(ShiftRegPin_TypeDef cPin)
{
	SEVSEG_GPIO_PORT[cPin]->ODR ^= SEVSEG_GPIO_PIN[cPin];
}


void Sevenseg_TimInit(void)
{


	GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_7TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	//GPIO CONFIG--------------------------------------------------

	  /* Enable the GPIO_LED Clock */
	  RCC_AHB1PeriphClockCmd(SRCK_GPIO_CLK, ENABLE);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.GPIO_Pin = SRCK_GPIO_PIN;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_Init(SRCK_GPIO_PORT, &GPIO_InitStructure);


	//NVIC CONFIG----------------------------------------

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	//TIMER CONFIG---------------------------------------------

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	/* Compute the prescaler value */
	SevenSeg_PrescalerValue = (uint16_t) (SystemCoreClock /(1 * SR_TIM_CNTCLK) - 1);


	//Calculate timer and CCR periods from frequency and prescaler value
	srClk_T = (SR_TIM_CNTCLK / SR_TIM_OUTCLK) - 1;
	srClk_pulse =  (srClk_T / ( (uint32_t)100 / (uint32_t)SR_TIM_DUTY )) - 1;

	/* Time base configuration */
	TIM_7TimeBaseStructure.TIM_Period = srClk_T;
	TIM_7TimeBaseStructure.TIM_Prescaler = SevenSeg_PrescalerValue;
	TIM_7TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_7TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(SRCK_TIM, &TIM_7TimeBaseStructure);

	 GPIO_PinAFConfig(SRCK_GPIO_PORT,SRCK_TIM_PINSRC, GPIO_AF_TIM4);
	/* Enable TIM4 Preload register on ARR */
	TIM_ARRPreloadConfig(SRCK_TIM, ENABLE);

	/* TIM PWM1 Mode configuration: Channel */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = srClk_pulse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;

	/* Output Compare PWM1 Mode configuration: Channel1 */
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);
	TIM_CCxCmd(TIM4, TIM_Channel_1, ENABLE);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

	TIM_ITConfig( SRCK_TIM, TIM_IT_Update, ENABLE);

	//disable OC output
	//SRCK_TIM->CCER &= (uint16_t)~TIM_CCER_CC1E;



	Sevenseg_PinHigh(EN); //initially disabled

	//TIM_Cmd(SRCK_TIM, ENABLE);
}


void Sevenseg_Setup(void)
{
	Sevenseg_PinInit(SOUT);
	Sevenseg_PinInit(SRCLR);
	Sevenseg_PinInit(EN);
	Sevenseg_PinInit(RCK);
	//Sevenseg_PinInit(SRCK);

	Dig_PinInit(D1);
	Dig_PinInit(D2);
	Dig_PinInit(D3);
	Dig_PinInit(D4);

	Sevenseg_TimInit();


	//turn on sreg output

	Sevenseg_PinLow(RCK);
	Sevenseg_PinLow(SOUT);
	Sevenseg_PinLow(EN);
	Sevenseg_PinHigh(SRCLR);

	Dig_PinLow(D1);
	Dig_PinLow(D2);
	Dig_PinLow(D3);
	Dig_PinLow(D4);


}

void Sevenseg_On()
{
	Sevenseg_PinLow(EN);
	TIM_Cmd(SRCK_TIM, ENABLE);
	srTxTrig = 1;
}

void Sevenseg_Off()
{
	TIM_Cmd(SRCK_TIM, DISABLE);
	Sevenseg_PinHigh(EN);

}

void Sevenseg_Update(uint32_t val)
{
	GenerateDigits(DigSelVector,val);
}

void GenerateDigits(volatile uint8_t* dv,uint32_t n)
{
	int rem;
	int num = n;
	int i = 0;

	for(i = NDIGITS - 1; i >= 0; i--)
	{
		rem = num % 10;
		dv[i] = BCD[rem];
		num = (num - rem) / 10;
	}
}



void Sevenseg_Send(DigPin_TypeDef dSelect , uint16_t sendbyte )
{

	if(srBusy == 0)
	{

		//SelectDigit(dSelect);
		srOutByte = BCD[sendbyte];
		//srOutByte = sendbyte;
		srTxTrig = 1;
		//Sevenseg_PinLow(SRCLR);



		/*
		tmpccer = TIMx->CCER;
		TIM_CCER_CC1NE
		*/
	}

}


void TIM4_IRQHandler(void)
{

	//Dig_PinToggle(D1);

	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

		if(srTxTrig == 1) // start condition
		{
			srBusy = 1;
			srTxTrig = 0;
			//SRCK_TIM->CCER |= (uint16_t)TIM_CCER_CC1E;

			//increment nextdig
			nextDig = (nextDig + 1) % NDIGITS;

			//start loading next digits byte into SR
			srOutByte = DigSelVector[nextDig]; //
			//srToSend = 8;
			//Sevenseg_PinLow(RCK);
			//Sevenseg_PinHigh(SRCLR);
			srTxMask = 0x0001;
		}

		// Set Shiftreg pins if needed
		if(srTxMask > 0x0080) // have sent 8 bits, push to output reg and reset
		{
			Dig_PinLow(curDig);// turn off current di
			//curDig = (curDig + 1) % NDIGITS; // move to next digit
			curDig = nextDig;
			Dig_PinHigh(nextDig);
			Sevenseg_PinHigh(RCK);

			//SRCK_TIM->CCER &= (uint16_t)~TIM_CCER_CC1E;
			/*

			*/
			srTxMask = 0x00; // reset mask
			srBusy = 0;
			srTxTrig = 1;// start loading SR again next cycle
			Sevenseg_PinLow(RCK);
		}
		else if(srTxMask > 0x0000) // still have bits to send
		{
			if( (srTxMask & srOutByte) == 0x0000)
			//send low bit
			{
				Sevenseg_PinLow(SOUT);
			}
			else
			//send high bit
			{
				Sevenseg_PinHigh(SOUT);
			}
			//Sevenseg_PinHigh(RCK);
			srTxMask = srTxMask << 1; // advance bitmask
			//Sevenseg_PinLow(RCK);
		}






	}
}





