#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove(WKP);
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  int to_client;
  int from_client;
  char downstream[BUFFER_SIZE];
  while (1){
    from_client = server_setup();
    if (fork()){
      remove(WKP);
      close(from_client); }
    else{
      subserver(from_client); }
  }
}

void subserver(int from_client) {
  int to_client = server_connect(from_client);
  char buffer[1000];
  while (read(from_client, buffer, 1000)){
    printf("subserver: %d recieved %s \n", getpid(), buffer);
    process(buffer);
    write(to_client, buffer, 1000);
  }

}

//replace
void process(char * s) {
  return 0;


}
