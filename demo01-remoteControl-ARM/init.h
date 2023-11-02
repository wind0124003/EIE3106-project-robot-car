#include "stdbool.h"
#include "stdint.h"

#define LEFT_WHEEL_FORWARD_RCC_GPIO						RCC_APB2Periph_GPIOA
#define LEFT_WHEEL_FORWARD_GPIO								GPIOA
#define LEFT_WHEEL_FORWARD_PIN								GPIO_Pin_8

#define LEFT_WHEEL_BACKWARD_RCC_GPIO					RCC_APB2Periph_GPIOB
#define LEFT_WHEEL_BACKWARD_GPIO							GPIOB
#define LEFT_WHEEL_BACKWARD_PIN								GPIO_Pin_13

#define RIGHT_WHEEL_FORWARD_RCC_GPIO					RCC_APB2Periph_GPIOB
#define RIGHT_WHEEL_FORWARD_GPIO							GPIOB
#define RIGHT_WHEEEL_FORWARD_PIN							GPIO_Pin_14

#define RIGHT_WHEEL_BACKWARD_RCC_GPIO					RCC_APB2Periph_GPIOA
#define RIGHT_WHEEL_BACKWARD_GPIO							GPIOA
#define RIGHT_WHEEL_BACKWARD_PIN							GPIO_Pin_9

#define LEFT_WHEEL_COUNTER_RCC_GPIO						RCC_APB2Periph_GPIOB
#define LEFT_WHEEL_COUNTER_GPIO								GPIOB
#define LEFT_WHEEL_COUNTER_PIN								GPIO_Pin_7

#define RIGHT_WHEEL_COUNTER_RCC_GPIO					RCC_APB2Periph_GPIOA
#define RIGHT_WHEEL_COUNTER_GPIO							GPIOA
#define RIGHT_WHEEL_COUNTER_PIN								GPIO_Pin_1

#define LED_YELLOW_RCC_GPIO 			RCC_APB2Periph_GPIOA
#define LED_GREEN_BLUE_RCC_GPIO		RCC_APB2Periph_GPIOB

#define LED_YELLOW_GPIO						GPIOA
#define LED_BLUE_GPIO							GPIOB
#define LED_GREEN_GPIO						GPIOB

#define LED_YELLOW_PIN						GPIO_Pin_0
#define LED_BLUE_PIN							GPIO_Pin_12
#define LED_GREEN_PIN							GPIO_Pin_6

#define BUTTON_RCC_GPIO 					RCC_APB2Periph_GPIOA
#define BUTTON_GPIO								GPIOA
#define BUTTON_PIN								GPIO_Pin_12
#define BUTTON_GPIO_PORTSOURCE		GPIO_PortSourceGPIOA
#define BUTTON_GPIO_PINSOURCE			GPIO_PinSource12

#define USART3_TX_RX_RCC_GPIO				RCC_APB2Periph_GPIOB
#define USART3_TX_GPIO						GPIOB
#define USART3_TX_PIN							GPIO_Pin_10

#define USART3_RX_GPIO						GPIOB
#define USART3_RX_PIN							GPIO_Pin_11

#define SR04_ECHO_RCC_GPIO				RCC_APB2Periph_GPIOA
#define SR04_ECHO_GPIO						GPIOA
#define SR04_ECHO_PIN							GPIO_Pin_10

#define SR04_TRIG_RCC_GPIO				RCC_APB2Periph_GPIOB
#define SR04_TRIG_GPIO						GPIOB
#define SR04_TRIG_PIN							GPIO_Pin_15

#define USART2_TX_RX_RCC_GPIO			RCC_APB2Periph_GPIOA
#define USART2_TX_GPIO						GPIOA
#define USART2_TX_PIN							GPIO_Pin_2

#define USART2_RX_GPIO						GPIOA
#define USART2_RX_PIN							GPIO_Pin_3	

#define SPI_SCK_MISO_MOSI_RCC_GPIO					RCC_APB2Periph_GPIOA
#define SPI_SCK_GPIO							GPIOA
#define SPI_SCK_PIN								GPIO_Pin_5

#define SPI_MISO_GPIO							GPIOA
#define SPI_MISO_PIN							GPIO_Pin_6

#define SPI_MOSI_GPIO							GPIOA
#define SPI_MOSI_PIN							GPIO_Pin_7

// variable 
enum led_state{ON, OFF, BLINK};
enum led_color{BLUE, GREEN, YELLOW};

// function prototype
void wheelInit(void);
void wheelCounterInit(void);
void uart3Init(void);
void SR04Init(void);
void uart2Init(void);
void ledInit(void);
void buttonInit(void);
void spiInit(void);		// TODO


// Motor
void leftForward(unsigned short);
void leftBackward(unsigned short);
void rightForward(unsigned short);
void rightBackward(unsigned short);
void move(bool, unsigned short, bool, unsigned short);		// move the car
int getLeftWheelCounter(void);
int getRightWheelCounter(void);

// GPIO
void setLed(enum led_color ,enum led_state); // turn on led by sending led color and led state
// cannot blink all led, only one led can blink
void setYellowLedBlink(void);
void setBlueLedBlink(void);
void setGreenLedBlink(void);

// USART
void USART3Send(char *, unsigned long);			// send a string through USART3
unsigned char USART3Read(void);
unsigned char USART2Read(void);
void USART2Send(unsigned char);
char getKeyboardValue(void);

// Timer
void delay_us(unsigned short);		// using TIM3 to generate a time delay(period in us)
void delay_stop(void);
void sysTickInit(void);
void delay_ms(uint32_t);

// SR04
void triggerSR04(void); // trigger SR04
int getDistance(void);	// get measured distance

// SPI
void readFloor(void);
char getPath(void);