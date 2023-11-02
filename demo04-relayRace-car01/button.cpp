#include <stm32f10x.h>

static int key = 1<<12;

bool read_button(void) { return key; }
void __attribute__((weak)) on_button(bool) {}

void button(void) {
  static int count;
  if (++count < 8) return;       //count 20 ms
  count = 0;
  int k = GPIOA->IDR & (1<<12);
  if (key == k) return;
  on_button((key = k));            //let user decide what to do
}

