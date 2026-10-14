#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128
#define KEY 5678

#define REQ_CLIENT1 10
#define RESP_CLIENT1 100

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main(void) {
    int msqid;
    message_buf buf;

    if ((msqid = msgget(KEY, 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    // Envía solicitud al servidor
    buf.mtype = REQ_CLIENT1;
    strcpy(buf.mtext, "Hola Servidor, soy el Cliente 1");
    msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0);

    // Escucha ÚNICAMENTE el mtype canalizado para él (100)
    msgrcv(msqid, &buf, MSGSZ, RESP_CLIENT1, 0);
    printf("[Cliente 1] Respuesta recibida: %s\n", buf.mtext);

    return 0;
}