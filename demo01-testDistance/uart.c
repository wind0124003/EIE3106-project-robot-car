#include "stm32f10x.h"
#include "init.h"

void uart3Init() {
	// USART3 TX RX
	RCC_APB2PeriphClockCmd(USART3_TX_RX_RCC_GPIO | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	// TX pin, PB10
	GPIO_InitStruct.GPIO_Pin = USART3_TX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(USART3_TX_GPIO, &GPIO_InitStruct);
	
	// RX pin, PB11
	GPIO_InitStruct.GPIO_Pin = USART3_RX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART3_RX_GPIO, &GPIO_InitStruct);
	
	// USART3 ST-LINK USB
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
  
  USART_InitTypeDef USART_InitStruct;
  USART_InitStruct.USART_BaudRate = 115200;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Rx |USART_Mode_Tx;
  
  USART_Init(USART3, &USART_InitStruct);
  USART_Cmd(USART3,ENABLE);
}
	
	



void uart2Init() {
	RCC_APB2PeriphClockCmd(USART2_TX_RX_RCC_GPIO | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	// Tx pin
	GPIO_InitStruct.GPIO_Pin = USART2_TX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(USART2_TX_GPIO, &GPIO_InitStruct);
	
	// Rx pin
	GPIO_InitStruct.GPIO_Pin = USART2_RX_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(USART2_RX_GPIO, &GPIO_InitStruct);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 9600;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
 	USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_Init(USART2, &USART_InitStruct);
	USART_Cmd(USART2, ENABLE);
	
//	// enable the USART3 RX interrupt (delete)
//	NVIC_InitTypeDef NVIC_InitStruct;
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
//	NVIC_Init(&NVIC_InitStruct);
}

void USART3Send(char *pucBuffer, unsigned long ulCount) {
	while (ulCount--) {
		USART_SendData(USART3, *pucBuffer++);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET){
		}
	}
}

unsigned char USART3Read() {
	while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
		return USART_ReceiveData(USART3) & 0xFF;
}

unsigned char USART2Read() {
	while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET);
		return USART_ReceiveData(USART2) & 0xFF;
}