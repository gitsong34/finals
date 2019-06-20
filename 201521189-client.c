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
	};
	int ssock;
	int clen;
	struct cal ex;
	int result;
	char again;
	struct sockaddr_in server_addr;
	char sbuf[MAXBUF] = "I going to send an expression to you!!";
	if((ssock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		perror("socket error : ");
		exit(1);
	}
	clen = sizeof(server_addr);
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(3317);
	if(connect(ssock, (struct sockaddr *)&server_addr, clen) < 0)
	{
		perror("connect error : ");
		exit(1);
	}
	if(write(ssock, sbuf, MAXBUF) <= 0)
		perror("(From Client) write error by input string: ");
	do
	{
		printf("Plesae enter an expression : ");
		scanf("%d %c %d", &ex.num1, &ex.oper, &ex.num2);
		if(ex.oper != '+')
			if(ex.oper != '-')
				if(ex.oper != '*')
					if(ex.oper != '/')
					{
						printf("Worong operator!!!\n");
						break;
					}
		if(write(ssock, &ex, sizeof(struct cal)) <= 0)
		{
			perror("(From Client) write error by expression : ");
			exit(1);
		}
		memset(&ex, 0, sizeof(ex));
		if(read(ssock, &result, sizeof(result)) <= 0)
		{
			perror("(From Client) read error by calculate expression : ");
			exit(1);
		}
		printf("The result is : %d \n", result);
		again = ' ';
		printf("\nDo you want continue? (y/n) : ");
		scanf(" %c",&again);
		printf("\n");
		if(again == 'n')
		{
			printf("It Will be terminated... \n\n");
		}
		else if(again == 'y')
		{
		}
		else
		{
			printf("Wrong input !! \n");
			printf("It will be treminated ...\n");
			return 1;
		}
		if(write(ssock, &again, sizeof(again)) <= 0)
		{
			perror("(From client) write error by continue value : ");
			exit(1);
		}
	}while(again == 'y');
	close(ssock);
	return 0;
}
