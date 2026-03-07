//i/0 - system on DTEK-V board

int get_sw( void ){
  volatile int *toggle = (volatile int*) 0x04000010; //Switches are memory-mapped at 0x04000010
  return *toggle & 0x3FF; //We mask with 0x3FF to only keep the lowest 10 bits (SW0–SW9), Each switch corresponds to one bit in the memory-mapped register at 0x04000010
}

int get_btn(void){
  volatile int *button2 = (volatile int*) 0x040000d0;  //The second push-button is mapped to 0x040000d0.
  return *button2 & 0x1;  //Returns 1 if pressed, otherwise 0
}

void set_leds(int led_mask){
  volatile int *LED = (volatile int*) 0x04000000; //The LEDs are memory-mapped at address 0x04000000,
  *LED = led_mask & 0x3FF; //Only the 10 least significant bits are used (since 10 LEDs on the board).
}

// uppgift e -  seg_codes[] holds the bit patterns for digits 0–9 and a blank code
const unsigned char seg_codes[11] = {
    0x40, /*0*/ 0x79, /*1*/ 0x24, /*2*/ 0x30, /*3*/ 0x19, /*4*/ 0x12, /*5*/ 
    0x02, /*6*/ 0x78, /*7*/ 0x00, /*8*/ 0x10, /*9*/ 0x7F //blank
};

void set_displays(int display_number, int value){
  //första displayen (0) finns på addressen 0x04000050
    volatile unsigned char *display = 
        (volatile unsigned char *)(0x04000050 + display_number * 0x10);
    //if statement is checking whether the variable value is a digit between 0 and 9.
    if (value >= 0 && value <= 9) { 
        *display = seg_codes[value];
    } else {
        *display = seg_codes[10]; 
    }
}
