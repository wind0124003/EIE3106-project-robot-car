#include "init.h"
#include "stm32f10x.h"

void ledInit() {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(LED_YELLOW_RCC_GPIO, ENABLE);
	RCC_APB2PeriphClockCmd(LED_GREEN_BLUE_RCC_GPIO, ENABLE);
	
	// LED yellow
	GPIO_InitStruct.GPIO_Pin = LED_YELLOW_PIN;		//
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED_YELLOW_GPIO, &GPIO_InitStruct);
	
	// LED blue 
	GPIO_InitStruct.GPIO_Pin = LED_BLUE_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED_BLUE_GPIO,&GPIO_InitStruct);
	
	// LED green
	GPIO_InitStruct.GPIO_Pin = LED_GREEN_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED_GREEN_GPIO, &GPIO_InitStruct);
}

void setLed(enum led_color color, enum led_state s) {
	switch (color) {
		case YELLOW:
			if (s == ON) {
				GPIO_SetBits(LED_YELLOW_GPIO, LED_YELLOW_PIN);
			}
			else if (s == OFF) {
				GPIO_ResetBits(LED_YELLOW_GPIO, LED_YELLOW_PIN);
			}
			else if(s == BLINK) {
				setYellowLedBlink();
			}
			break;
		case BLUE:
			if (s == ON) {
				GPIO_ResetBits(LED_BLUE_GPIO, LED_BLUE_PIN);
			}
			else if (s == OFF) {
				GPIO_SetBits(LED_BLUE_GPIO, LED_BLUE_PIN);
			}
			else if (s == BLINK) {
				setBlueLedBlink();
			}
			break;
		case GREEN:
				if (s == ON) {
				GPIO_SetBits(LED_GREEN_GPIO, LED_GREEN_PIN);
			}
			else if (s == OFF) {
				GPIO_ResetBits(LED_GREEN_GPIO, LED_GREEN_PIN);
			}
			else if (s == BLINK) {
				setGreenLedBlink();
			}
			break;
		}
}

void setYellowLedBlink() {
	setLed(YELLOW, ON);
	delay_ms(500);
	setLed(YELLOW, OFF);
	delay_ms(500);
}

void setBlueLedBlink(){
	setLed(BLUE, ON);
	delay_ms(500);
	setLed(BLUE, OFF);
	delay_ms(500);
}

void setGreenLedBlink() {
	setLed(GREEN, ON);
	delay_ms(500);
	setLed(GREEN, OFF);
	delay_ms(500);
}

void buttonInit() {
	RCC_APB2PeriphClockCmd(BUTTON_RCC_GPIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = BUTTON_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(BUTTON_GPIO, &GPIO_InitStruct);
	
	// EXTI Config
	GPIO_EXTILineConfig(BUTTON_GPIO_PORTSOURCE, BUTTON_GPIO_PINSOURCE);
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line12;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
	
	// enable EXTI interrupt
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}



