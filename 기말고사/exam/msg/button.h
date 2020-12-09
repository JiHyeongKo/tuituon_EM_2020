#ifndef _BUTTON_H_
#define _BUTTON_H_
#define MESSAGE_ID 999
#define INPUT_DEVICE_LIST "/dev/input/event"
#define PROBE_FILE "/proc/bus/input/devices"
#define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
#define HAVE_TO_FIND_2 "H: Handlers=kbd event"
typedef struct
{
    long int messageNum;
	long int messageType;
} BUTTON_MSG_T;

int buttonInit(void);
int buttonExit(void);
int buttonGet(BUTTON_MSG_T *msgrxdata);
int probeButtonPath(char *newPath);
void *buttonThFunc(void *arg);

int startbutton(void);
#endif