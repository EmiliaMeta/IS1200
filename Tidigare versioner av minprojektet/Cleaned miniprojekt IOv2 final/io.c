/**
 * io.c

 * Inkluderar:
 *  - Avläsning av switchar och knapp
 *  - LED-utgång
 *  - 7-segmentdisplay
 *  - Interrupt för SW9 (öppnar meny)
 */

#include "io.h"
#include "menu.h"
#include "textstring.h"

// Minnesmappade adresser för DTEK-V-hårdvara
#define SW_BASE      0x04000010
#define LED_BASE     0x04000000
#define BTN_BASE     0x040000D0
#define DISP_BASE    0x04000050
#define TIMER_BASE   0x04000020

// Registerpekare för switch-interrupts
volatile int *sw_enable  = (volatile int*)(SW_BASE + 0x4);
volatile int *sw_pending = (volatile int*)(SW_BASE + 0x8);

// Extern global spelare, används för att öppna menyn via interrupt
extern Player global_player;

// Input

// Läser av alla switchar (SW0–SW9)
int get_sw(void) {
    volatile int *sw = (volatile int*)SW_BASE;
    return *sw & 0x3FF;    // Maskera till 10 switchar
}

// Läser av knapp BTN1 (retur 1 = nedtryckt)
int get_btn(void) {
    volatile int *btn = (volatile int*)BTN_BASE;
    return *btn & 0x1;
}

// Output

// Tänder LED:ar enligt mask
void set_leds(int led_mask) {
    volatile int *led = (volatile int*)LED_BASE;
    *led = led_mask & 0x3FF;  // Begränsa till 10 LED
}

// Initiering

// Initierar switch-interrupts (SW9)
void io_init(void) {
    int dummy = *sw_pending;      // Läs bort gamla pending-flaggor
    *sw_pending = (1u << 9);      // Rensa SW9 pending
    *sw_enable  = (1u << 9);      // Aktivera interrupt för SW9
}

// Polling

// Väntar på att användaren trycker på en aktiv switch (SW1–SW4)
int io_switch(void) {
    int val = 0;

    while (1) {
        val = get_sw();

        if (val & 0x2) {  // SW1
            delay(200);              // kort paus för debounce (200 ms)
            while (get_sw() & 0x2);  // vänta tills släppt
            return 1;
        }
        if (val & 0x4) {  // SW2
            delay(200);
            while (get_sw() & 0x4);
            return 2;
        }
        if (val & 0x8) {  // SW3
            delay(200);
            while (get_sw() & 0x8);
            return 3;
        }
        if (val & 0x10) { // SW4
            delay(200);
            while (get_sw() & 0x10);
            return 4;
        }
        if (val & 0x20) { // SW5
            delay(200);
            while (get_sw() & 0x20);
            return 5;
        }
    }
}

// Interrupt

// Hanterar avbrott (SW9 används för att öppna meny)
void handle_interrupt(unsigned int cause) {
    if (*sw_pending & (1u << 9)) {
        *sw_pending = (1u << 9);      // Rensa pending-flagga
        game_menu(&global_player);    // Öppna menyn
    }
}

// 7-segm display

// Segmentkoder för 0–9 samt blank (index 10)
const unsigned char seg_codes[11] = {
    0x40, 0x79, 0x24, 0x30, 0x19, 0x12,
    0x02, 0x78, 0x00, 0x10, 0x7F
};

// Sätter ett värde på given display (0–9, annars blank)
void set_displays(int display_number, int value) {
    volatile unsigned char *display = 
        (volatile unsigned char *)(DISP_BASE + display_number * 0x10);

    if (value >= 0 && value <= 9)
        *display = seg_codes[value];
    else
        *display = seg_codes[10];
}

// HP display

// Visar HP för spelare (vänster) och fiende (höger)
void hp_display(Creature *p, Creature *e) {
    if (p) {
        set_displays(5, p->health / 10);
        set_displays(4, p->health % 10);
    }

    // Separator
    set_displays(3, -1);
    set_displays(2, -1);

    if (e && e->is_alive) {
        set_displays(1, e->health / 10);
        set_displays(0, e->health % 10);
    } else {
        set_displays(1, -1);
        set_displays(0, -1);
    }
}

// Led countdown

// Enkel LED-nedräkning inför strid
void led_countdown_fight(void) {
    for (int i = 4; i > 0; i--) {
        set_leds(i);
        print("Countdown: ");
        print_dec(i);
        print("\n");
        delay(1000);
    }
    set_leds(0);
    print_and_wait("Let the battle begin!", NULL);
}
