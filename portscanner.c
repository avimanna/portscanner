#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>

void port_scanner(char *tIP, char *First_Port, char *Last_Port) {
    int start_port = atoi(First_Port);
    int end_port = atoi(Last_Port);

    for (int port = start_port; port <= end_port; port++) {
        struct addrinfo hints, *serv_addr, *temp;
        int sockfd, status;
        char tport[6];  // Port as string

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        snprintf(tport, sizeof(tport), "%d", port);

        status = getaddrinfo(tIP, tport, &hints, &serv_addr);
        if (status != 0) {
            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
            continue;
        }

        for (temp = serv_addr; temp != NULL; temp = temp->ai_next) {
            sockfd = socket(temp->ai_family, temp->ai_socktype, temp->ai_protocol);
            if (sockfd < 0) {
                continue;
            }

            status = connect(sockfd, temp->ai_addr, temp->ai_addrlen);
            if (status == 0) {
                printf("Port %d is OPEN\n", port);
                close(sockfd);
                break;
            }

            close(sockfd);
        }

        if (temp == NULL) {
            printf("Port %d is CLOSED\n", port);
        }

        freeaddrinfo(serv_addr);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Please enter the server IP address and range of ports to be scanned\n");
        printf("USAGE: %s IPv4 First_Port Last_Port\n", argv[0]);
        exit(1);
    }

    char tIP[16] = {0};
    strcpy(tIP, argv[1]);
    char First_Port[6] = {0};
    strcpy(First_Port, argv[2]);
    char Last_Port[6] = {0};
    strcpy(Last_Port, argv[3]);

    port_scanner(tIP, First_Port, Last_Port);
    return 0;
}
