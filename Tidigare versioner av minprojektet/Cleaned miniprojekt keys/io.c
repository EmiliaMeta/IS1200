/**
 * io.c
 * -------------------------
 * Hanterar hårdvaru-I/O för DTEK-V-kortet,
 * men kan även köras i PC-simuleringsläge för testning.
 *
 * I PC_TEST-läget används tangentbordsinput istället för
 * fysiska switchar, knappar och LED.
 *
 * Kodspråk: engelska
 * Kommentarer: svenska
 */

#include "io.h"
#include "menu.h"
#include "textstring.h"

#ifdef PC_TEST
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#endif

// Minnesmappade adresser för DTEK-V-hårdvara
#define SW_BASE      0x04000010
#define LED_BASE     0x04000000
#define BTN_BASE     0x040000D0
#define DISP_BASE    0x04000050

#ifndef PC_TEST
// Registerpekare för interrupts (endast DTEK-V)
volatile int *sw_enable  = (volatile int*)(SW_BASE + 0x4);
volatile int *sw_pending = (volatile int*)(SW_BASE + 0x8);
#endif

// Global spelare (för meny-interrupt)
extern Player global_player;

/* ========================= INPUT ========================= */

// Läser av switchar (SW0–SW9)
int get_sw(void) {
#ifdef PC_TEST
    int val = 0;
    printf("[INPUT] Enter switch number (1–4, 0 to cancel): ");
    if (scanf("%d", &val) != 1) {
        while (getchar() != '\n'); // töm buffert
        return 0;
    }
    if (val <= 0 || val > 9) return 0;
    return (1 << val);
#else
    volatile int *sw = (volatile int*)SW_BASE;
    return *sw & 0x3FF;
#endif
}

// Läser knapp (BTN1)
int get_btn(void) {
#ifdef PC_TEST
    char c;
    printf("[INPUT] Press 'a' to simulate BTN1: ");
    if (scanf(" %c", &c) != 1) return 0;
    return (c == 'a' || c == 'A');
#else
    volatile int *btn = (volatile int*)BTN_BASE;
    return *btn & 0x1;
#endif
}

/* ========================= OUTPUT ========================= */

// Sätter LED-mask
void set_leds(int led_mask) {
#ifdef PC_TEST
    printf("[LEDs] Mask = %d\n", led_mask);
#else
    volatile int *led = (volatile int*)LED_BASE;
    *led = led_mask & 0x3FF;
#endif
}

/* ========================= INITIALISERING ========================= */

// Initierar switchar och interrupts
void io_init(void) {
#ifndef PC_TEST
    int dummy = *sw_pending;
    *sw_pending = (1u << 9);
    *sw_enable  = (1u << 9);
#else
    printf("[IO] Running in PC simulation mode (no real interrupts).\n");
#endif
}

/* ========================= POLLING ========================= */

// Pollar efter switch-val (SW1–SW4)
int io_switch(void) {
#ifdef PC_TEST
    int val = 0;
    printf("[MENU] Choose an action (1–4): ");
    if (scanf("%d", &val) != 1) {
        while (getchar() != '\n');
        return 0;
    }
    if (val < 1 || val > 4) return 0;
    return val;
#else
    while (1) {
        int val = get_sw();
        if (val & 0x2) return 1;
        if (val & 0x4) return 2;
        if (val & 0x8) return 3;
        if (val & 0x10) return 4;
    }
#endif
}

/* ========================= INTERRUPT ========================= */

// Hanterar SW9-interrupt (öppnar meny)
void handle_interrupt(unsigned int cause) {
#ifndef PC_TEST
    if (*sw_pending & (1u << 9)) {
        *sw_pending = (1u << 9);
        game_menu(&global_player);
    }
#else
    printf("[INTERRUPT] Simulated SW9 interrupt -> opening menu\n");
    game_menu(&global_player);
#endif
}

/* ========================= 7-SEGMENT DISPLAY ========================= */

#ifndef PC_TEST
// Segmentkoder för 0–9 samt blank
const unsigned char seg_codes[11] = {
    0x40, 0x79, 0x24, 0x30, 0x19, 0x12,
    0x02, 0x78, 0x00, 0x10, 0x7F
};
#endif

// Sätter värde på display (0–9, annars blank)
void set_displays(int display_number, int value) {
#ifdef PC_TEST
    if (value >= 0 && value <= 9)
        printf("[DISPLAY %d] %d\n", display_number, value);
    else
        printf("[DISPLAY %d] blank\n", display_number);
#else
    volatile unsigned char *display =
        (volatile unsigned char *)(DISP_BASE + display_number * 0x10);
    if (value >= 0 && value <= 9)
        *display = seg_codes[value];
    else
        *display = seg_codes[10];
#endif
}

/* ========================= HP DISPLAY ========================= */

void hp_display(Creature *p, Creature *e) {
#ifdef PC_TEST
    int player_hp = p ? p->health : 0;
    int enemy_hp = (e && e->is_alive) ? e->health : 0;
    printf("[HP] Player: %d | Enemy: %d\n", player_hp, enemy_hp);
#else
    if (p) {
        set_displays(5, p->health / 10);
        set_displays(4, p->health % 10);
    }

    set_displays(3, -1);
    set_displays(2, -1);

    if (e && e->is_alive) {
        set_displays(1, e->health / 10);
        set_displays(0, e->health % 10);
    } else {
        set_displays(1, -1);
        set_displays(0, -1);
    }
#endif
}

/* ========================= LED COUNTDOWN ========================= */

void led_countdown_fight(void) {
#ifdef PC_TEST
    printf("[COUNTDOWN] Starting battle countdown...\n");
    for (int i = 4; i > 0; i--) {
        printf("[COUNTDOWN] %d...\n", i);
        delay(300);
    }
    printf("Let the battle begin!\n");
#else
    for (int i = 4; i > 0; i--) {
        set_leds(i);
        print("Countdown: ");
        print_dec(i);
        print("\n");
        delay(1000);
    }
    set_leds(0);
    print_and_wait("Let the battle begin!", NULL);
#endif
}

/* ========================= DELAY (för PC-test) ========================= */

#ifdef PC_TEST
void delay(int ms) {
    // Enkel, CPU-vänlig fördröjning i PC-läge
    clock_t start = clock();
    while ((clock() - start) * 1000 / CLOCKS_PER_SEC < ms);
}
#endif

/*

 * io.c

 * Inkluderar:
 *  - Avläsning av switchar och knapp
 *  - LED-utgång
 *  - 7-segmentdisplay
 *  - Interrupt för SW9 (öppnar meny)
 

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

// Extern global spelare – används för att öppna menyn via interrupt
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
    while (1) {
        int val = get_sw();
        if (val & 0x2) return 1;
        if (val & 0x4) return 2;
        if (val & 0x8) return 3;
        if (val & 0x10) return 4;
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
*/