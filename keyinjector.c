#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <time.h>
#include "ui.h"
#include "keyinjector.h"
#include "gestures.h"
#include "gesture_definition.h"
#include "config.h"

enum Key {Left=105, Right=106, Home=102,Power=116, Up=103, Down=108, Esc=1};

enum TouchStatus {Disabled, Gestures};

static enum TouchStatus touch_status = Disabled;


static int f;
static int w;
void injector_init() {
    f = open(BUTTONS,O_WRONLY);
    if (!f){
        fprintf(stderr, "cannot open buttons");
        exit(1);
    }

    /* w = open(WACOM, O_WRONLY); */
    /* if (!w){ */
    /*     fprintf(stderr, "cannot open digitizer"); */
    /*     exit(1); */
    /* } */
}

extern bool verbose;
void press_button(int code);
void press_pen(int x, int y, long time);
void move_pen(int x, int y, long time);

void interpret_gesture(struct Gesture *g){
    if (g->type == TwoTapWide) {
        touch_status = ((int)touch_status + 1) % 2; //cycle the states
        switch(touch_status) {
            case Disabled:
                show("touch disabled");
                break;
            case Gestures:
                show("gestures enabled");
                break;
        }
        return;
    }

    time_t rawtime;
    time(&rawtime);
    switch(g->type) {
        case SwipeDownLong:
            {
            struct tm* timeinfo = localtime(&rawtime);
            char buffer[100];
            strftime(buffer,sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
            show(buffer);
            }
            break;

        default:
            break;

    }

    if (touch_status == Disabled)
        return;

    //require touch enabled
    switch(g->type){
        case Gestures:
            if (verbose)
                printf("execute custom script\n");
            system("~/scripts/swipeup.sh");
            break;
        default:
            break;

    }
}


//todo: wip
void move_pen(int x, int y, long time)
{
    struct input_event key_input_event;

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_X;
    key_input_event.value = x;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));


    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_Y;
    key_input_event.value = y;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_DISTANCE;
    key_input_event.value = 16;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = 24;
    key_input_event.value = 3000;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

}
void press_pen(int x, int y, long time){
    struct input_event key_input_event;

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = 1;
    key_input_event.code = 320;
    key_input_event.value = 1;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));


    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_X;
    key_input_event.value = x;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));


    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_Y;
    key_input_event.value = y;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_DISTANCE;
    key_input_event.value = 16;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = 1; //touch
    key_input_event.code = 330;
    key_input_event.value = 1;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_X;
    key_input_event.value = x;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = ABS_Y;
    key_input_event.value = y;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));
    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = 24;
    key_input_event.value = 3300;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = 25;
    key_input_event.value = 21;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = 26;
    key_input_event.value = 800;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_ABS;
    key_input_event.code = 27;
    key_input_event.value = -100;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    key_input_event.time.tv_sec = time;
    write(w, &key_input_event,sizeof(key_input_event));
}

void press_button(int code){
    struct input_event key_input_event;
    memset(&key_input_event, 0, sizeof(key_input_event));

    debug_print("inject button %d \n", code);

    key_input_event.type = EV_KEY;
    key_input_event.code = code;
    key_input_event.value = 1;
    write(f, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    write(f, &key_input_event,sizeof(key_input_event));

    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_KEY;
    key_input_event.code = code;
    write(f, &key_input_event,sizeof(key_input_event));


    memset(&key_input_event, 0, sizeof(key_input_event));
    key_input_event.type = EV_SYN;
    key_input_event.code = SYN_REPORT;
    write(f, &key_input_event,sizeof(key_input_event));
}
