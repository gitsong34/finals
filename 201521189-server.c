#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#define MAXBUF 256

int main()
{
	struct cal
	{
		int num1;
		char oper;
		int num2;
	}ex;
	int ssock, csock;
	int clen;
	int result;
	char again;
	pid_t pid;
	struct sockaddr_in client_addr, server_addr;

	char rbuf[MAXBUF];
	if((ssock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		perror("socket error : ");
		exit(1);
	}
	printf("생성된 소켓의 번호는 %d 입니다.\n",ssock);
	clen = sizeof(client_addr);
	memset(rbuf, 0,MAXBUF);
	memset(&ex, 0, sizeof(ex));
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(3317);
	if(bind(ssock,(struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind error : ");
		exit(1);
	}
	if(listen(ssock,8) < 0)
	{
		perror("listen error : ");
		exit(1);
	}
	while(1)
	{
		csock = accept(ssock, (struct sockaddr *)&client_addr, &clen);
		printf("클라이언트 소켓의 번호는 %d 입니다. \n",csock);

		pid = fork();
		switch(pid)
		{
			case -1:
				perror("fork error : ");
				exit(0);
			case 0:
				if(read(csock, rbuf, MAXBUF) <= 0)
				{
					perror(" (From Server) read error by input string : ");
					exit(1);
				}
				printf("\n %s \n", rbuf);
				do{
					if(read(csock, &ex, sizeof(struct cal)) <= 0)
					{
						perror("(From Server) read expression error : ");
						exit(1);
					}
					switch(ex.oper)
					{
						case '+':
							result = ex.num1 + ex.num2;
							break;
						case '-':
							result = ex.num1 - ex.num2;
							break;
						case '*':
							result = ex.num1 * ex.num2;
							break;
						case '/':
							result = ex.num1 / ex.num2;
							break;
						default:
							printf("It's wrong operator !! \n");
							break;
					}
					if(write(csock, &result, sizeof(result)) <= 0)
					{
						perror("(From Server) write error by calculate expression : ");
						exit(1);
					}
					again = ' ';
					if(read(csock, &again, sizeof(again)) <= 0)
					{
						perror("(From Server) read error by countine value : ");
					}
					if(again != 'y')
					{
						printf("Connect terminated by wrong input...\n");
						break;
					}
				}while(again);
				close(csock);
				exit(1);
			default:
				close(csock);
				break;
		}
	}
	return 0;
}
