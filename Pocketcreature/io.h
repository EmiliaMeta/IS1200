#ifndef IO_H
#define IO_H

#ifndef NULL
#define NULL ((void*)0)
#endif

#include "objects.h"

/**
 * io.h

 * Hanterar:
 *  - Switch-inmatning
 *  - LED-indikering
 *  - 7-segmentdisplay
 *  - written by Aleena Amir
 */

// Läser av switchar (SW0–SW9)
int get_sw(void);

// Sätter LED-status (10 LED:ar)
void set_leds(int led_mask);

// Pollar switchar tills en aktiv hittas (1–4)
int io_switch(void);

// Sätter siffror på 7-segmentdisplay (0–9, -1 = blank)
void set_displays(int display_number, int value);

// Visar HP-nivå för spelare och battle
void hp_display(Creature *p, Creature *e);

// LED-baserad nedräkning inför battle
void led_countdown_fight(void);

// Segmentkoder (0–9 + blank)
extern const unsigned char seg_codes[11];

#endif
