#include <agon/vdp_vdu.h>
#include <agon/vdp_key.h>
#include <stdio.h>
#include <stdlib.h>
#include <mos_api.h>
#include "graphics.h"
#include <math.h>
#include <string.h>
#include "screen.h"
#include "screen1.h"
#include "screen2.h"
#include "screen3.h"
#include "screen4.h"
#include "screen5.h"
#include "screen6.h"
#include "keyboard.h"
// https://github.com/AgonConsole8/agon-docs/blob/main/VDP---Screen-Modes.md
#define SC_MODE 136
//#define SC_MODE 8

volatile SYSVAR *sv;

void vdp_reset(void) {
//    VDU 23, 0, &C8, 2, flags
    putch(23);
    putch(0);
    putch(0xC8);
    putch(2);
    putch(255);
}

#define setup_screen(X) define_screen(screen ## X ## _init, screen ## X ## _update, screen ## X ## _draw, screen ## X ## _exit);

int main(int argc, char **argv)
{
    
    sv = vdp_vdu_init();
    if ( vdp_key_init() == -1 ) return 1;

    vdp_reset();
    
    init_keys();

    setup_screen(1);
    setup_screen(2);
    setup_screen(3);
    setup_screen(4);
    setup_screen(5);
    setup_screen(6);
    
    // Each screen's index is 1 less than its name, so screen 1 is id 0
    // Here's what each screen does
    // Screen 1 - Draws the colour palette
    // Screen 2 - Draws some fancy rotating shapes
    // Screen 3 - Mystify screensaver
    // Screen 4 - Starfield screensaver
    // Screen 5 - Moving logos demo thing
    // Screen 6 - Scrolling tilemap

    if (argc == 2) {
        int current_screen = atoi(argv[1]);
        if (current_screen < 6)
            set_current_screen(current_screen);
        else
            set_current_screen(0);
    } else
        set_current_screen(4);

    while (1) {
        update_keys();
        if (IS_KEY_PRESSED(KEY_ESC))
            break;

        run_screens();
    }

    set_video_mode(0);
    vdp_clear_screen();
    vdp_logical_scr_dims( true );
    vdp_cursor_enable( true );
    free_screens();
    return 0;
}

// # vim: set expandtab tabstop=4:
