#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <bits/stdc++.h>

#include <netdb.h>

using namespace std;

ifstream fin("date.in");
ofstream fout("date.out");

int main()
{
	int sock;
	unsigned int addr_len;
	int bytes_read;
	char recv_data[1024];
	struct sockaddr_in server_addr , client_addr1, client_addr2;

	struct hostent *host;
	host= (struct hostent *) gethostbyname((char *)"127.0.0.1");


	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(5000);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);


	if (bind(sock,(struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1){
		perror("Bind");
		exit(1);
	}

	addr_len = sizeof(struct sockaddr);

	printf("\nUDPServer Waiting for client on port 5000\n");
	fflush(stdout);


	char word[1024];
	memset(word, 0, sizeof(word));
	
	while( (fin >> word) ) {
		fout << word << ' ';

		sendto(sock, word, strlen(word), 0, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
		cout << "send data: " << word << " wait for response:\n";

		memset(recv_data, 0, sizeof(recv_data));
		bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr1, &addr_len);
		recv_data[bytes_read] = '\0';

		cout << "Response client 1: " << recv_data << '\n';
		fout << recv_data << ' ';

		memset(recv_data, 0, sizeof(recv_data));
		bytes_read = recvfrom(sock,recv_data,1024,0,(struct sockaddr *)&client_addr2, &addr_len);
		recv_data[bytes_read] = '\0';

		cout << "Response client 2: " << recv_data << '\n';
		fout << recv_data << '\n'; 
	}

	return 0;
}
