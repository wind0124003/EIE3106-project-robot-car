#include <stm32f10x.h>
#include "init.h"
#include "myfunction.h"

void move(bool leftDirection, unsigned int pwmL, bool rightDirection, unsigned int pwmR) {
	if (pwmL < 0) {
		pwmL = 0;
	}
	else if (pwmL > 44999) {
		pwmL = 44999;
	}
	setLeft(leftDirection);
	pwmLeft(pwmL);
	
	if (pwmR < 0) {
		pwmR = 0;
	}
	else if (pwmR > 44999) {
		pwmR = 44999;
	}
	setRight(rightDirection);
	pwmRight(pwmR);
}
