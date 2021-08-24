#include <stdio.h>
//
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
//
#include "hw_config.h"

extern "C" void printAt(unsigned int x, unsigned int y, const char *fmt, ...);

int testsd() {
    //stdio_init_all();
    time_init();

    printAt(0,20, "Hello, world!");
    // See FatFs - Generic FAT Filesystem Module, "Application Interface",
    // http://elm-chan.org/fsw/ff/00index_e.html
    sd_card_t *pSD = sd_get_by_num(0);
    printAt(0,21, "found 0");
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    printAt(0,21, "r 0");
    if (FR_OK != fr) printAt(0,21, "f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    printAt(0,21, "Mounted sd 0");
    FIL fil;
    const char* const filename = "filename.txt";
    fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr)
        printAt(0,22,"f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    printAt(0,23,"Opened filename.txt");
    if (f_printf(&fil, "Hello, world!\n") < 0) {
        printAt(0,23,"f_printf failed\n");
    }
    fr = f_close(&fil);
    if (FR_OK != fr) {
        printAt(0,24,"f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    f_unmount(pSD->pcName);

    printAt(0,25,"Goodbye, world!");

    return 0;
}
