#include <stdio.h>
//
#include "f_util.h"
#include "ff.h"
#include "pico/stdlib.h"
#include "rtc.h"
//
#include "hw_config.h"

extern "C" void printAt(unsigned int x, unsigned int y, const char *fmt, ...);
extern "C" sd_card_t sd_card;

int testsd() {
    //stdio_init_all();
    time_init();

    printf( "Hello, world!\n");
    // See FatFs - Generic FAT Filesystem Module, "Application Interface",
    // http://elm-chan.org/fsw/ff/00index_e.html
    sd_card_t *pSD = sd_get_by_num(0);

    
    printf("found 0\n");
    FRESULT fr = f_mount(&pSD->fatfs, pSD->pcName, 1);
    printf( "r 0");
    if (FR_OK != fr) printAt(0,21, "f_mount error: %s (%d)\n", FRESULT_str(fr), fr);
    printf("Mounted sd 0\n");
    FIL fil;
    const char* const filename = "filename.txt";
    fr = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE);
    if (FR_OK != fr && FR_EXIST != fr)
        printf("f_open(%s) error: %s (%d)\n", filename, FRESULT_str(fr), fr);
    printAt(0,23,"Opened filename.txt\n");
    if (f_printf(&fil, "Hello, world!\n") < 0) {
        printf("f_printf failed\n");
    }
    fr = f_close(&fil);
    if (FR_OK != fr) {
        printf("f_close error: %s (%d)\n", FRESULT_str(fr), fr);
    }
    f_unmount(pSD->pcName);

    printf("Goodbye, world!\n");

    return 0;
}
