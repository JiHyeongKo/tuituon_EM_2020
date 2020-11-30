#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>

#include "sharedMemory.h"

int flag = 0;
int count = 0;

int msgSend(void) // 메시지 보내는 함수
{
    msg_t msgTx;    // 구조체 선언
    int msgQueue = msgget((key_t) MY_MSG_ID, IPC_CREAT|0666);   // ID get

    if(msgQueue == -1)  // failed
    {
        printf("MSG: 처음부터 실패!\r\n");
        return -1;
    }
    // 성공시

    if(flag == 1)
    {
        flag = 0;
        msgTx.messageType = 1;  // he said 1 is good enough

        printf("난 %d번째 메시지 가져가라고 했다?\r\n", count);
        msgsnd(msgQueue, &msgTx, sizeof(msgTx.flag), 0);    // 가져가라고 flag 전송
        count++;
    }

    return count;
}

int shMemWrite(void) // 메모리 쓰는 함수
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
    
    if( ((int)(sharedMemAddr)) == -1 )    // 실패시
    {
        printf("SHM: 두번째부터 실패!\r\n");
        return -2;
    }

    sleep(1);
    printf("요거 씁니다: %d\r\n", count);   // 쓴다?
    *((int*)sharedMemAddr) = count; // 진짜 썼어요
    shmdt(shMID);   // detach
    flag = 1;

    return 0;
}

int main(void)
{
    while(1)
    {
        shMemWrite();
        msgSend();
    }
}