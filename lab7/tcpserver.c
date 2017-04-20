#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define DATA_LENGTH 1024
#define LISTENING_PORT 5000

int main()
{
    int server_socket;
    int client_socket;
    int bytes_recieved;
    char recv_data[DATA_LENGTH];       
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int sin_size;
    
    //initialize the socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    //initialize the connection data
    server_addr.sin_family = AF_INET; //TCP connection
    server_addr.sin_port = htons(LISTENING_PORT); //listening port
    server_addr.sin_addr.s_addr = INADDR_ANY; //accept connections on all network interfaces
    bzero(&(server_addr.sin_zero),8); //clear the internal buffers

    //bind the connection
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Unable to bind");
        exit(1);
    }

    //wait for maximum 5 concurrent users
    if (listen(server_socket, 5) == -1) {
        perror("Listen");
        exit(1);
    }
    
    printf("\nTCP server waiting for client on port %d\n", LISTENING_PORT);
    fflush(stdout);

    sin_size = sizeof(struct sockaddr_in);



    int client_socket2;

    int turn = 1;

    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size);

    printf("\tI got a connection from (%s , %d)\n", 
        inet_ntoa(client_addr.sin_addr), 
        ntohs(client_addr.sin_port)
    );

    client_socket2 = accept(server_socket, (struct sockaddr *)&client_addr, &sin_size);

    printf("\tI got a connection from (%s , %d)\n", 
        inet_ntoa(client_addr.sin_addr), 
        ntohs(client_addr.sin_port)
    );

    while(1) {            
        //read data, convert it to string and print it to screen

        if(turn == 1) {
            bytes_recieved = recv(client_socket, recv_data, DATA_LENGTH, 0);
            recv_data[bytes_recieved] = 0;


            printf("\t\tRECIEVED DATA = %s\n" , recv_data);            
            fflush(stdout);

            send(client_socket2, recv_data, strlen(recv_data), 0); 
            turn = 2;

        } else {

            bytes_recieved = recv(client_socket2, recv_data, DATA_LENGTH, 0);
            recv_data[bytes_recieved] = 0;

            printf("\t\tRECIEVED DATA = %s\n" , recv_data);            
            fflush(stdout);

            send(client_socket, recv_data, strlen(recv_data), 0);
            turn = 1;
        }
    }  

    //close the connection
    close(server_socket);

    return 0;
} 
