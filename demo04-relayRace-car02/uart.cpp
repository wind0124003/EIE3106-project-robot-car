#include <stm32f10x.h>

class Uart{
public:
  Uart(USART_TypeDef *b) { base = b; ptr = 0; }
  void isr(void) {
    if (base->SR & USART_SR_TXE) {
      if (ptr) {
        if (*ptr) base->DR = *ptr++;
        else ptr = 0;
      } else base->CR1 &= ~USART_CR1_TXEIE;
    }
  }
  bool print(const char* p) {
    if (ptr) return false;
    ptr = p;
    base->CR1 |= USART_CR1_TXEIE;
    return true;
  }
  int read(void) {
    return base->SR & USART_SR_RXNE ? base->DR : -1;
  }
private:
  const char *ptr;
  USART_TypeDef *base;
} static uart2(USART2), uart3(USART3);

bool uart2_print(const char* p) { return uart2.print(p); }
bool uart3_print(const char* p) { return uart3.print(p); }
int uart2_read(void) { return uart2.read(); }
int uart3_read(void) { return uart3.read(); }

extern "C" void USART2_IRQHandler(void) { uart2.isr(); }
extern "C" void USART3_IRQHandler(void) { uart3.isr(); }
