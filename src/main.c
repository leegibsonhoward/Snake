#include <allegro.h>
#include <stdbool.h>

#include "switch.h"

#define MODE GFX_AUTODETECT_WINDOWED
#define WIDTH 640
#define HEIGHT 480
#define BLACK makecol(0, 0, 0)
#define WHITE makecol(255, 255, 255)
#define DEBUG_BLUE makecol(0, 0, 200)

// variables and functions
BITMAP *buffer;
BITMAP *console;
bool is_running = false;
int debug(void);
bool debug_on = false;

// timer variables
volatile int counter;
volatile int ticks;
volatile int framerate; 
volatile int resting, rested;
volatile int key_down, key_up;

void keypress_watcher(int scancode)
{
    if (scancode & 0x80) {
       key_up = 1;
    } else {
       key_down = 1;
    }
} END_OF_FUNCTION(keypress_watcher)

// timer1 function, calculate framerate every second
void timer1(void)
{
    counter++;
    framerate = ticks;
    ticks = 0;
    rested = resting;
}
END_OF_FUNCTION(timer1)

// rest1 function, what to do while resting
void rest1(void)
{
    resting++;
}

int main(void)
{
    // initialize
    allegro_init();
    set_color_depth(16);
    set_gfx_mode(MODE, WIDTH, HEIGHT, 0, 0);
    srand(time(NULL));
    install_keyboard();
    install_timer();

    // create double buffer
    buffer = create_bitmap(SCREEN_W, SCREEN_H);
    console = create_bitmap(SCREEN_W, SCREEN_H / 12);

    // lock interrupt variables
    LOCK_VARIABLE(counter);
    LOCK_VARIABLE(framerate);
    LOCK_VARIABLE(ticks);
    LOCK_VARIABLE(resting);
    LOCK_VARIABLE(rested);
    LOCK_VARIABLE(key_down);
    LOCK_VARIABLE(key_up);
    LOCK_FUNCTION(timer1);
    LOCK_FUNCTION(rest1);

    // install interrupt
    install_int(timer1, 1000);

    // keyboard input low-level, catch all keystrokes
    keyboard_lowlevel_callback = keypress_watcher;
    
    // disable keyboard repeat
    set_keyboard_rate(0, 0);
    
    // start game
    is_running = true;

    // game loop
    while (is_running)
    {
        // update ticks
        ticks++;

        // slow the game down
        // it atleast, rests once
        resting = 0;
        rest_callback(16, rest1);

        // clear bitmap
        clear_bitmap(buffer);

        // main check for input
        if (key_down)
        {
            key_down = 0;

            // SHORT CIRCUIT game loop
            if(key[KEY_Q])
                // (TODO): create exit confirm, then goto main menu
                is_running = false;

            // toggle DEBUG_CONSOLE menu
            if(key[KEY_D])
                flip_debug_switch();
        }

        // nothing done on key_up, do nothing?
        if (key_up)
        {
            key_up = 0;
        }
        
        // debug_on flag true, then debug
        if(is_debug_on())
        {
            clear_to_color(console, DEBUG_BLUE);
            debug();
        }

        // update the screen
        acquire_screen();
        blit(buffer, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
        release_screen();
    }

    // remove objects from memory
    destroy_bitmap(buffer);

    allegro_exit();

    return 0;
}
END_OF_MAIN()

int debug(void)
{
    if(debug_on)
    {

        textprintf_ex(console, font, (SCREEN_W / 2) - 40, 5, WHITE, -1,
                "Snake v0.1");
        textprintf_ex(console, font, 10, 7, WHITE, -1,
        "COUNTER %d", counter);
        textprintf_ex(console, font, 10, 17, WHITE, -1,
        "FRAMERATE %d", framerate);
        textprintf_ex(console, font, 10, 27, WHITE, -1,
        "RESTING %d", rested);
        blit(console, buffer, 0, 0, 0, 0, SCREEN_W, SCREEN_H / 12);

    }

    return -1;
}