#include "server.h"

zsock_t		*init_pub(t_conf *conf)
{
    zsock_t	*pub;

    pub = zsock_new(ZMQ_PUB);
    assert(pub);
    printf("bind publisher to port %d\n", conf->pub_port);
    zsock_bind(pub, "tcp://*:%d", conf->pub_port);
    printf("publisher ready to publish on: %d\n", conf->pub_port);

  return pub;
}

void *exec_pub(void *arg)
{
    t_thread *thread = (t_thread *)(arg);

    zsock_t *pub = thread->publisher;

    zstr_send (pub, "testing");
//        while (!zsys_interrupted) {
//                zstr_sendm (pub, "testing");
//                zstr_send(pub, "This is a test");
//                zclock_sleep (1000);
//        }
}


t_thread	*init_thread(t_game *game, t_conf *conf)
{
  t_thread	*thread;

  if ((thread = malloc(sizeof (*thread))) == NULL)
    {
      return (NULL);
    }
  thread->game = game;
  thread->publisher = init_pub(conf);
  return (thread);
}
