/* main.c
   This file written 2024 by Artur Podobas and Pedro Antunes
   For copyright and licensing, see file COPYING */
/* Below functions are external and found in other files. */
//Assignment 2 - timer (replacing old delay)
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void delay(int);
extern int nextprime( int );

int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) 
{}

volatile int *timer = (volatile int*) 0x04000020;

//We want the timer to overflow every 100 ms, 3,000,000 cycles, which equals 100 ms at a 30 MHz clock
//Timer is split into two 16-bit registers, timer hardware exposes each of its registers as 32-bit (4bytes) wide memory locations.

void labinit(void){
    timer[2] = 3000000 & 0xFFFF;        // period low (maskar 16 lägsta bitar av timeout-värdet), (bits 0–15) defining time
    timer[3] = (3000000 >> 16) & 0xFFFF;// period high (övre 16 bitar av timeout-värdet), (bits 16-31)
    timer[0] = 0;                       // statusregister: clears the timeout flag (reset), so we start with a clean state, 0-->1
    timer[1] = 0x7;                     // controllregister: 0x1 = ITO (allow interupts), 0x2 = CONT (reload automatically after reachin 0). 0x4 = START (start timer). 
                                        // 0x7 = 111b → the timer starts running, keeps repeating, and sets the TO flag every 100 ms (every 10 loop)
}

// uppgift c
void set_leds(int led_mask){
  volatile int *LED = (volatile int*) 0x04000000; 
  *LED = led_mask & 0x3FF; 
}
// uppgift e
static const unsigned char seg_codes[11] = {
    0x40, // 0  
    0x79, // 1
    0x24, // 2
    0x30, // 3
    0x19, // 4
    0x12, // 5
    0x02, // 6
    0x78, // 7
    0x00, // 8
    0x10, // 9
    0x7F  // blank
};

void set_displays(int display_number, int value){
    volatile unsigned char *display = 
        (volatile unsigned char *)(0x04000050 + display_number * 0x10);

    if (value >= 0 && value <= 9) {
        *display = seg_codes[value];
    } else {
        *display = seg_codes[10]; 
    }
}

//uppgift f
int get_sw( void ){
  volatile int *toggle = (volatile int*) 0x04000010; 
  return *toggle & 0x3FF; 
}

//uppgift g
int get_btn(void){
  volatile int *button2 = (volatile int*) 0x040000d0; 
  return *button2 & 0x1; 
}

/* Your code goes into main as well as any needed functions. */
//global räknare, Eftersom timern ger 100 ms intervall, räknar vi 10 timeouts = 1 sekund
int timeoutcount = 0;

int main(void) {
    labinit();
    int seconds = 0, minutes = 0, hours = 0;
    int sw;

    int ledcount = 0;
    set_leds(ledcount); 
    //ersättit delay funktionen
    while (ledcount < 16) {
        if (timer[0] & 0x1) {                   //timer[0] is the statusregister, 0 bit is tO, when tO = 1 means counted down to 0 (signals one period has passed (100ms)), ox1 kollar om tO är 1 eller 0
            timer[0] = 0;                       //reset flag, The flag represents an interrupt event, if we didnt the flag would give false interupts
            timeoutcount++;                     //every 100ms increment the counter
            if (timeoutcount == 10) {           //when 10 timeout flag is done we know 1 second has passed 10*100 =1 s
                timeoutcount = 0;               //reset the timercounter so we can count again
                ledcount++;                     //increase ledcount
                set_leds(ledcount);             //to turn on/off the led
            }
        }
    }

    timeoutcount = 0; // reset för main loop 
    
    // Enter a forever loop
    while (1) {
        if (timer[0] & 0x1) {  //if 100 ms has passed, 
            timer[0] = 0;      //clear the flag, acknowledge the event so the timer is ready to flag again on the next 100 milliseconds
            timeoutcount++;    //increase timeout flag 

            if (timeoutcount == 10) { //last q,
                timeoutcount = 0;
                //real interrupts for the button input, so that every press immediately triggers an interrupt handler regardless of when it occurs.
                //updaterar tiden på displayen
                if (get_btn()) {
                    sw = get_sw();
                    int timeField = (sw >> 8) & 0x3;
                    int value = sw & 0x3F;

                    if (timeField == 1) seconds = value;
                    else if (timeField == 2) minutes = value;
                    else if (timeField == 3) hours = value;

                    if (sw & 0x80) break;  
                } else {
                    // räkna fram tiden
                    seconds++;
                    if (seconds == 60) { seconds = 0; minutes++; }
                    if (minutes == 60) { minutes = 0; hours++; }
                    if (hours == 24)   { hours = 0; }
                }

                set_displays(0, seconds % 10);
                set_displays(1, seconds / 10);
                set_displays(2, minutes % 10);
                set_displays(3, minutes / 10);
                set_displays(4, hours % 10);
                set_displays(5, hours / 10);

                time2string(textstring, mytime);
                display_string(textstring);
                tick(&mytime);
            }
        }
    }
}
