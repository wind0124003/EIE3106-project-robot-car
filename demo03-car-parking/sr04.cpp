#include <stm32f10x.h>
#include "init.h"

#define PWM 0x6870
#define CAPTURE 0x6801
#define IDLE 0x6803

static unsigned distance;
unsigned getDistance(void) { return distance/1050; } // return distance /1050

void sr04(unsigned phase) {
  if (phase == 7) {
    if (TIM1->CCMR2 == CAPTURE) distance = 0;
    TIM1->CCER &= ~TIM_CCER_CC3E;     //disable capture 
    TIM1->CCR3 = 44820;               //10us
    TIM1->CCMR2 = PWM;
    TIM1->CCER |= TIM_CCER_CC3NE;     //enable o/p pin
  } else {
    if (!phase) {
      TIM1->CCER &= ~TIM_CCER_CC3NE;  //disable o/p pin
      TIM1->CCMR2 = CAPTURE;
      TIM1->CCER |= TIM_CCER_CC3E;    //enable capture 
      TIM1->SR &= ~TIM_SR_CC3IF;
    } else {
      if (TIM1->SR & TIM_SR_CC3IF) {
        distance = phase * 45000 + TIM1->CCR3;
        if (TIM1->CCR3 > TIM1->CNT) distance -= 45000;
        TIM1->SR &= ~TIM_SR_CC3IF;
        TIM1->CCER &= ~TIM_CCER_CC3E; //disable capture
        TIM1->CCMR2 = IDLE;
      }
    }
  }
}
