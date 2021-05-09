#include "my_server.h"

int main()
{

    int sock;
    struct sockaddr_un name = {0};
    char buffer[BUFSZ] = {0};
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if (sock < 0)
    {
        perror("Error: unable to create socket!");
        exit(1);
    }

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, PATH, sizeof(PATH));

//connect
    if (connect(sock, (struct sockaddr*)&name, sizeof(name)) < 0)
    {
        perror("Error: unable to connect!");
        close(sock);
        return 1;
    }

//write
    if (write(sock, buffer, BUFSZ) < 0 || write(sock, buffer, BUFSZ) > BUFSZ)
    {
        perror("Error: unable to write!");
        exit(1);
    }

//read
    if (read(1, buffer, BUFSZ) < 0 || read(1, buffer, BUFSZ) > BUFSZ)
    {
        perror("Error: unable to read!");
        exit(1);
    }

    return 0;
}
