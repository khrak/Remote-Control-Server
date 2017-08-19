#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc , char *argv[])
{
    int server_socket, client_socket;
    int addr_length, read_size, buffer_size = 20;
    
    struct sockaddr_in server, client;
 
    char client_message[buffer_size];

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        printf("Could not create socket");
    }

    puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(2222);

    if (bind(server_socket, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("bind failed. Error");
 
        return 1;
    }

    puts("bind done");

    listen(server_socket , 3);

    puts("Waiting for incoming connections...");
 
    addr_length = sizeof(struct sockaddr_in);

    while (1)
    {
      client_socket = accept(server_socket, (struct sockaddr *) &client, (socklen_t*) &addr_length);

      if (client_socket < 0)
      {
          perror("accept failed");
    
          return 1;
      }

      puts("Connection accepted");

      while ((read_size = recv(client_socket , client_message , buffer_size , 0)) > 0)
      {
          printf("%s\n", client_message);
      }

      if (read_size == 0)
      {
          puts("Client disconnected");

          fflush(stdout);
      }
      else if(read_size == -1)
      {
          perror("recv failed");
      }
    }

    return 0;
}
