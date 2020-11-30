#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
// 30 라인이 훌쩍 넘는데요 ㅠㅠㅠ
int main(void)
{
	int fd = 0;
	int forwardBackward = 0;
	int step = 0;
	char buf = 0;

	while(1)
	{
		printf("Forward 1 or -1 ? ");	
		scanf("%d", &forwardBackward);
		printf("Step? ");	
		scanf("%d", &step);

		if( (forwardBackward == 1 || forwardBackward == -1)
			&& (step > 0) 
			&& (step <5) )
		{
			fd = open("./input.txt", O_RDONLY);

			if(fd < 0)
			{
				printf("file open error\n");
				return -1;
			}
			
			if(forwardBackward == 1) 
				lseek(fd, 0, SEEK_SET);

			else if(forwardBackward == -1) // else는 생략. why? 위에서 잡아주겠다고 믿음...
				lseek(fd, -1, SEEK_END);

			while(read(fd, (void*) &buf, sizeof(buf)) > 0)
				{
					printf("%c", buf);

					if (lseek(fd, (forwardBackward*step)-1, SEEK_CUR) == -1)
						break;
				}
			
			close(fd);
			return 0;
			}

		else
			continue;
	}
}

