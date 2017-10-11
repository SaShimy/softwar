#include "server.h"

int listen_pub(t_conf conf) {
  zsock_t *pub = zsock_new(ZMQ_PUB);
  zsock_bind(pub, "tcp://*:%d", conf.pub_port);

  return (0);
}