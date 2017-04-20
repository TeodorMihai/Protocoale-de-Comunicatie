#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int main()
{
	int sock;
	struct sockaddr_in server_addr;
	struct hostent *host;
	char send_data[1024];

	host= (struct hostent *) gethostbyname((char *)"127.0.0.1");


	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5000);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);


	FILE * pFile;
    pFile = fopen("fisier.txt","r");

    //trimit nume fisier
    sprintf(send_data, "%s", "fisier.txt");
    sendto(sock, send_data, strlen(send_data), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
    
    fseek(pFile, 0, SEEK_END); 
 	int len = ftell(pFile);
	sprintf(send_data,"%d", len);
	sendto(sock, send_data, strlen(send_data), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

	fseek(pFile, 0, SEEK_SET);

	while (len >= 1024) {

		printf("Pachet trimis:\n");
		fread(send_data, 1, 1024, pFile);
		len -= 1024;
		if ((strcmp(send_data , "q") == 0) || strcmp(send_data , "Q") == 0) {
		   break;
		}else {
		   sendto(sock, send_data, strlen(send_data), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
		}		 
   }

   fread(send_data, 1, len, pFile);
   sendto(sock, send_data, strlen(send_data), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

   return 0;
}
