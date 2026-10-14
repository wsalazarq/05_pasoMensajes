#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGSZ 128
#define KEY 5678

#define REQ_CLIENT1 10
#define REQ_CLIENT2 20

#define RESP_CLIENT1 100
#define RESP_CLIENT2 200

typedef struct msgbuf {
    long mtype;
    char mtext[MSGSZ];
} message_buf;

int main(void) {
    int msqid;
    message_buf buf;

    // Servidor crea la cola única
    if ((msqid = msgget(KEY, IPC_CREAT | 0666)) < 0) {
        perror("msgget");
        exit(1);
    }

    printf("Servidor iniciado. Esperando peticiones...\n");

    // Procesa peticiones de ambos clientes
    for (int i = 0; i < 2; i++) {
        // Lee cualquier mensaje de tipo > 0 (mtype 10 o 20)
        if (msgrcv(msqid, &buf, MSGSZ, -20, 0) < 0) {
            perror("msgrcv");
            exit(1);
        }

        if (buf.mtype == REQ_CLIENT1) {
            printf("[Servidor] Recibida petición del Cliente 1: %s\n", buf.mtext);
            buf.mtype = RESP_CLIENT1;
            strcpy(buf.mtext, "Mensaje privado para Cliente 1");
        } else if (buf.mtype == REQ_CLIENT2) {
            printf("[Servidor] Recibida petición del Cliente 2: %s\n", buf.mtext);
            buf.mtype = RESP_CLIENT2;
            strcpy(buf.mtext, "Mensaje privado para Cliente 2");
        }

        if (msgsnd(msqid, &buf, strlen(buf.mtext) + 1, 0) < 0) {
            perror("msgsnd");
            exit(1);
        }
    }

    printf("Servidor finalizó el procesamiento.\n");
    return 0;
}