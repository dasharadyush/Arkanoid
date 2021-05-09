#include "my_server.h"

int main()
{

    int socket;
    struct sockaddr_un name = {0};
    char buffer[BUFSZ] = {0};
    socket = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if (socket < 0)
    {
        perror("Error: unable to create socket!");
        exit(1);
    }

    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, PATH, sizeof(PATH));

//connect
    if (connect(socket, (struct sockaddr*)&name, sizeof(name)) < 0)
    {
        perror("Error: unable to connect!");
        close(socket);
        return 1;
    }

//write
    if (write(socket, buffer, BUFSZ) < 0 || write(socket, buffer, BUFSZ) > BUFSZ)
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