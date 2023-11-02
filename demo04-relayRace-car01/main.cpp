#include <stdio.h>

void wait(unsigned);   //call poll while wait
bool uart3_print(const char*);
unsigned getADC(void), get_count(void), getDistance(void);
char getPath(void);
int uart2_read(void), uart3_read(void);
bool read_button(void);

enum led_state { ON, OFF, NORMAL, FAST, SLOW };
enum led_color { BLUE, GREEN, YELLOW };
void led(led_color, led_state);

enum wheel_action { STOP, FORWARD, BACKWARD, LEFT, RIGHT };
void wheel(wheel_action, unsigned speed);		// speed<100: speed control; speed>100:direct pwm
bool check_count(unsigned);
void sr04(bool);            // turn on ultrasound: true=ON; false=OFF

//remote
//   G   x
//  D@C H@P
//   F   `

void wheel_track(int);	//1:left, -1:right
void null_count(void);
int main(void){
	while(read_button())wait(0);
	while(!read_button())wait(0);

	wheel_track(-1);
	while(!check_count(420)) wait(0);
	wheel(FORWARD,4);
	while(!check_count(180)) wait(0);
	while(!(getPath() & 0x10)) wait(0);
	
	wheel_track(1);
	while(!check_count(320)) wait(0);
	
	wheel(STOP,1);
	wait(1200);
	// wait for quiting
	sr04(true);
	wait(50);
	while (!(getDistance() > 70 || getDistance() == 0)) {
		wait(50);
	}
	sr04(false);
	
	wheel_track(-1);
	while(!check_count(220)) wait(0);
	
	wheel(LEFT, 3);
	while(!check_count(130)) wait(0);
	wheel(STOP,1);
	
	// wait for car02 coming
	wait(1200);
	sr04(true);
	wait(50);
	while(getDistance() > 70 || getDistance() == 0) wait(50);
	sr04(false);
	wheel(LEFT,3);
	while(!check_count(60)) wait(0);
	while(!(getPath() & 0x10)) wait(0);
	
	
	wheel_track(1);
	while(!check_count(700)) wait(0);
	
	
	
		
	
	wheel(STOP, 1);
	while(1) wait(0);
}

int main1(void) {
	int previous;
	while(1){
		wait(0);
		int k = uart2_read();
		if (k != -1) {
			switch(k) {
				case '@': if (previous & 0x38) wheel(STOP, 0); break;
				case 'x': wheel(FORWARD, 3); break;
				case '`': wheel(BACKWARD, 2); break;
				case 'H': wheel(LEFT, 2); break;
				case 'P': wheel(RIGHT, 2); break;
				case 'D': wheel_track(1); break;
				case 'C': wheel_track(-1); break;
				case 'G': wheel(FORWARD, 25000); wait(400);
				case 'F': wheel(STOP, 0);
				default:;
			}
			previous = k;
		}
	}
}

int maino(void) {
  led(GREEN, NORMAL);
  while (1) {
    wheel(STOP, 0);
    while (!check_count(600)) {
      wait(0);
      int k = uart2_read();
      if (k == -1) k = uart3_read();
      if (k != -1) switch(k) {
				case 'x': case 'f': wheel(FORWARD, 1); break;
				case '`': case 'b': wheel(BACKWARD, 1); break;
				case 'H': case 'l': wheel(LEFT, 1); break;
				case 'P': case 'r': wheel(RIGHT, 1); break;
      default: wheel(STOP, 0);
      }
    }
  }
}

static void display(unsigned t) {
  static char buffer[99];
  if (t & 31) return;
  sprintf(buffer, "\rpath=[xxxx xxxx] dist=%d count=%d batt=%d    ", getDistance(), get_count(), (getADC()*89)>>12);
  char c = getPath();
  buffer[7] = c & 1 ? '0' : '.';
  buffer[8] = c & 2 ? '1' : '.';
  buffer[9] = c & 4 ? '2' : '.';
  buffer[10] = c & 8 ? '3' : '.';
  buffer[12] = c & 16 ? '4' : '.';
  buffer[13] = c & 32 ? '5' : '.';
  buffer[14] = c & 64 ? '6' : '.';
  buffer[15] = c & 128 ? '7' : '.';
  uart3_print(buffer);
}

void led(void), button(void), update_pwm();
void poll(unsigned tick) { //tick increments every 2.5 ms
  static unsigned t;
  /* do something here */
  if (t == tick) return;
  t = tick;
  update_pwm();
  led(); button();
  display(t);
}

void on_button(bool b) {
//	wheel(b ? FORWARD : STOP, 25000);		// speed value is ignored in STOP
	sr04(!b);
}

