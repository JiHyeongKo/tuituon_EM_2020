#include <unistd.h> 
#include <fcntl.h> 
#include <stdlib.h>
#include <stdio.h>

int main ( int argc, char *argv[] ) 
{ 
    int fdin, fdout; 
    off_t fdin_size;
    ssize_t nread; 

    if ( argc < 3 )
        {
            printf ("cp [source file] [dest file]\r\n");
            exit(-1);
        }
   
    fdin = open(argv[1], O_RDONLY); 
    fdout = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644); 
    fdin_size = lseek(fdin, 0, SEEK_END);   // fdin size
    
    lseek(fdin, 0, SEEK_SET);   // 포인터 되돌림.

    char *buffer = malloc(fdin_size);
    if(buffer == NULL)
    {
        printf("error\n");
        return -1;
    }
    /* 정상적으로 읽어 들인 내용이 1바이트 이상인 동안 반복문 수행 */ 
    while((nread = read(fdin, buffer, fdin_size)) > 0)  //sizeof() 시간 소모가 크다;
        { 
            /* write가 비정상적으로 수행되었다. (실패) */ 
            if(write(fdout, buffer, nread) < nread)
                { 
                    printf ("File copy failed\r\n");
                    close(fdin); 
                    close(fdout); 
                    free(buffer);
                } 
        }

        close(fdin); 
        close(fdout); 
        free(buffer);

        return argc;
} 