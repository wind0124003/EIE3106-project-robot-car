#include "init.h"
#include "stm32f10x.h"

static __IO uint32_t msTicks;

void delay_ms(uint32_t ms) {
	msTicks = ms;
	while(msTicks);
}

// call this function every 1ms
void SysTick_Handler() {
	if (msTicks != 0) {
		msTicks--;
	}
}

void sysTickInit() {
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
}

void delay_us(unsigned short period) {
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseInitTypeDef timerInitStruct;
	timerInitStruct.TIM_Prescaler = 72;	// 1*us , 1/(72MHz/7200)
	timerInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStruct.TIM_Period = period - 1; // 1 us * period = delay_us
	timerInitStruct.TIM_RepetitionCounter = 0;
	timerInitStruct.TIM_ClockDivision = 0;
	TIM_TimeBaseInit(TIM3, &timerInitStruct);
	TIM_Cmd(TIM3, ENABLE);
	
	// interrupt
	 TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	
}

void delay_stop() {
	while (TIM_GetFlagStatus(TIM3, TIM_IT_Update) == RESET);
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, DISABLE);
		TIM_Cmd(TIM3, DISABLE);
//		TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
//		NVIC_DisableIRQ(TIM3_IRQn);
}