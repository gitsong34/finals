#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>

struct msgbuf {
	long msgtype;
	char mtext[256];
	char myname[16];
	int  seq;
};

struct msgbuf1 {
	long msgtype;
	char ip[128];
	char host[16];
	int  seq;
};

int main(int argc, char **argv)
{
	key_t key_id;
	struct msgbuf mybuf;
	struct msgbuf1 mybuf1;
	int msgtype;

	if (argc == 2)
		msgtype = 3;
	else
		msgtype = 4;

	key_id = msgget(1234, IPC_CREAT|0666);
	if (key_id < 0) {
		perror("msgget error : ");
		return 0;
	}

	while(1) {
		if (msgtype == 3) {
			if (msgrcv( key_id, (void *)&mybuf1, sizeof(struct msgbuf1), msgtype, 0) == -    1)
			{
				perror("msgrcv error : ");
				return 0;
			}
			printf("%s %d\n",mybuf1.host, mybuf1.seq);
		} else {
			if (msgrcv( key_id, (void *)&mybuf, sizeof(struct msgbuf), msgtype, 0) == -1)
			{
				perror("msgrcv error : ");
				return 0;
			}
			printf("%s %d\n",mybuf.mtext, mybuf.seq);
		}
	}
	return 0;
}
