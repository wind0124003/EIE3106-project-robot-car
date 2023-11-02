/*
this program is for the stm32 robot car to finish line tracking task. 
*/
#include <stdio.h>
#include "stm32f10x.h"
#include "init.h"
#include "car.h"

int delayO2I = 0;
int delayY = 0;
int delay3 = 0;



// put the robot car's left hand side near the black line
int main(void) {
	// polyu: Car car(10000, 11000, 595, 0,0);
	// 11000, 12000, 598,0,20
	Car car(11500, 12500, 610, 0,20);
	
  while (1) {
		wait(5);
		
    car.track_detect();
		switch(car.stateCar) {
			case OUTER:		
				car.calculateError_outer();
				car.tracker();
				break;
			case READYFORB:
				led(GREEN, ON);
				car.changeBasicSpeed(9000, 14000); // 10000, 14000
				car.changePID(550, 0, 50); // car.changePID(590, 0, 40);
				car.checkFlagB();
				car.calculateError_readyForB();
				car.tracker();
				break;
			case OUTER2INNER:
				
				delayO2I++;
				led(GREEN, OFF);
				//car.changePID(590, 0, 40); //car.changePID(590, 0, 40);
			//move(true, 0, true,0);
				if (delayO2I < 5) { // 10
					move(true, 0, true, 0);
				}
				else if (delayO2I >= 5 && delayO2I < 70) { // 10, 65
					move(true, 0, true, 18000);
				}
				else {
					//move(true, 0 ,true, 0);
					car.setState(INNER);
				}
					
				break;
			
			case INNER:
				led(YELLOW, ON);
			
				car.changeBasicSpeed(11500, 12500); // 10000,11000
				car.changePID(700,0,100);
				car.calculateError_inner();
				car.tracker();
				car.checkFlagY();
				break;
			
			case INNER_YKDJ:
				delayY ++;
				if (delayY < 10) { // 10
					move(true, 0, true, 0);
				}
				else if (delayY >= 10 && delayY < 60) { // 10, 65
					move(true, 18000, true, 0);
				}
				else if (delayY >= 60) {
					led(YELLOW, ON);
					led(GREEN, ON);
					car.changeBasicSpeed(13000, 9000);
					car.changePID(690, 0,100);    //d = 200
					car.calculateError_innerYKDJ();
					car.tracker();
					car.checkFlagY_2();			
				}
				break;
			
			case YMB:
				delay3 ++;
				if (delay3 < 10) { // 10
					move(true, 0, true, 0);
				}
				else if (delay3 >= 10 && delay3 < 80) { // 10, 65
					move(true, 0, true, 18000);
				}
				else {
					car.changeBasicSpeed(11500, 12500);
					car.changePID(690,0,150);    //d = 200
					car.calculateError_outer();
					car.tracker();
				}
				break;
			case STOP:
				move(true, 0, true, 0);
				led(YELLOW, OFF);
				led(GREEN, OFF);
		}
		
	}
}

void on_button(bool b) {
  static int state;
  if (b) {
		if (state++ > 1) {
			state = 0;
		}
	}
}
