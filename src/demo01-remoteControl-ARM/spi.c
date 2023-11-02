#include "init.h"
#include "stm32f10x.h"
#include "stdbool.h"

int count;
char spiValue = 0;
bool state = false;

void spiInit() {
	
	RCC_APB2PeriphClockCmd(SPI_SCK_MISO_MOSI_RCC_GPIO|RCC_APB2Periph_AFIO, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// SPI1 SCK pin, PA5
	GPIO_InitStructure.GPIO_Pin = SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(SPI_SCK_GPIO, &GPIO_InitStructure);
	
	// SPI1 MISO pin, PA6
	GPIO_InitStructure.GPIO_Pin = SPI_MISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI_MISO_GPIO, &GPIO_InitStructure);
	
	// SPI1 MOSI pin, PA7
	GPIO_InitStructure.GPIO_Pin = SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_MOSI_GPIO, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	// SPI initialization
	SPI_InitTypeDef SPI_InitStructure;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	// 36 MHz / 256 = 140.625 kHz
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Init(SPI1, &SPI_InitStructure);
	
	// Enable the receive interrupt
	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
	// Enable SPI1
	SPI_Cmd(SPI1, ENABLE);
}

void SPI1_IRQHandler(void)
{
	spiValue = (char) SPI_I2S_ReceiveData(SPI1) & 0xff;

	// the received character has all the readings
	if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_7) == 1)
	{
		if (count != 0)
		{
			// keep turning on LEDs
			SPI_I2S_SendData(SPI1, 0xff);
			count--;
		}
		else
		{
			// Set PA7 to 0 to trigger the shift register
			GPIO_ResetBits(GPIOA, GPIO_Pin_7);
			// Go to get the next reading
			SPI_I2S_SendData(SPI1, 0);
			state = true;
		}
	} 
	else 
	{
		if(count == 0 && state == true)
		{
			state = false;
		}
		// disable the interrupt because it is not ready
		NVIC_DisableIRQ(SPI1_IRQn);
	}
}

// get the readings once
void readFloor() 
{
	// Set PA7 to 1
	GPIO_SetBits(GPIOA, GPIO_Pin_7);
	// Initialize the data transmission from the master to the slave
	SPI_I2S_SendData(SPI1, 0);
	count = 4;
	// Enable the interrupt to receive data by using the ISR handler
	NVIC_EnableIRQ(SPI1_IRQn);
}

char getPath() {
	return spiValue;
}