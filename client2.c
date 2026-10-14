#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128
#define KEY 5678

#define REQ_CLIENT2 20
#define RESP_CLIENT2 200

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
    buf.mtype = REQ_CLIENT2;
    strcpy(buf.mtext, "Hola Servidor, soy el Cliente 2");
    msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0);

    // Escucha ÚNICAMENTE el mtype canalizado para él (200)
    msgrcv(msqid, &buf, MSGSZ, RESP_CLIENT2, 0);
    printf("[Cliente 2] Respuesta recibida: %s\n", buf.mtext);

    return 0;
}