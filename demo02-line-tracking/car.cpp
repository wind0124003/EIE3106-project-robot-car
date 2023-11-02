#include <stm32f10x.h>
#include "init.h"
#include <stdio.h>
#include "car.h"

int delay_readyForB = 0;
char targetAC = 20;
char targetB = 32;


// white -> black -> white
void Car::checkFlagY_2() {
	countY++;
	
	if (countY > 320) {
		led(BLUE, OFF);
		if (p8 == 1) {
			if (ifWhite_1DY == 1) {
				ifBlack_1DY = 1;
			}
		}
		else {
			if (ifWhite_1DY == 0) {
				ifWhite_1DY = 1;
			
			}
			else {
				if (ifBlack_1DY == 1) {
					ifWhite_2DY = 1;
					led(YELLOW, OFF);
				}
			}
		}

		if (ifWhite_1DY == 1 && ifBlack_1DY == 1 && ifWhite_2DY == 1) {
			stateCar = YMB;
			flagY = 0;
		}
			
		}
}
		
void Car::calculateError_innerYKDJ () {
	if ((p1 == 0) && (p2 == 0) && (p3 == 1) && (p4 == 1) && (p5 == 1) && (p6 == 0) && (p7 == 0) && (p8 == 0)) {  // p3 p4 p5
			error = -6;
		}
		else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 1) && (p5 == 1) && (p6 == 1) && (p7 == 0) && (p8 == 0)) { // p4 p5 p6
			error = 6;
		}
		else if ((p1 == 0) && (p2 == 1) && (p3 == 1) && (p4 == 1) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p2 p3 p4
			error = -6;
		}
		else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 1) && (p6 == 1) && (p7 == 1) && (p8 == 0)) { // p5 p6 p7
			error = 6;
		}
		else if ((p1 == 1) && (p2 == 1) && (p3 == 1) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1 p2 p3
			error = -8;
		}
		else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 1) && (p7 == 1) && (p8 == 1)) { // p6 p7 p8
			error = 8;
		}
		else if ((p1 == 1) && (p2 == 1) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1 p2
			error = -11;
		}
		else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 1) && (p8 == 1)) { // p7 p8
			error = 11;
	}
		else if ((p1 == 1) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1
			error = -11;
		}
		else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 1)) { // p8
			error = 11;
		}
	}


void Car::checkFlagY() {
		// check Y point: white -> black->white->black
		if (p1 == 0) {
			if(ifWhite_1Y == 0) { // white 1
				ifWhite_1Y = 1;
			}
			else if (ifBlack_1Y == 1) { // white 2
				ifWhite_2Y = 1;
			}
			
		}
		else { 
			if (ifWhite_1Y == 1 && ifBlack_1Y == 0) { // black 1
				ifBlack_1Y = 1;
			}
			else if (ifWhite_2Y == 1) {// black 2
				ifBlack_2Y = 1;
			}
		}
		
		if (ifWhite_1Y == 1 && ifBlack_1Y == 1 && ifWhite_2Y == 1 && ifBlack_2Y == 1) {
			flagY = 1;
			stateCar = INNER_YKDJ;
			integral = 0;
			lastError = 0;
		}
}

void Car::calculateError_inner() {
		if ((p1 == 0) && (p2 == 0) && (p3 == 1) && (p4 == 1) && (p5 == 1) && (p6 == 0) && (p7 == 0) && (p8 == 0)) {  // p3 p4 p5
			error = 0;
		}
		else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 1) && (p5 == 1) && (p6 == 1) && (p7 == 0) && (p8 == 0)) { // p4 p5 p6
			error = -3;
		}
		else if ((p1 == 0) && (p2 == 1) && (p3 == 1) && (p4 == 1) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p2 p3 p4
			error = 7;
		}
		else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 1) && (p6 == 1) && (p7 == 1) && (p8 == 0)) { // p5 p6 p7
			error = -7;
		}
		else if ((p1 == 1) && (p2 == 1) && (p3 == 1) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1 p2 p3
			error = 10;
		}
		else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 1) && (p7 == 1) && (p8 == 1)) { // p6 p7 p8
			error = -10;
		}
		else if ((p1 == 1) && (p2 == 1) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1 p2
			error = 15;
		}
		else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 1) && (p8 == 1)) { // p7 p8
			error = -15;
		}
		else if ((p1 == 1) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1
			error = 20;
		}
		else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 1)) { // p8
			error = -20;
		}
}

// check flag B is on or off
void Car::checkFlagB() {
	delay_readyForB++;
	
	if (flagAC == 1 && delay_readyForB > targetB) {
		// check B point: black->white->black
		if (p8 == 1) {
			if (ifBlack_1B == 0) {
				ifBlack_1B = 1;
			}
			else if (ifWhite_1B == 1) {
				ifBlack_2B = 1;
			}
		}
		else {
			if (ifBlack_1B == 1) {
				ifWhite_1B = 1;
			}
		}
		
		if (ifBlack_1B == 1 && ifWhite_1B == 1 && ifBlack_2B == 1) {
			flagB = 1;
			stateCar = OUTER2INNER;
			integral = 0;
		}
	}
}


void Car::calculateError_readyForB() {
	if ((p1 == 0) && (p2 == 0) && (p3 == 1) && (p4 == 1) && (p5 == 1) && (p6 == 0) && (p7 == 0) && (p8 == 0)) {  // p3 p4 p5
		error = 0;
	}
	else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 1) && (p5 == 1) && (p6 == 1) && (p7 == 0) && (p8 == 0)) { // p4 p5 p6
		error = -1;
	}
	else if ((p1 == 0) && (p2 == 1) && (p3 == 1) && (p4 == 1) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p2 p3 p4
		error = 4;
	}
	else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 1) && (p6 == 1) && (p7 == 1) && (p8 == 0)) { // p5 p6 p7
		error = -4;
	}
	else if ((p1 == 1) && (p2 == 1) && (p3 == 1) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1 p2 p3
		error = 9;
	}
	else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 1) && (p7 == 1) && (p8 == 1)) { // p6 p7 p8
		error = -9;
	}
	else if ((p1 == 1) && (p2 == 1) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1 p2
		error = 15;
	}
	else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 1) && (p8 == 1)) { // p7 p8
		error = -15;
}
	else if ((p1 == 1) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1
		error = 20;
	}
	else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 1)) { // p8
		error = -20;
	}
}

Car::Car() {
	basicSpeedL = 10000;
	basicSpeedR = 10500;
	turn = 0;
	p1 = p2 = p3 = p4 = p5 = p6 = p7 = p8 = 0;
	c = 0;	
	error = 0;
	lastError = 0;
	derivative = 0;
	integral = 0;
	kp = 590;
	kd = 0;
	ki = 0;
	
	countAC = 0;
	flagAC = 0;
	flagB =0;
//	flagCheck = 0;
	stateCar = OUTER;
}

Car::Car(unsigned int left, unsigned int right, int tempP, float tempI, int tempD) {
	basicSpeedL = left;
	basicSpeedR = right;
	turn = 0;
	p1 = p2 = p3 = p4 = p5 = p6 = p7 = p8 = 0;
	c = 0;	
	error = 0;
	lastError = 0;
	derivative = 0;
	integral = 0;
	kp = tempP;
	ki = tempI;
	kd = tempD;
	
	countAC = 0;
	countY = 0;
	
	flagAC = 0;
	flagB = 0;
	flagY = 0;
//	flagCheck = 0;
	
	ifBlack_1B = 0;
	ifWhite_1B = 0;
	ifBlack_2B = 0;
	
	ifWhite_1Y = 0;
	ifWhite_2Y = 0;
	ifBlack_1Y = 0;
	ifBlack_2Y = 0;
	
	ifWhite_1DY = 0;
	ifBlack_1DY = 0;
	ifWhite_2DY = 0;
	
	stateCar = OUTER;

}

// detect the track
void Car::track_detect() {
	c = getPath();
	p1 = c & 1? 1 : 0;
	p2 = c & 2? 1 : 0;
	p3 = c & 4? 1 : 0;
	p4 = c & 8? 1 : 0;
	p5 = c & 16? 1 : 0;
	p6 = c & 32? 1 : 0;
	p7 = c & 64? 1 : 0;
	p8 = c & 128? 1: 0;
}

// print the path
void Car::printPath() {
	sprintf(buffer, "\r%d, %d, %d, %d, %d, %d, %d, %d", p1, p2, p3, p4, p5, p6,p7,p8);
	uart3_print(buffer);
}


// calculate outer error
void Car::calculateError_outer() {
	if ((p1 == 0) && (p2 == 0) && (p3 == 1) && (p4 == 1) && (p5 == 1) && (p6 == 0) && (p7 == 0) && (p8 == 0)) {  // p3 p4 p5
		error = 0;
	}
	else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 1) && (p5 == 1) && (p6 == 1) && (p7 == 0) && (p8 == 0)) { // p4 p5 p6
		error = -1;
	}
	else if ((p1 == 0) && (p2 == 1) && (p3 == 1) && (p4 == 1) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p2 p3 p4
		error = 3;
	}
	else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 1) && (p6 == 1) && (p7 == 1) && (p8 == 0)) { // p5 p6 p7
		error = -3;
	}
	else if ((p1 == 1) && (p2 == 1) && (p3 == 1) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1 p2 p3
		error = 6;
	}
	else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 1) && (p7 == 1) && (p8 == 1)) { // p6 p7 p8
		error = -6;
	}
	else if ((p1 == 1) && (p2 == 1) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1 p2
		error = 9;
	}
	else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 1) && (p8 == 1)) { // p7 p8
		error = -9;
}
	else if ((p1 == 1) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 0)) { // p1
		error = 12;
	}
	else if ((p1 == 0) && (p2 == 0) && (p3 == 0) && (p4 == 0) && (p5 == 0) && (p6 == 0) && (p7 == 0) && (p8 == 1)) { // p8
		error = -12;
	}
	else if (((p1 == 1) && (p2 == 1) && (p3 == 1) && (p4 == 1) && (p5 == 1) && (p6 == 1) && (p7 == 1) && (p8 == 0)) ||  
						((p1 == 1) && (p2 == 1) && (p3 == 1) && (p4 == 1) && (p5 == 1) && (p6 == 1) && (p7 == 1) && (p8 == 1))) { // 1111 1111 or 0111 1111
		error = -11;
	
		countAC++;
	 
		if (countAC > targetAC) {                                                                           
			flagAC++;
			if (flagAC == 1) {
				lastError = 0;
				error = -30;
				stateCar = READYFORB;
			}
		}
	}
}


void Car::monitorCounter() {
	
	
	last_leftCounter = getLeft();
	last_rightCounter = getRight();
	
}
	


// control the car according to the error
void Car::tracker() {
	integral = integral + error;
	derivative = error - lastError;
	
	// calculate turn
	turn = (int) ( kp * error + kd * derivative + ki * integral); // PID control
	
	if (flagY == 0) {
		move(true, basicSpeedL + turn, true, basicSpeedR - turn) ;
	}
	else if (flagY == 1) {
		move(true, basicSpeedL - turn, true, basicSpeedR + turn) ;
	}
	lastError = error;
}

// change basic speed
void Car::changeBasicSpeed(unsigned int left, unsigned int right) {
	basicSpeedL = left;
	basicSpeedR = right;
}

// change PID variable
void Car::changePID(int P, int I, int D) {
	kp = P;
	ki = I;
	kd = D;
}

// reset the car when clicking the button
void Car::reset() {
	basicSpeedL = 10000;
	basicSpeedR = 10500;
	turn = 0;
	p1 = p2 = p3 = p4 = p5 = p6 = p7 = p8 = 0;
	c = 0;	
	error = 0;
	lastError = 0;
	derivative = 0;
	integral = 0;
	kp = 590;
	ki = 0;
	kd = 0;
	
	countAC = 0;
	
	flagAC = 0;
	flagB = 0;
//	flagCheck = 0;
	
	stateCar = OUTER;
}

void Car::setState(state tempState) {
	stateCar = tempState;
}
	

void move(bool stateL, unsigned int pwmL, bool stateR, unsigned int pwmR) {
	if (pwmL < 0) {
		pwmL = 0;
	}
	else if (pwmL > 44999) {
		pwmL = 44999;
	}
	else {
		setLeft(stateL);
		pwmLeft(pwmL);
	}
	
	if (pwmR < 0) {
		pwmR = 0;
	}
	else if (pwmR > 44999) {
		pwmR = 44999;
	}
	else {
		setRight(stateR);
		pwmRight(pwmR);
	}
}


		

