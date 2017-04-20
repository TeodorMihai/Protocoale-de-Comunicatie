#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct server_connection {

	char* ip;
	unsigned int port;
};

void parse_data(int argc, char* argv[], struct server_connection* serv_con) {

	char wrong_call[] = "Apelare necorespuncatoarea a serverului.\n";
	char expected_format[] = "Format asteptat: ./client ip_server port_server\n";

	if(argc != 3) {
		fprintf(stderr, "%s%s", wrong_call, expected_format);
		exit(EXIT_FAILURE);
	}


	serv_con->ip = malloc(strlen(argv[1]) * sizeof *serv_con->ip);
	strcpy(serv_con->ip, argv[1]);

	char* c = NULL;

	for(c = argv[2] ; *c != 0; ++c)
		if(isdigit(*c) == 0) {

			fprintf(stderr, "%s", wrong_call);
			fprintf(stderr, "%s", "Portul nu este un numar / este un numar negativ.\n");
			exit(EXIT_FAILURE);
		}

	serv_con->port = atoi(argv[2]);

	fprintf(stdout, "S-au citit cu succes ip-ul: %s si portul: %u\n", 
											 serv_con->ip,   serv_con->port);
}

int main(int argc, char* argv[]) {

	struct server_connection serv_con;
	parse_data(argc, argv, &serv_con);
	

	return EXIT_SUCCESS;
}