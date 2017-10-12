#include "server.h"

void test (t_game *game, t_conf *conf) {

    zsock_t	*pub;
    pthread_t t;
    int ret;
    t_thread	*thread;

    thread = init_thread(game, conf);
    pub = thread->publisher;
//    pub = init_pub(conf);
//    pub = zsock_new(ZMQ_PUB);
//    assert(pub);
//    printf("bind publisher to port %d\n", 4243);
//    zsock_bind(pub, "tcp://*:%d", 4243);
//    printf("publisher ready to publish on: %d\n", 4243);

    ret = pthread_create (&t,NULL,exec_pub, pub);
    fprintf (stderr, "%s\n", strerror (ret));

    listen_rep(conf, game);
//    pthread_join (t, NULL);

    zsock_destroy(&pub);
}

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
    t_thread	*thread = (t_thread *)(arg);

//    zsock_t *pub = (zsock_t *)(arg);
    zsock_t *pub = thread->publisher;


    while (!zsys_interrupted) {
            zstr_sendm (pub, "testing");
            zstr_send(pub, "This is a test");
            zclock_sleep (1000);
    }
//        while (!zsys_interrupted) {
//                zstr_sendm (pub, "testing");
//                zstr_send(pub, "This is a test");
//                zclock_sleep (1000);
//        }
}

void *test_exec_pub(void *arg)
{
    zsock_t *pub = (zsock_t *)(arg);
    while (!zsys_interrupted) {
        zstr_sendm (pub, "testing");
        zstr_send(pub, "This is a test");
        zclock_sleep (1000);
    }
    pthread_exit(NULL);
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


void init_pub_thread(t_game *game, t_conf *conf)
{
    t_thread *thread;
    pthread_t t;
    int ret;
    thread = init_thread(game, conf);

    ret = pthread_create (&t,NULL,exec_pub, thread->publisher);

    if (!ret)
    {
        fprintf (stderr, "%s\n", strerror (ret));
    }
    else {
        fprintf (stderr, "%s\n", strerror (ret));
    }
    pthread_join (t, NULL);
    zsock_destroy(&thread->publisher);
}