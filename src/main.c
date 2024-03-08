#include <agon/vdp_vdu.h>
#include <agon/vdp_key.h>
#include <stdio.h>
#include <mos_api.h>
#include "graphics.h"
#include <math.h>
#include <string.h>
#include "screen.h"
#include "screen1.h"
#include "screen2.h"
#include "screen3.h"

// https://github.com/AgonConsole8/agon-docs/blob/main/VDP---Screen-Modes.md
#define SC_MODE 136

volatile SYSVAR *sv;

int main(void)
{
    
    sv = vdp_vdu_init();
    if ( vdp_key_init() == -1 ) return 1;

    set_video_mode(SC_MODE);
    vdp_clear_screen();
    vdp_logical_scr_dims( true );
    vdp_cursor_enable( false );

    define_screen(screen1_init, screen1_update, screen1_draw);
    define_screen(screen2_init, screen2_update, screen2_draw);
    define_screen(screen3_init, screen3_update, screen3_draw);

    set_current_screen(2);

    //putch(19); putch(0); putch(255); putch(255);putch(0);putch(0); 
    while (1) {
        vdp_clear_screen();

        run_screens();

        waitvblank();
        flip_buffer();
    }

    return 0;
}

// # vim: set expandtab tabstop=4:
