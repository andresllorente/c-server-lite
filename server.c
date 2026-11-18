#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/sendfile.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void processRequest(int client_fd, char *buffer)
{
    memset(buffer, 0, 256);
    recv(client_fd, buffer, 256, 0);

    char requestedFile[256];
    char *start = strchr(buffer, '/');
    char *end   = strchr(buffer, ' ');

    if (start && end && end > start) 
    {
        int len = end - start - 1;
        if (len == 0)
            strcpy(requestedFile, "index.html");
        else
        {
            strncpy(requestedFile, start + 1, len);
            requestedFile[len] = '\0';
        }
    } 
    else 
    {
        strcpy(requestedFile, "index.html");
    }

    int file_fd = open(requestedFile, O_RDONLY);
    if (file_fd < 0)
    {
        const char *notFound =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<h1>404 Not Found</h1>";
        write(client_fd, notFound, strlen(notFound));
        return;
    }

    const char *header =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n";
    write(client_fd, header, strlen(header));

    struct stat st;
    fstat(file_fd, &st);
    off_t filesize = st.st_size;

    sendfile(client_fd, file_fd, NULL, filesize);

    close(file_fd);
}

int main()
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) { perror("socket"); exit(1); }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        perror("bind");
        exit(1);
    }

    listen(serverSocket, 5);
    printf("Listening on port %d...\n", PORT);

    while(1)
    {
        struct sockaddr_in clientAddr;
        socklen_t addrlen = sizeof(clientAddr);
        int client = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrlen);
        if(client < 0) { perror("accept"); continue; }

        char buffer[BUFFER_SIZE];
        processRequest(client, buffer);

        close(client);
    }

    close(serverSocket);
    return 0;
}
