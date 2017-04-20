#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct server_conf {

	unsigned int port_server;
	FILE* users_config_file;
	FILE* static_shares_config_file;
};

struct user_info {

	char name[25];
	char pass[25];
};

struct file {

	char name[25];
	unsigned int size;
}

struct shared_files {

	struct user_info user;
	struct file* fis;
};

void parse_users_and_pass(FILE*, struct user_info**, int*);
void parse_data(int, char**, struct server_conf*);
void parse_shared_files(FILE*, struct shared_files*, int);


void parse_data(int argc, char* argv[], struct server_conf* server) {

	char wrong_call[] = "Apelare necorespuncatoarea a serverului.\n";
	char expected_format[] = "Format asteptat: ./server port_server user_config_file static_shares_config_file\n";

	if(argc != 4) {
		fprintf(stderr, "%s%s", wrong_call, expected_format);
		exit(EXIT_FAILURE);
	}

	char* c = NULL;

	for(c = argv[1] ; *c != 0; ++c)
		if(isdigit(*c) == 0) {

			fprintf(stderr, "%s", wrong_call);
			fprintf(stderr, "%s", "Portul nu este un numar / este un numar negativ.\n");
			exit(EXIT_FAILURE);
		}

	server->port_server = atoi(argv[1]);

	server->users_config_file = fopen(argv[2], "r"); 

	if(server-> users_config_file == NULL) {
		fprintf(stderr, "Eroare la deschiderea fisierului: %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	server->static_shares_config_file = fopen(argv[3], "r");

	if(server-> users_config_file == NULL) {
		fprintf(stderr, "Eroare la deschiderea fisierului: %s\n", argv[3]);
		exit(EXIT_FAILURE);
	}
	
	fprintf(stdout, "S-au citit cu succes portul: %u si fisierelere: %s %s\n", 
	 			server->port_server, argv[2], argv[3]);

}

void parse_users_and_pass(FILE* users_file, struct user_info** users, int *cnt) {

	int nr_users;
	int characters_per_line = 70;
	char line[characters_per_line];
	char name[25];
	char pass[25];
	fscanf(users_file, "%d", &nr_users);
	*users = malloc(nr_users * sizeof *users);

	if(users == NULL) {
		fprintf(stderr, "Allocation problem of users.\n");
		exit(EXIT_FAILURE);
	}

	int i;
	*cnt = 0;

	fgets(line, characters_per_line, users_file);

	for(i = 0 ; i < nr_users ; ++i) {

		if(fgets(line, characters_per_line, users_file) == NULL) {
			fprintf(stderr, "Numar de useri necorespunzator in fisierul users_config.\n");
			exit(EXIT_FAILURE);
		}

		char* it = line;

		while(*it == ' ') ++it;

		int len_name = 0;
		while(isalnum(*it)) {
			name[len_name] = *it;
			len_name++;
			++it;
			if(len_name > 24) break;
		}

		if(len_name > 24) {
			fprintf(stdout, "Nume user prea lung pe linia: %d. Nu e luat in considerare.\n", i + 1);
			continue;
		}

		name[len_name] = 0;

		while(*it == ' ') ++it;

		int len_pass = 0;

		while(isalnum(*it)) {
			pass[len_pass] = *it;
			len_pass++;
			++it;

			if(len_pass > 24) break;

		}

		if(len_pass > 24) {
			fprintf(stdout, "Parola user prea lunga pe linia: %d. Nu e luat in considerare.\n", i + 1);
			continue;
		}

		pass[len_pass] = 0;
		strcpy((*users)[*cnt].name, name);
		strcpy((*users)[*cnt].pass, pass);
		++*cnt;

	}
}

void parse_shared_files(FILE* config_file, struct shared_files **sh_files, int cnt) {
	
}

int main(int argc, char* argv[]) {

	int i, j;
	struct server_conf server;
	struct user_info* users = NULL;
	struct shared_files* sh_files = NULL;
	int cnt_users;

	parse_data(argc, argv, &server);
	parse_users_and_pass(server.users_config_file, &users, &cnt_users);
	parse_shared_files(server.static_shares_config_file, &sh_filesm, cnt_users);


	for(i = 0 ; i < cnt_users; ++i )
		fprintf(stdout, "%s %s\n", users[i].name, users[i].pass);


	return EXIT_SUCCESS;
}