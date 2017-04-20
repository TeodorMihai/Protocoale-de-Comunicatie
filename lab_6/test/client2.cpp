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
#include <bits/stdc++.h>

using namespace std;


int main()
{
	int sock;
	struct sockaddr_in server_addr , client_addr;
	char send_data[1024];

	unsigned addr_len;
	addr_len = sizeof(struct sockaddr);

	struct hostent *host;
	host= (struct hostent *) gethostbyname((char *)"127.0.0.1");


	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5000);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);

	while(1) {
		int bytes_read = 0;
		char recv_data[1024];
		memset(recv_data, 0, sizeof(recv_data));

		cout << "Wait for message:\n";
		bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr, &addr_len);
		cout << "S-a primit: " << recv_data << '\n'; 


		//prelucraare
		for(int i = 0 ; i < strlen(recv_data); ++i)
			recv_data[i] = toupper(recv_data[i]);
		cout << recv_data << '\n';
		sendto(sock, recv_data, 1024, 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));


	}


    /*
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
	*/
   return 0;
}
