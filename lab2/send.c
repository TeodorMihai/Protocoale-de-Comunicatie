#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10000

const int block = 1000;

int main(int argc,char** argv){

  int sz = 0;

  init(HOST,PORT);
  msg t;

  FILE * pFile;
  pFile = fopen ("fisier.txt","r");

  fseek(pFile, 0, SEEK_END); 
  t.len = ftell(pFile);
  sz = t.len;
  fseek(pFile, 0, SEEK_SET);

  printf("send: %d bytes \n", t.len );
  send_message(&t);
  //trimit dimensiunea
  
  if (recv_message(&t) < 0)
    perror("Eroare la primire primul raspuns. ");
  else 
    printf("Mesajul intors de recv: %s\n ", t.payload);
  

  while(sz >= block) {

      t.len = fread(t.payload, 1, block, pFile);

      if( send_message(&t) < 0 )
        perror("Eroare trimititere bloc");
      
      sz -= block;
      recv_message(&t);
  }

  fread(t.payload, 1, sz, pFile);

  if( send_message(&t) < 0 )
    perror("Eroare la trimitere ultimul bloc");
  
  recv_message(&t);

  return 0;
}
