#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
 
struct msgbuf
{
	long msgtype;
	char mtext[256];
	char myname[16];
	int  seq;
};

struct msgbuf1
{
	long msgtype;
	char ip[128];
	char host[16];
	int  seq;
};


int main(void)
{
	key_t key_id;
	int i;
	struct msgbuf mybuf;
	struct msgbuf1 mybuf1;

	key_id = msgget((key_t)1234, IPC_CREAT|0666);
	if (key_id == -1)
	{
		perror("msgget error : ");
		return 0;
	}
	printf("Key is %d\n", key_id);

	mybuf.seq = 0;
	i = 0;
	while (1) {
		memset(&mybuf, 0, sizeof(struct msgbuf));
		memset(&mybuf1, 0, sizeof(struct msgbuf1));

		if (i % 2 == 0) {
			mybuf.msgtype = 1;
			snprintf(mybuf.mtext, sizeof(mybuf.mtext), "hello [%d]",i);
			snprintf(mybuf.myname, sizeof(mybuf.myname), "name [%d]",i);
			mybuf1.seq = i;
			mybuf.seq = i;

			if (msgsnd( key_id, (void *)&mybuf, sizeof(struct msgbuf), IPC_NOWAIT) == -1)
			{
				perror("msgsnd error : ");
				return 0;
			}
		} else {
			mybuf1.msgtype = 3;
			snprintf(mybuf1.ip, sizeof(mybuf1.ip), "10.0.8.%d",i);
			snprintf(mybuf1.host, sizeof(mybuf1.host), "ip[%d]",i);
			mybuf1.seq = i;

			if (msgsnd( key_id, (void *)&mybuf1, sizeof(struct msgbuf1), IPC_NOWAIT) == -    1)
			{
				perror("msgsnd error : ");
				return 0;
			}
		}
		printf("send %d\n", i);
		i++;
		sleep(1);
	}
	return 0;
}
