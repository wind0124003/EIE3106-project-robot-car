#include <stdio.h>
#include "init.h"
#include "myFunction.h"

char buffer[70];
volatile unsigned int distance;


int main(void) {
	move(true, 30000, true, 30000);
	wait(5);
	
	while (1) {
    wait(1);
		distance = getDistance();
		
		if (distance <= 47) {
			move(true, 0,true, 0);
		}
		else if (distance > 47 && distance <= 70) {
			move(true, 10500, true, 10500);
		}
		else if (distance > 70 && distance <= 90) {
			move(true, 12000, true, 12000);
		}
		else if (distance > 90) {
			move(true, 13500, true, 13500);
		}
	}
}

void on_button(bool b) {
  static int state = 0;
  if (b) {
		if (state == 1) {
			state = 0;
		}
		else {
			state++;
		}
		
		
			
		
	}
}
