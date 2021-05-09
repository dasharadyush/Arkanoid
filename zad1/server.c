#include "my_server.h"

int test_buffer(const char* buffer) {
    int error = 0;
    if (strcmp (buffer , "ls") == 0) {
        pid_t pd = fork();
        if (pd == 0) {
            execlp("ls", "ls", NULL);
            fflush(stdout);
        }
    } else if (buffer[0] == 'c' && buffer[1] == 'd') {

        if (buffer[2] == ' ') {
            error = chdir(buffer + 3);
        } else if (buffer[2] == '\0') {
            error = chdir("/");
        } else {
            error = -1;
        }

        if (error == -1) {
            printf("Can't do this! ");
        }

    } else {
        printf ("There is no : %s ", buffer);
    }
    return 0;
}

int main() {
    int socket;
    socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket < 0) {
        perror("Error: unable to create socket!");
        return 1;
    }
    struct in_addr in_ad = { inet_addr (MY_IP )};
    struct sockaddr_in name = { AF_INET, PORT, in_ad };

    struct sockaddr* name_ = (struct sockaddr*)&name;
    socklen_t sock_len = sizeof (struct sockaddr_in);

    if (bind(socket, name_,  sock_len) < 0) {
        perror("Error: unable to bind socket!");
        close(socket);
        return 1;
    }

    while(1) {

        char buffer[BUFSZ] = {0};

        if (recvfrom(socket , buffer , BUFSZ , 0 , name_ , &sock_len) == -1) {
            perror("Error : unable to read client! ");
            break;
        }

        if (strcmp(buffer, "CLOSE_SERVER") == 0) {
            break;
        }
       if (test_buffer(buffer) == -1) {
           break;
       }
    }
    close(socket);
    return 0;
}
