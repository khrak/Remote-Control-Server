#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>

int main(int argc, char *argv[])
{
    int server_socket, client_socket;
    int length, read_size, port = 8888;
    int message_length = 1024;

    struct sockaddr_in server, client;
    
    char client_message[message_length + 1];
     
    server_socket = socket(AF_INET , SOCK_STREAM , 0);

    if (server_socket == -1)
    {
        printf("Could not create socket");
    }

    puts("Socket created");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    if (argc > 1)
    {
      std::stringstream stream(argv[1]);
 
      stream >> port;
    }

    server.sin_port = htons(port);
     
    if( bind(server_socket, (struct sockaddr *) &server, sizeof(server)) < 0)
    {
        perror("bind failed. Error");

        return 1;
    }

    puts("bind done");
     
    listen(server_socket , 3);
     
    puts("Waiting for incoming connections...");

    length = sizeof(struct sockaddr_in);
    
    while (1)
    {
      client_socket = accept(server_socket, (struct sockaddr *) &client, (socklen_t*) &length);
      
      if (client_socket < 0)
      {
          perror("accept failed");
      
          return 1;
      }

      puts("Connection accepted");
       
      while ((read_size = recv(client_socket , client_message , message_length , 0)) > 0)
      {
          client_message[read_size] = '\0';

          printf("Received: %s", client_message);

          char reply[] = "I'm from server\n";

          write(client_socket , reply , strlen(reply));

          memset(client_message, 0, read_size);
      }
       
      if(read_size == 0)
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