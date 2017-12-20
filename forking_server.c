#include "pipe_networking.h"
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
    //parent:
    if (fork()) {
        close(to_client);
        close(from_client);
    }
    //child:
    else{
      subserver(from_client);
      exit(0);
    }
  }
}

void subserver(int from_client) {
  int to_client = server_connect(from_client);
  char buffer[BUFFER_SIZE];
  while (read(from_client, buffer, BUFFER_SIZE)){
    printf("[serber-child pid=%d] recieved %s \n", getpid(), buffer);
    process(buffer);
    write(to_client, buffer, BUFFER_SIZE);
  }

}

//replace
void process(char * s) {
    while (*s) {
        if (*s == 'C') *s = 'B';
        if (*s == 'c') *s = 'b';
        s++;
    }
}
