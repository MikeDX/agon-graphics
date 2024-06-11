#include "screen5.h"
#include <stdio.h>
#include <stdlib.h>
#include <mos_api.h>
#include "graphics.h"
#include <agon/vdp_vdu.h>
#include "maths_utils.h"
#include "bmprint.h"

#define LOGO_COUNT 10
typedef struct Sprite {
    Point position;
    uint16_t bitmap_id;
} Sprite;

Sprite logos[LOGO_COUNT];
static font my_font;
Point origin;
float t = 0;

const char *scrolltext = "HELLO WORLD! THIS IS A TEST OF THE SCROLLING TEXT ROUTINE. IT SHOULD SCROLL AND LOOP!     ";
Point helloText;
vector2 textVect;
int xoff;

#define SIN_TABLE_SIZE 360
int sin_table[SIN_TABLE_SIZE];
int sin_use[SIN_TABLE_SIZE];

void generate_sin_table() {
    for (int i = 0; i < SIN_TABLE_SIZE; i++) {
        double angle = i * (2 * M_PI / SIN_TABLE_SIZE);
        sin_table[i] = (int)((double)sin(angle) * 50);
    }
}

void screen5_init(void) {
    change_screen_mode(136, false, true);
    memset(sin_use, 0, sizeof(sin_use));
    t = 0;
    helloText = (Point){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    textVect = (vector2){4,-6};
    xoff = SCREEN_WIDTH;

    ncotb_header header;
    bitmap_load_result res = load_bitmap_into_buffer(1, "images/ncot-logo.222", &header);
    if (res == MALLOC_FAIL) {
        printf ("Failed to allocate memory\n");
    }

    if (res != SUCCESS) {
        printf ("Failed to load %s\n", "images/ncot-logo.222");
        return;
    }
    assign_buffer_to_bitmap(1,RGBA2222,header.width,header.height);

    res = load_bitmap_into_buffer(2, "images/rmc.222", &header);
    if (res != SUCCESS) {
        return;
    }
    assign_buffer_to_bitmap(2, RGBA2222, header.width, header.height);

    for (int i = 0; i < LOGO_COUNT; i++) {
        logos[i].position.x = rand_between(0, SCREEN_WIDTH);
        logos[i].position.y = rand_between(0, SCREEN_HEIGHT);
        int rand = rand_between(0, 10);
        if (rand == 8) logos[i].bitmap_id = 2;
        else logos[i].bitmap_id = 1;
    }

    bm_load_font("fonts/08X08-F5/08X08-F5.fon", &my_font);
    origin.x = 0;
    origin.y = 0;
    generate_sin_table();
}


int screen5_update(void) {
    t+= 0.01;

    for (int i = 0; i < LOGO_COUNT; i++) {
        logos[i].position.x += 3;
        if (logos[i].position.x > SCREEN_WIDTH) {
            logos[i].position.x = -200;
            logos[i].position.y = rand_between(0, SCREEN_HEIGHT);
            int rand = rand_between(0, 10);
            if (rand >= 8) logos[i].bitmap_id = 2;
            else logos[i].bitmap_id = 1;
        }
    }
    return -1; // Continue with the current screen
}




void screen5_draw(void) {
    vdp_clear_screen();
    for (size_t i = 0; i < strlen(scrolltext); i++) {
        int x = xoff + (i * 32);
        if (x > -16 && x < SCREEN_WIDTH + 16) {
            int angle = ((int)(x * 1.145)) % SIN_TABLE_SIZE;
            int y = SCREEN_WIDTH / 2 + sin_table[angle];
            bm_put_char(&my_font, x, y, scrolltext[i]);
        }

        if (x > SCREEN_WIDTH) {
            break;
        }
    }
    bm_printf(&my_font, helloText.x,helloText.y, "HELLO WORLD!");
    for (int i = 0; i < LOGO_COUNT; i++) {
       vdp_plot_bitmap(logos[i].bitmap_id, logos[i].position.x,logos[i].position.y);
    }
    xoff -= 16;
    if (xoff < (int)(-32*strlen(scrolltext))) xoff = SCREEN_WIDTH;
    helloText.x += textVect.x;
    helloText.y += textVect.y;
    if (helloText.x < 0 || helloText.x+(strlen("HELLO WORLD!") * 32) > SCREEN_WIDTH) textVect.x *= -1;
    if (helloText.y < 0 || helloText.y+32 > SCREEN_HEIGHT) textVect.y *= -1;

    waitvblank();
    flip_buffer();
}

void screen5_exit(void) {
}