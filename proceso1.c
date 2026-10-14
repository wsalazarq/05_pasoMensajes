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

    // Crea la cola de mensajes
    if ((msqid = msgget(key, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    // Envía "Are you hearing me?" (mtype = 1)
    sbuf.mtype = 1;
    strcpy(sbuf.mtext, "Are you hearing me?");
    printf("Process 1 envía: %s\n", sbuf.mtext);
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    // Espera la respuesta (mtype = 2)
    if (msgrcv(msqid, &rbuf, MSGSZ, 2, 0) < 0) {
        perror("msgrcv");
        exit(1);
    }
    printf("Process 1 recibe: %s\n", rbuf.mtext);

    // Envía "I can hear you too" (mtype = 1)
    sbuf.mtype = 1;
    strcpy(sbuf.mtext, "I can hear you too");
    printf("Process 1 envía: %s\n", sbuf.mtext);
    if (msgsnd(msqid, &sbuf, strlen(sbuf.mtext) + 1, 0) < 0) {
        perror("msgsnd");
        exit(1);
    }

    return 0;
}