#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char const *argv[]) {

	struct hostent* host = NULL;//malloc( sizeof(struct hostent) );

	printf("%s\n", argv[1]);

	if(strchr(argv[1], 'n') != NULL) {

		host = gethostbyname(argv[2]);
		printf("Name: %s\n", host->h_name);

		printf("Address type: %d\n", host->h_addrtype);
	
		struct in_addr **addr = (struct in_addr** ) host->h_addr_list;
		printf("Ipuri: \n ");
		int i = 0;

		while(addr[i] != NULL) {

			struct in_addr* it = (struct in_addr *) addr[i]; 
			printf("%s\n", inet_ntoa(*it));
			++i;
		}


	} else 	{	
		struct in_addr x;
		inet_aton(argv[2], &x);

		host = gethostbyaddr(&x, sizeof(struct in_addr),  AF_INET);

		printf("Name: %s\n", host->h_name);
		if(host == NULL)
			printf("aici");
		
		printf("Address type: %d\n", host->h_addrtype);
	
		struct in_addr **addr = (struct in_addr** ) host->h_addr_list;
		printf("Ipuri: \n ");
		int i = 0;

		while(addr[i] != NULL) {

			struct in_addr* it = (struct in_addr *) addr[i]; 
			printf("%s\n", inet_ntoa(*it));
			++i;
		}	

	}


	return 0;
}