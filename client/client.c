#include <czmq.h>

int main(int argc, char *argv[])
{
  char read[256];
  if (argc < 2) {
    printf("Port parameter is mandatory\n");
    return 0;
  }

  printf("Connecting..\n");
  zsock_t *req = zsock_new(ZMQ_REQ);
  zsock_connect(req, "tcp://localhost:%s", argv[1]);
  printf("Connected\n");


  while (!zsys_interrupted) {
    if (fgets(read, 256, stdin)) {
      zstr_sendf(req, read);
    }
    char *message = zstr_recv(req);
    printf("Received : %s", message);
    zstr_free(&message);
  }
  zsock_destroy(&req);
  return 0;
}
