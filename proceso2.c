#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main(void) {
    int msqid;
    key_t key = 1234;
    message_buf sbuf, rbuf;

    // Conecta a la cola existente
    if ((msqid = msgget(key, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    // Recibe el primer mensaje (mtype = 1)
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    printf("Process 2 recibe: %s\n", rbuf.mtext);

    // Responde "Loud and clear" (mtype = 2)
    sbuf.mtype = 2;
    strcpy(sbuf.mtext, "Loud and clear");
    printf("Process 2 envía: %s\n", sbuf.mtext);
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    // Recibe el mensaje (mtype = 1)
    if (msgrcv(msqid, &rbuf, MSGSZ, 1, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    printf("Process 2 recibe: %s\n", rbuf.mtext);

    return 0;
}