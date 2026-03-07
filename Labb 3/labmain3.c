/* main.c
   This file written 2024 by Artur Podobas and Pedro Antunes
   For copyright and licensing, see file COPYING */
/* Below functions are external and found in other files. */
//Assignment 3 - avbrott
extern void print(const char*);
extern void print_dec(unsigned int);
extern void display_string(char*);
extern void time2string(char*,int);
extern void tick(int*);
extern void enable_interrupt(void);
extern int nextprime( int );

volatile int *timer = (volatile int*) 0x04000020;
int prime = 1234567;
int mytime = 0x5957;
char textstring[] = "text, more text, and even more text!";
int timeoutcount = 0;

// uppgift e
static const unsigned char seg_codes[11] = {
    0x40, //0  
    0x79, //1
    0x24, //2
    0x30, //3
    0x19, //4
    0x12, //5
    0x02, //6
    0x78, //7
    0x00, //8
    0x10, //9
    0x7F  //blankt
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

/* Below is the function that will be called when an interrupt is triggered. */
void handle_interrupt(unsigned cause) {
    timer[0] = 0;                            
    timeoutcount++;

    if (timeoutcount == 10) {                
        timeoutcount = 0;
        tick(&mytime);                        
        set_displays(0, mytime & 0xF);        
        set_displays(1, (mytime >> 4) & 0xF);
        set_displays(2, (mytime >> 8) & 0xF);
        set_displays(3, (mytime >> 12) & 0xF);
        set_displays(4, (mytime >> 16) & 0xF);
        set_displays(5, (mytime >> 20) & 0xF);
    }
}                              

void labinit(void){
    timer[2] = 3000000 & 0xFFFF;        
    timer[3] = (3000000 >> 16) & 0xFFFF;
    timer[0] = 0;                       
    timer[1] = 0x7;                    
    enable_interrupt();                
}

// uppgift c
void set_leds(int led_mask){
  volatile int *LED = (volatile int*) 0x04000000; 
  *LED = led_mask & 0x3FF; 
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
int main(void) {
    labinit();
    while (1) {
        print("Prime: ");
        prime = nextprime(prime);
        print_dec(prime);
        print("\n");
    }
}

