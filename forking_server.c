#include "pipe_networking.h"
#include <signal.h>

void process(char *s);
void subserver(int from_client);

static void sighandler(int signo) {
  if (signo == SIGINT) {
    remove(WKP;
    exit(0);
  }
}

int main() {
  int to_client;
  int from_client;
  char downstream[BUFFER_SIZE];
  while (1){
    from_client = server_handshake(&to_client);
    while (read (from_client, downstream, BUFFER_SIZE)){
      printf()
    }



  }


}

void subserver(int from_client) {
  int to_client;
  int from_client;

}

//replace
void process(char * s) {



}
