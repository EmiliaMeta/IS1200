/* main.c
   Assignment 3 - avbrott + surprise + debug
*/

extern void print(const char*);
extern void print_dec(unsigned int);
extern void tick(int*);
extern void enable_interrupt(void);
extern int nextprime(int);

#define LED_BASE        0x04000000
#define SW_BASE         0x04000010
#define TIMER_BASE      0x04000020

volatile int *led_data     = (volatile int*) LED_BASE;         
volatile int *sw_data      = (volatile int*) (SW_BASE + 0x00); 
volatile int *sw_ienable   = (volatile int*) (SW_BASE + 0x04); 
volatile int *sw_ipending  = (volatile int*) (SW_BASE + 0x08); 
volatile int *timer        = (volatile int*) TIMER_BASE;

int prime = 1234567;
int mytime = 0x5957;   // starttid
int timeoutcount = 0;
int old_sw3_state = 0; // för kantdetektering

static const unsigned char seg_codes[11] = {
    0x40,0x79,0x24,0x30,0x19,
    0x12,0x02,0x78,0x00,0x10,0x7F
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

static void update_displays(void){
    set_displays(0,  mytime        & 0xF);        
    set_displays(1, (mytime >> 4)  & 0xF);
    set_displays(2, (mytime >> 8)  & 0xF);
    set_displays(3, (mytime >> 12) & 0xF);
    set_displays(4, (mytime >> 16) & 0xF);
    set_displays(5, (mytime >> 20) & 0xF);
}

/* Interrupt handler */
void handle_interrupt(unsigned int cause) {
    if (timer[0] & 0x1) {
        timer[0] = 0;    
        timeoutcount++;

        if (timeoutcount == 10) {
            timeoutcount = 0;
            tick(&mytime);          
            update_displays();      
        }
    }

    // sw3
    if (*sw_ipending & (1u << 3)) {
        *sw_ipending = (1u << 3);  // ack direkt

        int sw3_state = (*sw_data & (1u << 3)) ? 1 : 0;

        if (sw3_state && !old_sw3_state) {  
            // knapptryckning (rising edge)
            mytime += 2;
            update_displays();
        }

        old_sw3_state = sw3_state; // uppdatera senaste läget
    }
}

void labinit(void){
    // Timer initiering för 25 MHz CPU
    // Exempel: 25e6 / 2.5e6 = 10 => avbrott var 0.1 s
    int reload = 2500000;    
    timer[2] = reload & 0xFFFF;        
    timer[3] = (reload >> 16) & 0xFFFF;
    timer[0] = 0;                       
    timer[1] = 0x7;   // start + autoreload + irq enable

    // SW3 initiering
    int dummy = *sw_ipending;   // läs först
    *sw_ipending = (1u << 3);   // rensa SW3 pending
    *sw_ienable  = (1u << 3);   // tillåt SW3

    enable_interrupt(); 
    update_displays();  // visa initial tid direkt
}

int main(void) {
    labinit();
    while (1) {
        print("Prime: ");
        prime = nextprime(prime);
        print_dec(prime);
        print("\n");
    }
}
