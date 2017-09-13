#include "server.h"

int server_send_msg(char *target, char *message, zsock_t *router) {
  zmsg_t   *response = zmsg_new();
  zframe_t *identity = zframe_new(target, strlen(message));
  zframe_t *empty = zframe_new("", 0);
  zframe_t *content = zframe_new(message, strlen(message));

  zmsg_prepend(response, &identity);
  zmsg_append(response, &empty);
  zmsg_append(response, &content);
  zmsg_send(&response, router);
  zmsg_destroy(&response);
  zframe_destroy(&identity);
  zframe_destroy(&empty);
  zframe_destroy(&content);
  return (0);
}

int server_rcv_msg(zmsg_t *message) {
  zframe_t *identity = zmsg_pop(message);
  zframe_t *empty = zmsg_pop(message);
  zframe_t *content = zmsg_pop(message);

  zmsg_destroy(&message);
  printf("Content of message is : %s\n", zframe_strdup(content));
  return (0);
}

int listen_rep(t_conf conf) {
  zsock_t *router = zsock_new(ZMQ_ROUTER);
  zsock_bind(router, "tcp://*:%s", conf.rep_port);

  while (!zsys_interrupted) {
    zmsg_t *message = zmsg_recv(router);
    server_rcv_msg(message);
    //server_send_msg(message, router);
  }
  zsock_destroy(&router);
  return (0);
}