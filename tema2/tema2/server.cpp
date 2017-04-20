#include <bits/stdc++.h>
#include <dirent.h>
#include <sys/types.h>

using namespace std;

struct User {

	int id;
	string name;
	string pass;

	User() { }
	User(string _name) : name(_name) { }
	User(int _id, string _name, string _pass) : id(_id) , name(_name), pass(_pass) { }

	bool operator<(const User user) const {
		return this->name < user.name;
	}
};

struct File {

	string name;
	unsigned int size;

	File() {}
	File(string _name) : name(_name) , size(0) { }
	File(string _name, int _size) : name(_name) , size(_size) { }
	File(char* _name, int sz) {
		name.assign(_name);
		size = sz;
	}

	bool operator<(const File file) const {
		return this->name < file.name;
	}
};


void parseConfig(int, char**, set<User>&, multimap<User, File>& , int);
void parseUserFile(ifstream&, set<User>&);
void parseShareFile(ifstream&, multimap<User, File>&, set<User>&);
void getServerFiles(set<File>&);
void printShareFiles(set<User>& , multimap<User, File> &);
void printUsers(set<User>&);


void parseUserFile(ifstream& userStream, set<User>& users) {

	int maxSize = 24;
	int cnt;

	userStream >> cnt;

	for(int i = 0 ; i < cnt ; ++i) {

		string name; string pass;

		userStream >> name >> pass;
		if(name.size() > maxSize) {
			cout << "Nume user prea lung pe linia: " << i + 1;
			cout << ". Nu e luat in considerare.\n" ;
			continue;
		}

		if(pass.size() > maxSize) {
			cout << "Parola user prea lunga pe linia: " << i + 1;
			cout << ". Nu e luat in considerare.\n" ;
			continue;
		}	

		if(users.find(User(cnt, name, pass)) != users.end()) {

			cout << "Nume user intalnit de doua ori pe linia: " << i + 1 <<  ". Se ignora\n";
			continue;
		}

		users.insert({cnt, name, pass});		
	} 
}
void parseConfig(int argc, char* argv[], set<User>& users, multimap<User, File>& userFiles, int* port) {

	string wrong_call = "Apelare necorespuncatoarea a serverului.\n";
	string expected_format = "Format asteptat: ./server port_server user_config_file static_shares_config_file\n";

	if(argc != 4) {
		cout << wrong_call << expected_format ;
		exit(EXIT_FAILURE);
	}

	for(char* c = argv[1] ; *c != 0; ++c)
		if(isdigit(*c) == 0) {

			cout << wrong_call;
			cout << "Portul nu este un numar / este un numar negativ.\n";
			exit(EXIT_FAILURE);
		}

	*port = atoi(argv[1]);
	
	ifstream userStream;
	userStream.open(argv[2], fstream::in);

	if(userStream.is_open() == false) {
		cout << "Nu s-a putut deschide fisierul de configurare cu userii.\n";
		exit(EXIT_FAILURE);
	}

	parseUserFile(userStream, users);
	userStream.close();

	ifstream shareStream;
	shareStream.open(argv[3], fstream::in);

	if(shareStream.is_open() == false) {
		cout << "Nu s-a putut deschide fisierul de configurare cu fisierele partajate.\n";
		exit(EXIT_FAILURE);
	}

	parseShareFile(shareStream, userFiles, users); 
}
void parseShareFile(ifstream& shareStream, multimap<User, File>& userFiles, set<User>& users) {
	
	int nrFiles ;
	shareStream >> nrFiles;
	set<File> serverFiles;


	char userName[25];
	char fileName[100];

	getServerFiles(serverFiles);

	shareStream.getline(fileName, 100, '\n');

	for(int i = 0 ; i < nrFiles ; ++i) {

		shareStream.getline(userName, 24, ':');
		shareStream.getline(fileName, 100, '\n');

		string userStr(userName);
		string fileStr(fileName);

		if(users.find(User(userStr)) == users.end()) {
			cout << "Nu exista niciun user in sistem cu acest nume. Linia: " << i + 1  <<  " . Se ignora." << '\n';
			continue;
		}

		if(serverFiles.find(File(fileName)) == serverFiles.end()) {
			char error[100];
			sprintf(error, "Nu s-a gasit fisierului userului de pe linia: %d", i + 1);
			perror(error);
			continue;
		}

		User user = *users.find(User(userStr));
		userFiles.insert({user, File(fileStr)});
	}
}
void printUsers(set<User>& users) {

	cout << "Utilizatori:\n";

	for(User user : users)
		cout << user.name << ' ' << user.pass << ' ' << user.id << '\n';
}
void printShareFiles(set<User>& users, multimap<User, File>& userFiles) {

	cout <<"Files:\n";
	for(User u : users) {

		const auto range = userFiles.equal_range(u);

		for(auto it = range.first; it != range.second; ++it)
			cout << it->first.name << ' ' << it->second.name << '\n'; 
	}
}
void printFiles(set<File>& files) {

	for(File file : files) 
		cout << file.name << '\n'; 
}
void getServerFiles(set<File>& filesOnServer) {

	/* http://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c */

	DIR *dir;
	struct dirent *ent;

	if( (dir = opendir(".")) != NULL) {
	  /* print all the files and directories within directory */

	  while((ent = readdir(dir)) != NULL) 	
	  	filesOnServer.insert(File(ent->d_name));

	  closedir(dir);
	} else {
	  perror("Nu s-a putut deschide directorul cu fisieruele serverului.");
	  exit(EXIT_FAILURE);
	}
}

int main(int argc, char *argv[]) {

	set<User> users;
	multimap<User, File> userFiles;
	set<File> filesOnServer;
	int port;
	
	getServerFiles(filesOnServer);
	parseConfig(argc, argv, users, userFiles, &port);

	cout << port << '\n'; 
	return EXIT_SUCCESS;
}