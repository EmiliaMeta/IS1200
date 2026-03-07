#ifndef IO_H
#define IO_H

#include "objects.h"

// Initiera switchar, knappar, timer, interrupts
void io_init(void);

// Polling-funktion som returnerar vilket val användaren gör via switcharna
// T.ex. SW0 = 1, SW1 = 2, SW2 = 3, SW3 = 4 osv.
int io_get_switch_input(void);

// Interrupt-hanterare (kallas från boot_surprise.S)
void handle_interrupt(unsigned int cause);


int get_sw( void );

int get_btn(void);

void set_leds(int led_mask);

extern const unsigned char seg_codes[11];

void set_displays(int display_number, int value);

#endif
