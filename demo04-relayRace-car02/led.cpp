#include <stm32f10x.h>
enum led_state { ON, OFF, NORMAL, FAST, SLOW };
enum led_color { BLUE, GREEN, YELLOW };

class Led {
public:
  Led(GPIO_TypeDef*, int);
  void set(led_state);
  void tictac(void);
private:
  GPIO_TypeDef *base;
  int pin;
  led_state state;
  unsigned int period, count;
} static blue(GPIOB, 12), green(GPIOB, 6), yellow(GPIOA, 0);

Led::Led(GPIO_TypeDef *b, int p) {
  base = b;
  pin = 1 << p;
  set(OFF);
}

void Led::set(led_state s) {
  state = s;
  if (s == ON) {
    if (pin == 1<<12) base->BRR = pin; else base->BSRR = pin;
  }
  if (s == OFF) {
    if (pin == 1<<12) base->BSRR = pin; else base->BRR = pin;
  }
  if (s == NORMAL) period = 400;
  if (s == FAST) period = 200;
  if (s == SLOW) period = 800;
}

void Led::tictac(void) {
  if (state == ON) return;
  if (state == OFF) return;
  if (++count < period) return;
  count = 0;
  if (base->ODR & pin) base->BRR = pin; else base->BSRR = pin;
}

void led(void) {
  blue.tictac();
  green.tictac();
  yellow.tictac();
}

void led(led_color c, led_state s) {
  if (c == BLUE) blue.set(s);
  if (c == GREEN) green.set(s);
  if (c == YELLOW) yellow.set(s);
}

