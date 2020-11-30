#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

#include "sharedMemory.h"

int msgRecv(void) // 메시지 받는 함수
{
    int checkOnce =0;
    msg_t msgRx;    // 구조체 선언
    int msgQueue = msgget((key_t) MY_MSG_ID, IPC_CREAT|0666);   // ID get
    int returnValue  = 0;

    if(msgQueue == -1)  // failed
    {
        printf("MSG: 처음부터 실패!\r\n");
        return -1;
    }
    // 성공시

    while(1)
    {
        returnValue = msgrcv(msgQueue, &msgRx, sizeof(msgRx.flag), 0, IPC_NOWAIT);  // 메시지함 없으면 바로 끝

        if(returnValue == -1)   // 더 이상 없으면
        {
            break;
        }
        else
        {
            if(checkOnce==0)    // 한번만 읽게.. 왜 여러번 읽는진 모르겠음..;
            {
                printf("우편함 터져요. 미리미리 다닙시다. ");
                shMemRead();
                checkOnce++;
            }
        }
    }
    
    returnValue = msgrcv(msgQueue, &msgRx, sizeof(msgRx.flag), 0, 0);   // 메시지함 없으면 대기
    printf("message Comes. I need to check shared Memory\r\n");
}

int shMemRead(void) // 메모리 읽는 함수
{  
    int shMID = shmget((key_t)MY_SHM_ADDR, MY_SHM_SIZE, IPC_CREAT|0666);    // ADDR get
    
    if(shMID == -1) // failed
    {
        printf("SHM: 처음부터 실패!\r\n");
        return -1;
    }
    // 성공시
    char *sharedMemAddr;
    sharedMemAddr = shmat(shMID, (void *)NULL, 0);  // attach
    
    if( ((int)sharedMemAddr) == -1)    // 실패시
    {
        printf("SHM: 두번째부터 실패!\r\n");
        return -2;
    }

    printf("Read shared memory: %d\r\n", *((int*)sharedMemAddr));
    shmdt(shMID);   // detach
}

int main(void)
{
    while(1)
    {
        msgRecv();
        shMemRead();
    }
}