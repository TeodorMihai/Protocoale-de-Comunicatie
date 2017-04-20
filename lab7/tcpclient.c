#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define DATA_LENGTH 1024
#define LISTENING_PORT 5000

int  main(int argc, char const *argv[]) {
        
    printf("%d\n", argc);

    int client_socket;
    int bytes_recieved;  
    char send_data[DATA_LENGTH];
    char recv_data[DATA_LENGTH];
    struct hostent *host;
    struct sockaddr_in server_addr;  

    //initialize the socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    //get the IP for the specified host
    host = gethostbyname("localhost");

    //initialize the data for connection to a server
    server_addr.sin_family = AF_INET;  //TCP connection   
    server_addr.sin_port = htons(LISTENING_PORT); //server listening port
    server_addr.sin_addr = *((struct in_addr *)host->h_addr); //server address
    bzero(&(server_addr.sin_zero), 8); //clear the internal buffers

    //connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("Connect");
        exit(1);
    }

    //read data from the user

    int turn;

    if(argc == 1)
        turn = 1;
    else 
        turn = 2;

    while(turn) {

        if(turn == 1) {

            printf("\nMessage to send: ");
            scanf("%s", send_data);

            //send the data to the server
            send(client_socket, send_data, strlen(send_data), 0); 
            fflush(stdout);
            turn = 2;

        } else {

            bytes_recieved = recv(client_socket, recv_data, DATA_LENGTH, 0);
            recv_data[bytes_recieved] = 0;
            printf("\tRecieved %s\n" , recv_data);
            fflush(stdout);
            turn = 1;
        }

        //wait for the echo and print it to screen
        //bytes_reieved = recv(client_socket, recv_data, DATA_LENGTH, 0);
        //recv_data[bytes_recieved] = 0;
        //printf("\t\tRecieved data from the server = %s\n" , recv_data);
    } 

    close(client_socket);   

    return 0;
}
