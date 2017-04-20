#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

const int block = 1000;

int main(int argc,char** argv) {
  msg r,t;

  int sz = 0;

  init(HOST,PORT);

  FILE * pFile;
  pFile = fopen ("nume.txt","w");

  if (recv_message(&r) < 0 ){
    perror("Receive dimension error. ");
    return -1;
  } else
     sz = r.len;
  

   printf("rec: %d bytes\n", sz);

   sprintf(t.payload, "ACK(%s)", r.payload);
   t.len = strlen(t.payload+1);
   send_message(&t);

  while(sz >= block) {

    recv_message(&r);

    fwrite(r.payload, 1, block, pFile);

    sprintf(t.payload,"ACK(%s)",r.payload);
    t.len = strlen(t.payload+1);
    send_message(&t);

    sz -= block;
  }

  if( recv_message(&r) < 0 )
    perror("Eroare la trimitere ulimul bloc");

  fwrite(r.payload, 1, sz, pFile);

  sprintf(t.payload,"ACK(%s)",r.payload);
  t.len = strlen(t.payload+1);
  send_message(&t);

  return 0;
}
