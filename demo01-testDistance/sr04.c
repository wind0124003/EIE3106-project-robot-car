#include "init.h"
#include "stm32f10x.h"

// variable
__IO bool pulseHigh = false;
__IO bool generatePulse = true;
__IO u32 pulseWidth = 0;
__IO u32 captureValue = 0;
__IO u32 distance = 0;

void SR04Init(void) {
	RCC_APB2PeriphClockCmd(SR04_ECHO_RCC_GPIO | SR04_TRIG_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// echo pin PA10
	GPIO_InitStruct.GPIO_Pin = SR04_ECHO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SR04_ECHO_GPIO, &GPIO_InitStruct);
	
	// config trig pin , not enable tim1 ch3n
	RCC_APB2PeriphClockCmd(SR04_TRIG_RCC_GPIO, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = SR04_TRIG_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(SR04_TRIG_GPIO, &GPIO_InitStruct);
	

	// set TIM1 in 1000 Hz
	// pulse range: 0 to 1000
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	TIM_TimeBaseInitTypeDef timerInitStruct;
	timerInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStruct.TIM_Prescaler = 72 - 1;			// 1/(72MHz/72) = 1us,
	timerInitStruct.TIM_Period  = 10000	- 1;		// 1us * 10000 = 10 ms
	timerInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &timerInitStruct);
	TIM_Cmd(TIM1, ENABLE);
	
	// enable tim1 ch3 input capture
	TIM_ICInitTypeDef IC_InitStruct;
	IC_InitStruct.TIM_Channel = TIM_Channel_3;
	IC_InitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	IC_InitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	IC_InitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	IC_InitStruct.TIM_ICFilter = 0;
	TIM_ICInit(TIM1, &IC_InitStruct);
	
	//enable input capture interrput
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = TIM1_CC_IRQn; //TIM1 capture compare interrput
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	//allow update to interrput, allows the CC3IE to capture interrput
	
	 TIM_ITConfig(TIM1, TIM_IT_Update|TIM_IT_CC3, ENABLE);
}

void triggerSR04() {
	GPIO_SetBits(SR04_TRIG_GPIO, SR04_TRIG_PIN);
	delay_us(10);
	delay_stop();
	GPIO_ResetBits(SR04_TRIG_GPIO, SR04_TRIG_PIN);
}

// Get the measurement from the ultrasonic sensor
void TIM1_CC_IRQHandler(void)
{
    	if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)
	{
      		if(!pulseHigh)
		{
        		pulseHigh = true;   //pulse starts
        		TIM_SetCounter(TIM1, 0);
		        //change to detect the falling edge
        		TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Falling);
	      	}
		else
		{
        		pulseWidth += TIM_GetCounter(TIM1);
        		captureValue = pulseWidth;
			// output an integer only not a floating point number
			// very similar to captureValue/2*330/1000000
					distance = (captureValue/2)/30; // in cm

			//change to detect the rising edge
	        	TIM_OC3PolarityConfig(TIM1, TIM_ICPolarity_Rising);
		        pulseHigh = false;
		        pulseWidth = 0;
	      	}
    	}
	//clear interrupt flag
  	TIM_ClearITPendingBit(TIM1, TIM_IT_Update|TIM_IT_CC3);
}

int getDistance() {
	return distance;
}