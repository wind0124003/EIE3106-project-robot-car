// function prototype

// led.cpp
enum led_state { ON, OFF, NORMAL, FAST, SLOW };
enum led_color { BLUE, GREEN, YELLOW };
void led(void);
void led(led_color, led_state);

//	sr04.cpp
unsigned getDistance();


// tim1.cpp
void wait(unsigned int);   //background jobs are being serviced through this function
unsigned int getADC(void);
char getPath(void);
unsigned int getLeft(void);
unsigned int getRight(void);
void pwmLeft(unsigned int);
void pwmRight(unsigned int);
void setLeft(bool);
void setRight(bool);

//	usart.cpp
bool uart2_print(const char*);
bool uart3_print(const char*);
int uart2_read(void);          //return -1 for none
int uart3_read(void);

// myFunction.cpp
void move(bool, unsigned int, bool, unsigned int);