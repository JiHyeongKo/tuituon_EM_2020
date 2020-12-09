#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <linux/input.h>
#include <unistd.h> // for open/close
#include <fcntl.h> // for O_RDWR
#include <sys/ioctl.h> // for ioctl
#include <sys/msg.h>
#include <pthread.h>
#include "button.h"

// first read input device
static int fd=0;
struct input_event stEvent;
pthread_t buttonTh_id;

int probeButtonPath(char *newPath)
{
	int returnValue = 0; //button에 해당하는 event#을 찾았나?
	int number = 0; //찾았다면 여기에 집어넣자
	FILE *fp = fopen(PROBE_FILE,"rt"); //파일을 열고
	#define HAVE_TO_FIND_1 "N: Name=\"ecube-button\"\n"
	#define HAVE_TO_FIND_2 "H: Handlers=kbd event"
	while(!feof(fp)) //끝까지 읽어들인다.
	{
		char tmpStr[200]; //200자를 읽을 수 있게 버퍼
		fgets(tmpStr,200,fp); //최대 200자를 읽어봄
		//printf ("%s",tmpStr);
		if (strcmp(tmpStr,HAVE_TO_FIND_1) == 0)
		{
			printf("YES! I found!: %s\r\n", tmpStr);
			returnValue = 1; //찾음
		}
		if (
			(returnValue == 1) && (strncasecmp(tmpStr, HAVE_TO_FIND_2,strlen(HAVE_TO_FIND_2)) == 0) //Event??을 찾았으면
			)
		{
			printf ("-->%s",tmpStr);
			printf("\t%c\r\n",tmpStr[strlen(tmpStr)-3]);
			number = tmpStr[strlen(tmpStr)-3] - '0';
			//Ascii character '0'-'9' (0x30-0x39)
			//to interger(0)
			break;
		}
	}
	fclose(fp);
	if (returnValue == 1)
	sprintf (newPath,"%s%d",INPUT_DEVICE_LIST,number);
	return returnValue;
}

void *buttonThFunc(void *arg)
{
	int readSize = 0;
	int countOn, countOff, flagMorse = 0;	//버튼하나로 모스부호를 만들기위한 카운터와 플래그
	int shortTime = 3;		//단점 입력 체크 시간
	int endTime = 8;			//문자 입력 체크 시간
	char code[32] = {'0',};		//모스부호 저장
	int codeIndex = 0;
	BUTTON_MSG_T msgtxdata;
	int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);

	while(1)
	{
      readSize = read(fd, &stEvent, sizeof(stEvent));
      
      if(readSize != sizeof(stEvent) )
      {
         continue;
      }
      
      if( stEvent.type == EV_KEY )
      {
         /*printf("EV_KEY(");
         switch(stEvent.code)
         {
            case KEY_HOME:          printf("Home key):");	break;
            case KEY_SEARCH:       printf("Search Key):");	break;
            case KEY_BACK:          printf("Back key):");	break;
            case KEY_MENU:          printf("Menu key):");	break;
            case KEY_VOLUMEDOWN:    printf("Volume down key):");	break;
            case KEY_VOLUMEUP:      printf("Volume up key):");	break;
            default: break;
         }*/
         
         msgtxdata.messageNum = 1;
		 msgtxdata.messageType= stEvent.code;

         if( !stEvent.value )
        {
            msgsnd(msgID, &msgtxdata, sizeof(msgtxdata)-sizeof(msgtxdata.messageNum),0);
        }
	  }
   }
}

int buttonInit(void)
{
	int inputIndex;
	char inputDevPath[200]={0,};

	if ( probeButtonPath(inputDevPath) == 0)
	{
		printf ("ERROR! File Not Found!\r\n");
		printf ("Did you insmod?\r\n");
		return 0;
	}

	fd=open (inputDevPath, O_RDONLY);
	
	if(fd == -1)
	{
		printf("file open error\n");	
		return-1;
	}
	
	pthread_create(&buttonTh_id, NULL, buttonThFunc, NULL);

	return 1;
}

int buttonExit(void)
{
	pthread_join(buttonTh_id, NULL);
	close(fd);

	return 0;
}

int buttonGet(BUTTON_MSG_T *msgrxdata)
{
	int msgID = msgget (MESSAGE_ID, IPC_CREAT|0666);
    int msgValue =0;
    int checkOnce =0;

	if(msgID == -1)
	{
		printf("Cannot get msgID, Return\n");
		return -1;
	}

    while(1)
    {
        msgValue = msgrcv(msgID, msgrxdata, sizeof(*msgrxdata)-sizeof(msgrxdata->messageNum), 0, IPC_NOWAIT);  // 메시지함 없으면 바로 끝

        if(msgValue == -1)   // 더 이상 없으면
        {
            break;
        }
        else
        {
            if(checkOnce==0)
            {
                printf("%d 메시지 안 가져가셨어요.\n", msgValue);
                checkOnce++;
            }
        }
    }

    msgValue = msgrcv(msgID, msgrxdata, sizeof(*msgrxdata)-sizeof(msgrxdata->messageNum), 0, 0);   // 메시지함 없으면 대기
    printf("msg recv: %ld\n", msgrxdata->messageType);
}

int startbutton(void)
{
	BUTTON_MSG_T msgrxdata;
	buttonInit();
	
	while(1)
	{
		buttonGet(&msgrxdata);
	}

	buttonExit();
}