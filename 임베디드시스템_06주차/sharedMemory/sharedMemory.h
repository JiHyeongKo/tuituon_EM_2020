#ifndef _SHARED_MEMORY_H_
#define _SHARED_MEMORY_H_

#define MY_MSG_ID 60162460  // 우편함 이름
#define MY_SHM_ADDR 9168    // 광장 주소
#define MY_SHM_SIZE 128    // shared memory size

typedef struct
{
    long int messageType;   // 메시지 타입
    char flag;  // shared memory 쓰고 나서 플래그
} msg_t;

#endif