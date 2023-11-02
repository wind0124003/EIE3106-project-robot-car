// this main program is for test the hard ware: led, sr04, usart3, button, spi, wheel counter 

// the program is to read the value of spi, sr04 and wheel counter every 50 ms
// and send them to tera term through USART3
// Meanwhile, you can press the button to toggle three colors of leds(yellow, green, blue

// notice: sr04Init() and wheelInit() cannot be caledl at the same time, 
//				the later one will overwrite the previous one, because of tim1's period

#include "init.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"

char buffer[70] = {'\0'};
char character = 20;
char path = 0;
static int flag = 0;


int main() {
	// initalize hardware
	 wheelInit();
 // ledInit();
//	uart3Init();
//	SR04Init();
	uart2Init();
	//wheelCounterInit();
	//buttonInit();
	//spiInit();
	//sysTickInit();
	flag = 0;
	
	while (1) {
		character = USART2Read();
		switch(character) {
			case '3':		// move forward
				move(true, 300, true,300);
				break;
			case '1':	// move backward
				move(false, 270, false, 270);
				break;
			case '2':			// move forward and turn left
				move(true, 180, true, 320);
				break;
			case '0': // move forward and turn right
				move(true, 320, true, 180);
				break;
			case '7': // speed up, forward
				move(true, 250, true, 250);
				break;
			case '5': // turn right
				move(true, 400, true, 100);
				break;
			case '4': // turn left
				move(true,100, true, 420);
				break;
			default:		// stop
				move(true, 0, true, 0);
				break;
		}
	}
}

void EXTI15_10_IRQHandler(void) {		// may put it in 'main.c'
	
	// button interrupt
	if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
//		EXTI_ClearITPendingBit(EXTI_Line12);

}
