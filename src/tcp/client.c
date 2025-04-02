#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdint.h> // For uint16_t, uint32_t

#define PORT 8080
#define BUFFER_SIZE 1024

// Custom IPv4 address structure (same as in_addr)
typedef struct
{
    int s_addr;
} my_in_addr;

// Custom sockaddr_in equivalent
typedef struct
{
    short sin_family;
    short sin_port;
    my_in_addr sin_addr;
    unsigned char sin_zero[8];
} my_sockaddr_in;

int main()
{
    int sock = 0;
    my_sockaddr_in mine;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Initialize custom struct
    memset(&mine, 0, sizeof(mine)); // Clear sin_zero
    mine.sin_family = AF_INET;
    mine.sin_port = htons(PORT);

    // Convert IP address directly into our custom struct
    if (inet_pton(AF_INET, "127.0.0.1", &mine.sin_addr.s_addr) <= 0)
    {
        perror("invalid address / address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect using our custom struct (cast to sockaddr*)
    if (connect(sock, (struct sockaddr *)&mine, sizeof(mine)) < 0)
    {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }

    // Read server response
    read(sock, buffer, BUFFER_SIZE);
    printf("Server says: %s", buffer);

    close(sock);
    return 0;
}