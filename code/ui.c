#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stddef.h>

#include "tft.h"
#include "touchscreen.h"
#include "ui.h"

#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define BLUE    0x001F
#define CYAN    0x07FF
#define YELLOW  0xFFE0
#define GRAY    0x8410
#define DARK    0x4208

typedef enum {
    SCREEN_HOME,
    SCREEN_CONFIRM,
    SCREEN_DISPENSE
} Screen;

typedef enum {
    DRINK_NONE,
    DRINK_ONE,
    DRINK_TWO,
    DRINK_THREE
} Drink;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;
    const char *label;
} Button;

static Screen current_screen = SCREEN_HOME;
static Drink selected_drink = DRINK_NONE;

// Generic drink names for now
static const char *drink_one_name = "DRINK 1";
static const char *drink_two_name = "DRINK 2";
static const char *drink_three_name = "DRINK 3";

// Home screen buttons
static Button drink_one_btn   = {40, 80, 170, 60, NULL};
static Button drink_two_btn   = {270, 80, 170, 60, NULL};
static Button drink_three_btn = {40, 170, 170, 60, NULL};
static Button next_btn        = {270, 170, 170, 60, "NEXT"};

// Confirmation screen buttons
static Button start_btn = {80, 210, 150, 60, "START"};
static Button back_btn  = {260, 210, 150, 60, "BACK"};

// Dispense screen buttons
static Button pause_btn  = {80, 220, 150, 60, "PAUSE"};
static Button cancel_btn = {260, 220, 150, 60, "CANCEL"};

static const char* drink_name(Drink d) {
    switch (d) {
        case DRINK_ONE: return drink_one_name;
        case DRINK_TWO: return drink_two_name;
        case DRINK_THREE: return drink_three_name;
        default: return "NONE";
    }
}

uint8_t ui_button_pressed(uint16_t x, uint16_t y, uint16_t w, uint16_t h, TouchPoint p) {
    return p.touched &&
           p.x >= x && p.x <= x + w &&
           p.y >= y && p.y <= y + h;
}

static uint8_t button_hit(Button b, TouchPoint p) {
    return ui_button_pressed(b.x, b.y, b.w, b.h, p);
}

static void draw_button(Button b, uint16_t fill, uint16_t text_color) {
    tft_fill_rect(b.x, b.y, b.w, b.h, fill);
    tft_draw_rect(b.x, b.y, b.w, b.h, WHITE);
    tft_draw_string(b.x + 18, b.y + 20, b.label, text_color, fill, 2);
}

static void draw_drink_button(Button b, Drink d) {
    if (selected_drink == d) {
        draw_button(b, GREEN, BLACK);
    } else {
        draw_button(b, CYAN, BLACK);
    }
}

static void ui_draw_home_components(void) {
    tft_draw_string(90, 25, "SELECT DRINK", WHITE, BLACK, 3);

    draw_drink_button(drink_one_btn, DRINK_ONE);
    draw_drink_button(drink_two_btn, DRINK_TWO);
    draw_drink_button(drink_three_btn, DRINK_THREE);

    if (selected_drink == DRINK_NONE) {
        draw_button(next_btn, GRAY, WHITE);
    } else {
        draw_button(next_btn, YELLOW, BLACK);
    }
}

void ui_set_drink_names(
    const char *name_1,
    const char *name_2,
    const char *name_3
){
    drink_one_name = name_1;
    drink_two_name = name_2;
    drink_three_name = name_3;

    drink_one_btn.label = drink_one_name;
    drink_two_btn.label = drink_two_name;
    drink_three_btn.label = drink_three_name;

    if(current_screen == SCREEN_HOME){
        ui_draw_home();
    }
}

void ui_draw_home(void) {
    tft_fill_screen(BLACK);
    ui_draw_home_components();
}

void ui_draw_confirm(void) {
    tft_fill_screen(BLACK);

    tft_draw_string(80, 40, "CONFIRM", WHITE, BLACK, 3);
    tft_draw_string(70, 120, "DRINK:", WHITE, BLACK, 2);
    tft_draw_string(190, 120, drink_name(selected_drink), CYAN, BLACK, 2);

    draw_button(start_btn, GREEN, BLACK);
    draw_button(back_btn, GRAY, WHITE);
}

void ui_draw_dispense(void) {
    tft_fill_screen(BLACK);

    tft_draw_string(70, 35, "DISPENSING", WHITE, BLACK, 3);
    tft_draw_string(100, 105, drink_name(selected_drink), CYAN, BLACK, 2);

    tft_draw_rect(70, 160, 340, 30, WHITE);
    tft_fill_rect(75, 165, 100, 20, GREEN); // placeholder progress

    draw_button(pause_btn, YELLOW, BLACK);
    draw_button(cancel_btn, RED, WHITE);
}

void ui_init(void) {
    current_screen = SCREEN_HOME;
    selected_drink = DRINK_NONE;

    drink_one_btn.label = drink_one_name;
    drink_two_btn.label = drink_two_name;
    drink_three_btn.label = drink_three_name;

    ui_draw_home();
}

void ui_update(void) {
    TouchPoint p = touch_get_point();

    if (!p.touched) {
        return;
    }

    if (current_screen == SCREEN_HOME) {
        if (button_hit(drink_one_btn, p)) {
            selected_drink = DRINK_ONE;
            ui_draw_home();
        } 
        else if (button_hit(drink_two_btn, p)) {
            selected_drink = DRINK_TWO;
            ui_draw_home();
        } 
        else if (button_hit(drink_three_btn, p)) {
            selected_drink = DRINK_THREE;
            ui_draw_home();
        } 
        else if (button_hit(next_btn, p) && selected_drink != DRINK_NONE) {
            current_screen = SCREEN_CONFIRM;
            ui_draw_confirm();
        }
    }

    else if (current_screen == SCREEN_CONFIRM) {
        if (button_hit(start_btn, p)) {
            current_screen = SCREEN_DISPENSE;
            ui_draw_dispense();

            // later: start_dispense(selected_drink);
        } 
        else if (button_hit(back_btn, p)) {
            current_screen = SCREEN_HOME;
            ui_draw_home();
        }
    }

    else if (current_screen == SCREEN_DISPENSE) {
        if (button_hit(pause_btn, p)) {
            // later: pause_dispense();
            draw_button(pause_btn, BLUE, WHITE);
        } 
        else if (button_hit(cancel_btn, p)) {
            // later: cancel_dispense();
            current_screen = SCREEN_HOME;
            selected_drink = DRINK_NONE;
            ui_draw_home();
        }
    }

    _delay_ms(30);
}
