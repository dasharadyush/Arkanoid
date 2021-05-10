#include "my_server.h"

int main()
{
    int sock;
    struct sockaddr_un name = {0};
    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if (sock < 0)
    {
        perror("Error: unable to create socket!");
        return 1;
    }
    
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, PATH, sizeof(PATH));

    if (bind(sock, (struct sockaddr*)&name, sizeof(name)) < 0)
    {
        perror("Error: unable to bind socket!");
        close(sock);
        return 1;
    }
    
    if (listen(sock, 20))   
    {
        perror("Error: unable to listen socket!");
        close(sock);
        return 1;
    }
    
    while (1)
    {
        int client_sock;
        char buffer[BUFSZ] = {0};
        client_sock = accept(sock, NULL, NULL);
        
        if (client_sock < 0)
        {
            perror("Error: unable to accept socket!");
            exit(1);
        }
        
        if (read(client_sock, buffer, BUFSZ) < 0 || read(client_sock, buffer, BUFSZ) > BUFSZ)
        {
            perror("Error: unable to read socket!");
            exit(1);
        }
        
        close(client_sock);
        
        if (strncmp(buffer, "print", sizeof("print")-1) == 0) {
            printf("%s", buffer+sizeof("print"));
        }
        
        if (strncmp(buffer, "exit", sizeof("exit")-1) == 0) {
            break;
        }
    }
    unlink(PATH);
}

