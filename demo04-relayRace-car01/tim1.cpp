#include <stm32f10x.h>

class Timer {
public:
  Timer(void);
  unsigned int tick, spi_cnt, left, right;
  char path;
private:
  unsigned int last;
} static timer;

extern "C" void TIM1_UP_IRQHandler(void) {
  timer.left = TIM2->CNT;
  timer.right = TIM4->CNT;
  TIM1->SR &= ~TIM_SR_UIF;
  timer.spi_cnt = 4;
  SPI1->CR2 |= SPI_CR2_TXEIE;
  ADC1->CR2 = ADC_CR2_ADON;	//ADC start convert
}

void sr04(unsigned phase);
extern "C" void SPI1_IRQHandler(void) {
  char c;
  if (SPI1->CR2 & SPI_CR2_TXEIE)
    if (SPI1->SR & SPI_SR_TXE) {
      if (--timer.spi_cnt) SPI1->DR = 0xff;
      else { SPI1->DR = 0; SPI1->CR2 &= ~SPI_CR2_TXEIE; }
    }
  if (SPI1->SR & SPI_SR_RXNE) {
    c = SPI1->DR;
    if (SPI1->SR & SPI_SR_TXE) {
      timer.path = c;
      ++timer.tick;             //whenever tick updated, path is also updated.
      sr04(timer.tick & 7);
    }
  }
}

Timer::Timer(void) {
  RCC->APB2ENR = RCC_APB2ENR_SPI1EN | RCC_APB2ENR_TIM1EN |
    RCC_APB2ENR_ADC1EN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPAEN;
  RCC->APB1ENR = RCC_APB1ENR_USART3EN | RCC_APB1ENR_USART2EN |
    RCC_APB1ENR_TIM4EN | RCC_APB1ENR_TIM2EN;
  GPIOA->CRL = 0xa4a44a42;
  GPIOA->CRH = 0x4444a4aa;
  GPIOB->CRL = 0x42444440;
  GPIOB->CRH = 0xaaa24a44;
  TIM1->DIER = TIM_DIER_UIE;
  TIM1->PSC = 3;                 //18 MHz = 72/(3+1)
  TIM1->ARR = 44999;             //2.5 ms
  TIM1->CCMR1 = 0x6868;          //ch1 and ch2: PWM preload
  TIM1->CCER = TIM_CCER_CC3P;    //falling edge
  TIM1->BDTR = TIM_BDTR_MOE;     //master output enable
  USART2->BRR = 3750;                 //9600
  USART3->BRR = 313;                  //115200
  USART2->CR1 = USART3->CR1 = USART_CR1_TE | USART_CR1_RE;
  ADC1->SMPR2 = 0x07000000;           //SMP8=111 (239.5 cycles)
  ADC1->SQR3 = 8;                     //ch8
  SPI1->CR2 = SPI_CR2_RXNEIE;
  SPI1->CR1 = SPI_CR1_CPOL | SPI_CR1_MSTR |
    SPI_CR1_BR | SPI_CR1_SSM | SPI_CR1_SSI;
  TIM2->SMCR = TIM4->SMCR = 0x67;       //TI2FP2
  TIM2->CCMR1 = TIM2->CCMR1 = 0x100;    //CC2 as input, IC2 mapped on TI2
  NVIC_EnableIRQ(TIM1_UP_IRQn);
  NVIC_EnableIRQ(USART2_IRQn);
  NVIC_EnableIRQ(USART3_IRQn);
  NVIC_EnableIRQ(SPI1_IRQn);
  USART2->CR1 |= USART_CR1_UE;
  USART3->CR1 |= USART_CR1_UE;
  SPI1->CR1 |= SPI_CR1_SPE;
  TIM2->CR1 = TIM4->CR1 = TIM_CR1_CEN;  //turn on timers
  TIM1->CR1 = TIM_CR1_CEN;    		//timer on
}

void poll(unsigned);
void wait(unsigned int i) {
  if (!i) return poll(timer.tick);
  i += timer.tick;
  while (i != timer.tick) poll(timer.tick);
}

void pwmLeft(unsigned i) {    //input must be lest than 45000
  TIM1->CCR1 = i;
}

void pwmRight(unsigned i) {   //input must be lest than 45000
  TIM1->CCR2 = i;
}

void setLeft(bool b) {             //true-forward
  TIM1->CCER &= ~(TIM_CCER_CC1NE | TIM_CCER_CC1E);
  if (b) TIM1->CCER |= TIM_CCER_CC1E;
  else TIM1->CCER |= TIM_CCER_CC1NE;
}

void setRight(bool b) {            //true-forward
  TIM1->CCER &= ~(TIM_CCER_CC2NE | TIM_CCER_CC2E);
  if (b) TIM1->CCER |= TIM_CCER_CC2NE;
  else TIM1->CCER |= TIM_CCER_CC2E;
}

unsigned int getADC(void) { return ADC1->DR; }
char getPath(void) { return timer.path; }
unsigned int getLeft(void) { return timer.left; }
unsigned int getRight(void) { return timer.right; }

