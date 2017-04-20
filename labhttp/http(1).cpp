#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>  
#include <unistd.h>     
#include <sys/types.h>  

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include <netdb.h>
#include <sys/param.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <cstring>
#include <fstream>
#include <iostream>

using namespace std;

#define SMTP_PORT 80
#define MAXLEN 500



/**
 * Citeste maxim maxlen octeti din socket-ul sockfd. Intoarce
 * numarul de octeti cititi.
 */
ssize_t Readline(int sockd, char *vptr, size_t maxlen) {
    ssize_t n, rc;
    char    c, *buffer;

    buffer = vptr;

    for ( n = 1; n < maxlen; n++ ) {	
	if ( (rc = read(sockd, &c, 1)) == 1 ) {
	    *buffer++ = c;
	    if ( c == '\n' )
		break;
	}
	else if ( rc == 0 ) {
	    if ( n == 1 )
		return 0;
	    else
		break;
	} else {
	    if ( errno == EINTR )
		continue;
	    return -1;
	}
    }

    *buffer = 0;
    return n;
}

/**
 * Trimite o comanda SMTP si asteapta raspuns de la server.
 * Comanda trebuie sa fie in buffer-ul sendbuf.
 * Sirul expected contine inceputul raspunsului pe care
 * trebuie sa-l trimita serverul in caz de succes (de ex. codul
 * 250). Daca raspunsul semnaleaza o eroare se iese din program.
 */
void send_command(int sockfd, char sendbuf[], char *expected) {
  char recvbuf[MAXLEN];
  int nbytes;
  char CRLF[3];
  
  CRLF[0] = 13; CRLF[1] = 10; CRLF[2] = 0;
  strcat(sendbuf, CRLF);
  printf("Trimit: %s", sendbuf);
  write(sockfd, sendbuf, strlen(sendbuf));
  while( (nbytes = Readline(sockfd, recvbuf, MAXLEN - 1)) > 0 ) {
    recvbuf[nbytes] = 0;
    //cout << recvbuf;
    printf("Am primit: %s", recvbuf);
  }
  
  if (strstr(recvbuf, expected) != recvbuf) {
    //cout << recvbuf << 'x\n';
    printf("Am primit mesaj de eorare de la server!\n");
    exit(-1);
  }
}

int main(int argc, char **argv) {

  int sockfd;
  int port = SMTP_PORT;
  struct sockaddr_in servaddr;
  char server_ip[10];
  char sendbuf[MAXLEN]; 
  char recvbuf[MAXLEN];

  if (argc != 3) {
    printf("Utilizare: ./http adresaserver numefisier");
    exit(-1);
  }
  strcpy(server_ip, argv[1]);

  if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
	printf("Eroare la  creare socket.\n");
	exit(-1);
  }  

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  hostent * record = gethostbyname(argv[1]);

  in_addr * address = (in_addr * )record->h_addr;
  string ip_address = inet_ntoa(* address);
  cout << ip_address << '\n';
  
  if (inet_aton(&ip_address[0], &servaddr.sin_addr) <= 0 ) {
    printf("Adresa IP invalida.\n");
    exit(-1);
  }
  
  if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0 ) {
    printf("Eroare la conectare\n");
    exit(-1);
  }

  printf("M-am conectat\n");

  //Readline(sockfd, recvbuf, MAXLEN -1);
  //printf("Am primit: %s\n", recvbuf);

  sprintf(sendbuf, "GET %s HTTP/1.0\r\n\r\n", argv[2]);
  send_command(sockfd, sendbuf, "HTTP/1.1 200 OK");

  Readline(sockfd, recvbuf, MAXLEN -1);
  printf("Am primit: %s\n", recvbuf);
  
  close(sockfd);
}