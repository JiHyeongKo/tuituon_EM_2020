#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

pthread_t object_1; // 스레드 1
pthread_t object_2; // 스레드 2
pthread_mutex_t lockInput;  //버퍼와 레디 보호
char buffer[201];   // gets 입력 버퍼
int ready = 0;  // 입력 받았는지 상태

void* inputKeyboard(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lockInput); // 락
        if(ready == 0)
        {
            printf("Thread 1 : $ ");            
            gets(buffer);
            ready = 1;  // 플래그
            pthread_mutex_unlock(&lockInput);   // 언락
        }
        
        else
        {
            pthread_mutex_unlock(&lockInput);   // 언락
            usleep(1);
        }
    }
    return NULL;
}

void* outputMonitor(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&lockInput); // 락
        if(ready == 1)
        {
            printf("->Thread 2 : Output [%s]\r\n", buffer);
            ready = 0;      
            pthread_mutex_unlock(&lockInput);   // 언락
        }

        else
        {
            pthread_mutex_unlock(&lockInput);   // 언락
            usleep(1);
        }
    }
    return NULL;
}

int main(void)
{
    int err_1 = 0;
    int err_2 = 0;

    if (pthread_mutex_init(&lockInput, NULL) != 0)
    {
        printf("Mutex init failed\r\n");
        return -1;
    }

    while(1)
    {
        err_1 = pthread_create(&(object_1), NULL, &inputKeyboard, NULL);    // 1번 스레드 생성
        if(err_1 != 0)  // 걸리면 망함
            printf("Thread Create Error: %d\r\n", err_1);
            
        err_2 = pthread_create(&(object_2), NULL, &outputMonitor, NULL);    // 2번 스레드 생성
        if(err_2 != 0)  // 얘도
            printf("Thread Create Error: %d\r\n", err_2);

        if(!(err_1 || err_2))   // 한 번 생성되면 끝
            break;
    }

    pthread_join(object_1, NULL);
    pthread_join(object_2, NULL);
}