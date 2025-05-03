#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT      12345
#define BUF_SIZE  1024

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    char buf[BUF_SIZE];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket"); exit(1);
    }
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_port        = htons(PORT);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect"); exit(1);
    }

    while (1) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(sock, &fds);
        FD_SET(STDIN_FILENO, &fds);
        int mx = (sock > STDIN_FILENO ? sock : STDIN_FILENO) + 1;
        if (select(mx, &fds, NULL, NULL, NULL) < 0) {
            perror("select"); break;
        }

        if (FD_ISSET(sock, &fds)) {
            int n = recv(sock, buf, BUF_SIZE-1, 0);
            if (n <= 0) { printf("Disconnected from server.\n"); break; }
            buf[n] = '\0';
            printf("%s", buf);
        }
        if (FD_ISSET(STDIN_FILENO, &fds)) {
            if (!fgets(buf, BUF_SIZE, stdin)) break;
            send(sock, buf, strlen(buf), 0);
        }
    }

    close(sock);
    return 0;
}

