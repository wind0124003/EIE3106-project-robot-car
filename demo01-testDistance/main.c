// This program is to trigger sr04, and get the distance between sr04 and obstacle
// then, the result will send to tera term through usart3

#include "init.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"

char buffer[70] = {'\0'}; 
 
int main() {
	// initalize hardware
	uart3Init();
	SR04Init();
	sysTickInit();
	
	while (1) {
		delay_ms(60);
		triggerSR04();

		sprintf(buffer, "\rThe distance is %d cm" , 
					getDistance()-6);
    USART3Send(buffer, sizeof(buffer));
		
	}
}

void EXTI15_10_IRQHandler(void) {		// may put it in 'main.c'
	
	// button interrupt
	if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
//		EXTI_ClearITPendingBit(EXTI_Line12);

}
