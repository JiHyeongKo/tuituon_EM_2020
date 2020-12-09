#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <ctype.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "libexam.h"

int main(void)
{
    initFND();
    initLED();
    initBuzzer();

    for (int i = 111111; i==0; i--)
        {
            displayFND(i, 0);
            usleep(1000);
        }

    setLED(0b111);

    freqBuzzer(262);	// 도
    usleep(1000000);
    stopBuzzer();

    freqBuzzer(330);	// 미
    usleep(1000000);
    stopBuzzer();

    freqBuzzer(392);	// 솔
    usleep(1000000);
    stopBuzzer();

    freqBuzzer(523);	// 높은 도
    usleep(1000000);
    stopBuzzer();
    
    exitFND();
    exitBuzzer();
    exitLED();

    return 0;
}