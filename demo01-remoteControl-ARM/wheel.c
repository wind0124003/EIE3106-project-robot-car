#include "init.h"
#include "stm32f10x.h"
#include "stdbool.h"

// variable
int wheel_count_L;
int wheel_count_R;

void wheelInit() {
	RCC_APB2PeriphClockCmd(LEFT_WHEEL_BACKWARD_RCC_GPIO | LEFT_WHEEL_FORWARD_RCC_GPIO
		| RIGHT_WHEEL_BACKWARD_RCC_GPIO | RIGHT_WHEEL_FORWARD_RCC_GPIO
		, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// left wheel forward pin P8
	GPIO_InitStruct.GPIO_Pin = LEFT_WHEEL_FORWARD_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LEFT_WHEEL_FORWARD_GPIO, &GPIO_InitStruct);
	
	// left wheel backward pin Pb13
	GPIO_InitStruct.GPIO_Pin = LEFT_WHEEL_BACKWARD_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LEFT_WHEEL_BACKWARD_GPIO, &GPIO_InitStruct);
	
	// right wheel forward pin 
	GPIO_InitStruct.GPIO_Pin = RIGHT_WHEEEL_FORWARD_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(RIGHT_WHEEL_FORWARD_GPIO, &GPIO_InitStruct);
	
	// right wheel backforward pin
	GPIO_InitStruct.GPIO_Pin = RIGHT_WHEEL_BACKWARD_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(RIGHT_WHEEL_BACKWARD_GPIO, &GPIO_InitStruct);
	
	//set TIM1 1000Hz 
  	//range of PWM: 0 to 1000
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	
	TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Prescaler = 72-1; // 0.001ms
	timerInitStructure.TIM_Period = 1000-1; //1ms
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &timerInitStructure);
	TIM_Cmd(TIM1, ENABLE);
  
	TIM_ARRPreloadConfig(TIM1, ENABLE);
}

void leftForward(unsigned short pwm) {
	TIM_OCInitTypeDef leftForward_InitStruct;
	leftForward_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	leftForward_InitStruct.TIM_Pulse = pwm-1;
	leftForward_InitStruct.TIM_OutputState = TIM_OutputState_Enable;
	leftForward_InitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
	leftForward_InitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  
	TIM_OC1Init(TIM1, &leftForward_InitStruct);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void leftBackward(unsigned short pwm)
{
	TIM_OCInitTypeDef leftBackward_InitStruct;
	leftBackward_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	leftBackward_InitStruct.TIM_Pulse = pwm-1;
	leftBackward_InitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	leftBackward_InitStruct.TIM_OutputState = TIM_OutputState_Disable;
	leftBackward_InitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	
	TIM_OC1Init(TIM1, &leftBackward_InitStruct);
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// a function to enable the right wheel moving backward 
void rightBackward(unsigned short pwm)
{
	TIM_OCInitTypeDef rightBackward_InitStruct;
	rightBackward_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	rightBackward_InitStruct.TIM_Pulse = pwm-1;
	rightBackward_InitStruct.TIM_OutputNState = TIM_OutputNState_Disable;
	rightBackward_InitStruct.TIM_OutputState = TIM_OutputState_Enable;
	rightBackward_InitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	
	TIM_OC2Init(TIM1, &rightBackward_InitStruct);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

// a function to enable the right wheel moving forward 
void rightForward(unsigned short pwm)
{
	TIM_OCInitTypeDef rightFoward_InitStruct;
	rightFoward_InitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	rightFoward_InitStruct.TIM_Pulse = pwm-1;
	rightFoward_InitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	rightFoward_InitStruct.TIM_OutputState = TIM_OutputState_Disable;
	rightFoward_InitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
	
	TIM_OC2Init(TIM1, &rightFoward_InitStruct);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}


void move(bool leftDirection, unsigned short leftPwm, bool rightDirection, unsigned short rightPwm) {
	if (leftPwm <= 0) {
		leftPwm = 1;
	}
	if (leftPwm >= 1000) {
		leftPwm = 999;
	}
	if (rightPwm <= 0) {
		rightPwm = 1;
	}
	if (rightPwm >= 1000) {
		rightPwm = 999;
	}
	
	if (leftDirection == true) {
		leftForward(leftPwm);
	}
	else {
		leftBackward(leftPwm);
	}
	
	if (rightDirection == true) {
		rightForward(rightPwm);
	}
	else {
		rightBackward(rightPwm);
	}
}



void wheelCounterInit(void) {
	RCC_APB2PeriphClockCmd(LEFT_WHEEL_COUNTER_RCC_GPIO| RIGHT_WHEEL_COUNTER_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	// left wheel counter, PB7
	GPIO_InitStruct.GPIO_Pin = LEFT_WHEEL_COUNTER_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LEFT_WHEEL_COUNTER_GPIO, &GPIO_InitStruct);
	
	// right wheel counter, PA1
	GPIO_InitStruct.GPIO_Pin = RIGHT_WHEEL_COUNTER_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(RIGHT_WHEEL_COUNTER_GPIO, &GPIO_InitStruct);
	
	EXTI_InitTypeDef EXTI_InitStruct;
	// EXTI Config, left wheel counter, PB7
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource7);
	EXTI_InitStruct.EXTI_Line = EXTI_Line7;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	
	NVIC_EnableIRQ(EXTI9_5_IRQn);

	// EXTI Config, right wheel counter, PA1
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	EXTI_InitStruct.EXTI_Line = EXTI_Line1;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	
	// enable EXTI interrupt
	NVIC_InitTypeDef NVIC_InitStruct;
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	
	
	// variable 
	wheel_count_R = 0;
	wheel_count_L = 0;
}

void EXTI1_IRQHandler(void) {
	// EXTI interrupt->right wheel counter
	if (EXTI_GetITStatus(EXTI_Line1) != RESET) {
		wheel_count_R++;
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

void EXTI9_5_IRQHandler(void) {
	// EXTI interrupt -> left wheel counter
	if (EXTI_GetITStatus(EXTI_Line7) != RESET) {
		wheel_count_L++;
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}

int getLeftWheelCounter(void) {
	return wheel_count_L;
}

int getRightWheelCounter(void) {
	return wheel_count_R;
}



