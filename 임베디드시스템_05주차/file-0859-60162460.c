#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	char command[1000];	// 명령어 string 저장을 위한 배열.
	char executingList[10][1000];	// 실행 리스트 2차원 배열 저장. 10개, 999자까지
	char *executingListPtr[10];	// 실행 리시트 포인트 배열 저장. 10개
	char prevWorkinjDir[2048] = {0,};	// 이전 경로를 저장하기 위한 버퍼도 추가..

	while (1)
	{
		char currWorkingDir[2048]={0,};	// 경로를 저장하기 위한 버퍼. {0,} 모두 0으로 초기화.
		getcwd(currWorkingDir, sizeof (currWorkingDir));	// 현재 작업 중인 디렉토리 경로를 버퍼 크기(2048바이트)만큼 불러옴. 
		printf("[\033[1m\033[40;32mMy_Shell\033[0m] \033[40;36m%s$\033[0m ",currWorkingDir);	//Jaemin: 이 라인은 굳이 주석을 달 필요 없음.

		gets(command);	//	문자열을 입력받아 command에 저장.
		char *ptr = strtok(command," ");	// " "를 만나면 자르고
		if (ptr == NULL) continue;	// 입력 안하면 다시 while 처음으로
		
		printf ("StrTok:%s\r\n",ptr);	// " " 전까지 출력
		strcpy(executingList[0],ptr);	// 배열에 키보드로 처음 입력한 것 복사
		executingListPtr[0]=&executingList[0][0];	// 배열 포인터로 해당 배열 주소 넣어주고

		for (int numberOfToken=1;numberOfToken<10;numberOfToken++)	// 처음거는 끝났으니까, NULL 다음부터 다시..
		{
			ptr = strtok(NULL," ");	// 또 " " 만나면 ptr에 넣어주고
			if (ptr == NULL) 	// 없으면
			{
				executingListPtr[numberOfToken] = NULL; // 앞선 입력이 있는 포인터 배열의 다음 배열에 NULL
				break;	// Escape
			}
			strcpy(executingList[numberOfToken],ptr);	// NULL 아니면 배열에 넣어주고
			executingListPtr[numberOfToken] = &executingList[numberOfToken][0];	// 다음 포인터 배열에 주소를 주고
		}	

		for (int numberOfToken=0;numberOfToken<10;numberOfToken++)	// 이제 처음브터 루프
		{
			if (executingListPtr[numberOfToken]!=NULL) // 주소가 들어있으면
			{
				printf ("Tok %d->[%s]\r\n",numberOfToken, executingListPtr[numberOfToken]);	// 안에 있는 문자열 출력
			}
			else	// 주소가 없으면
			{
				break;	// Escape
			}
		}

		if (strcmp(executingListPtr[0],"도움")==0)	// 처음에 도움이 들어있으면?
		{
			printf ("도움말... 은 man 명령어를 실행시켜 보든지 말든지\r\n");	// 해당 문자열 출력
			printf ("종료는 quit \r\n");	// 얘도...
			continue;	// 그리고 다시 처음 while로 이동해서 처음부터.
		}

		else if (strcmp(executingListPtr[0],"quit")==0)	// 처음에 quit이 들어있으면?
		{
			printf ("임베디드시스템은 재미있어요!\r\n");	// 출력
			break;	// Escape
		}

		/*여기서 부터*/
		else if(strcmp(executingListPtr[0],"cd")==0)	// cd 입력하면; cd는 함수로 구현.
		{
			
			if(executingListPtr[1] == NULL || (strcmp(executingListPtr[1], "~") == 0)) // cd 다음에 입력한 문자가 없거나 ~를 입력한 경우에 유저 디렉토리로 간다 
				{
					getcwd(prevWorkinjDir, sizeof(prevWorkinjDir));
					chdir(getenv("HOME"));	// 환경변수... 홈으로 가기 위한 함수..
					continue;	// 사실 이거 필요없지만 도는 방향 보여주기 쉽게..
				}

			else if(strcmp(executingListPtr[1], "-") == 0)	// 이전 경로로
				{
					printf("%s \n", prevWorkinjDir);
					chdir(prevWorkinjDir);	// 이전 디렉토리
					strcpy(prevWorkinjDir, currWorkingDir);	// 현재 디렉토리를 복사, cd - 연속입력 대비
					continue;
				}

			else if(executingListPtr[1] != NULL)	// cd 다음에 입력한 문자가 있을 경우에	
				{
					getcwd(prevWorkinjDir, sizeof(prevWorkinjDir));

					if(chdir(executingListPtr[1]) == -1)	// 디렉토리 이동에 실패했을 경우에
					{
						printf("그런 파일이나 디렉토리가 없읍니다.\r\n");
					}
					else
						continue;	// 끝나면 다시 커맨드 입력받으러 갑니다.
				}
		}

		else	// 도움이나 quit이나 cd가 아닌 명령어..
		{
			pid_t pid = fork();	// cd 외 명령어는 프로세스 복제 필요
			int status;	// CP가 어떤 값을 리턴할지 저장

			if ( pid == 0 )	// CP가 할 일
			{
				execvp(executingListPtr[0], executingListPtr);	// 명령어 수행하고 종료
				exit(10);	// 실패하면 10 반환
			}

			else if( pid > 0 )	// PP가 할 일
			{
				wait(&status);	// CP 죽을때까지 대기
				if(status / (16*16) == 10)	// 실패하면 문자열 출력하고 처음으로
					printf("없는 명령어 어서오시고. \r\n");
				
				continue;	// 죽으면 출력하고 다시 위로
			}
			
			else	// fork 실패시
			{
				printf("call fork failure. \r\n");
				continue;	// 처음부터 명령어 다시 입력
			}
		}
		/*여기까지 채워 넣으세요*/	
		}
	printf ("진짜로 재미있어요!\r\n");	// quit 입력됐을 때 나가면서 출력
	return 1;	// 종료
}
