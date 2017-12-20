#include "pipe_networking.h"

/*=========================
  server_setup
  args:
  creates the WKP (upstream) and opens it, waiting for a
  connection.
  removes the WKP once a connection has been made
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  remove(WKP);
  printf("[serber] uhhh WKP made..\n");
  if (mkfifo(WKP, 0660) == -1) {
    print_errno();
    exit(1);
  }
  //if fd != -1, will lock program till afterwards
  int fd = open(WKP, O_RDONLY, 0);
  if (fd == -1){
    print_errno();
    exit(1);
  }
  printf("[serber] well known pipe is shopen\n");
  //remove the WKP so that it can be used for the next client.
  remove(WKP);
  return fd;
}


/*=========================
  server_connect
  args: int from_client
  handles the subserver portion of the 3 way handshake
  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
    char up_buffer[HANDSHAKE_BUFFER_SIZE];
    //first, read from the WKP (from_client):
    if (read(from_client, up_buffer, sizeof(up_buffer)) == -1) {
        print_errno();
    }
    //up_buffer contains the client pid
    printf("[serber-child] Received connection from PID %s\n", up_buffer);
    //open down pipe:
    int to_client = open(up_buffer, O_WRONLY);
    if (to_client == -1) {
        print_errno();
    }
    //make sure to remove the child pipe if it didnt do it already:
    remove(up_buffer);
    //ASSUME from_client and to_client work traditionally:
    //acknowledge the client, wait for it to acknowledge u back:
    if (write(to_client, ACK, HANDSHAKE_BUFFER_SIZE) == -1) {
        print_errno();
    }
    char down_buffer[HANDSHAKE_BUFFER_SIZE];
    if (read(from_client, down_buffer, sizeof(down_buffer)) == -1) {
        print_errno();
    }
    //validate that u got the right resposne back
    if (!strncmp(down_buffer, ACK, sizeof(down_buffer))) {
        printf("[serber-child] Received proper acknowledgement: %s\n", down_buffer);
    }
    else {
        fprintf(stderr, "ERROR: Received malformed acknowledgement \"%s\"\n", down_buffer);
        close(from_client);
        close(to_client);
    }

    return to_client;
}

/*=========================
  server_handshake
  args: int * to_client
  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {

  int from_client;

  char buffer[HANDSHAKE_BUFFER_SIZE];

  mkfifo(WKP, 0600);

  //block on open, recieve mesage
  printf("[server] handshake: making wkp\n");
  from_client = open( WKP, O_RDONLY, 0);
  read(from_client, buffer, sizeof(buffer));
  printf("[server] handshake: received [%s]\n", buffer);

  remove(WKP);
  printf("[server] handshake: removed wkp\n");

  //connect to client, send message
  printf("Forking subprocess to manage client\n");
  *to_client = open(buffer, O_WRONLY, 0);
  write(*to_client, buffer, sizeof(buffer));

  //read for client
  read(from_client, buffer, sizeof(buffer));
  printf("[server] handshake received: %s\n", buffer);

  return from_client;
}

/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

    int from_server;
    char buffer[HANDSHAKE_BUFFER_SIZE];

    //send pp name to server
    printf("[client] handshake: connecting to wkp\n");
    *to_server = open( WKP, O_WRONLY, 0);
    if ( *to_server == -1 ) {
        print_errno();
        exit(1);
    }


    //make private pipe
    sprintf(buffer, "%d", getpid() );
    mkfifo(buffer, 0600);

    write(*to_server, buffer, sizeof(buffer));

    //open and wait for connection
    from_server = open(buffer, O_RDONLY, 0);
    read(from_server, buffer, sizeof(buffer));
    /*validate buffer code goes here */
    printf("[client] handshake: received [%s]\n", buffer);

    //remove pp
    remove(buffer);
    printf("[client] handshake: removed private pipe\n");

    //send ACK to server
    write(*to_server, ACK, sizeof(buffer));

    return from_server;
}

int print_errno() {
    fprintf(stderr, "ERROR: %s\n", strerror(errno));
    return errno;
}
