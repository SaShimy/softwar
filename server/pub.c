#include "server.h"

void test (t_game *game, t_conf *conf) {

    zsock_t	*pub;
    pthread_t t;
    int ret;
    t_thread	*thread;

    thread = init_thread(game, conf);
//    pub = thread->publisher;

//    pub = init_pub(conf);
//    pub = zsock_new(ZMQ_PUB);
//    assert(pub);
//    printf("bind publisher to port %d\n", 4243);
//    zsock_bind(pub, "tcp://*:%d", 4243);
//    printf("publisher ready to publish on: %d\n", 4243);

    ret = pthread_create (&t,NULL,exec_pub, thread);
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
    int cycle;
       t_game *game;

       game = thread->game;
    cycle = thread->game->conf->cycle;


//    t_game *game = thread->game;
//    int first = 0;
//
//    while (game->game_status == 1) {
//        if (first == 0) {
//
//            first = 1;
//        }
//            zstr_sendm (pub, "testing");
//            zstr_send(pub, "This is a test");
//            zclock_sleep (1000);
//    }

//    printf("%d\n", thread->game->game_status);
        while (!zsys_interrupted) {
            zstr_sendm (pub, "softwar");
            zstr_sendf(pub, "%d", game->players_length);
//            zstr_sendm (pub, "softwar");
//            zstr_send(pub, "Cycle");
            if (game->game_status == GAME_STARTED)
            {
        	    game->game_status = GAME_IN_PROGRESS;
                // SEND NOTIF GAME STARTED
                zstr_sendm (pub, "testing");
                zstr_send(pub, "Game just started");
            }
            if (game->game_status == GAME_IN_PROGRESS) {
//                refresh_cycle();
//                zstr_sendm (pub, "softwar");
//                zstr_send(pub, "");
//
//                for(int i = 0; i<4; i++) {
//                    printf("Player: %s | %d E\n", game->players[i].id, game->players[i].energy);
//                }
            }

            usleep(cycle);
//                zclock_sleep (cycle);
        }
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


int init_pub_thread(t_game *game, t_conf *conf)
{
    t_thread *thread;
    pthread_t t;
    int ret;
    thread = init_thread(game, conf);

    ret = pthread_create (&t,NULL,exec_pub, thread->publisher);
    if (ret)
    {
        fprintf (stderr, "%s\n", strerror (ret));
        return (-1);
    }

//    pthread_join (t, NULL);

//    game();
    zsock_destroy(&thread->publisher);
    return (0);
}

void refresh_cycle(t_game *game)
{
    for (int i = 0; i < game->players_length; i++) {
        game->players[i].energy -= 2;

    }
}