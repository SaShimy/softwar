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
    zsock_t *pub = thread->publisher;
    int cycle;
    t_game *game;


    game = thread->game;
    cycle = thread->game->conf->cycle;

    while (!zsys_interrupted) {
        zstr_sendm (pub, "softwar");
        zstr_sendf(pub, "Game status: %d", game->game_status);
        if (game->game_status == GAME_STARTED)
        {
            game->game_status = GAME_IN_PROGRESS;
            // TODO: SEND NOTIF GAME_STARTED
            zstr_sendm (pub, "softwar");
            zstr_send(pub, "Game just started");
        }
        if (game->game_status == GAME_IN_PROGRESS) {
            int j;
            for (j = 0; j < 4; j++) {
                if (game->players[j].energy >= 100) {
                    // TODO : SEND NOTIFICATION CLIENT_LOSE
                    zstr_sendm (pub, "softwar");
                    zstr_sendf(pub, "CLIENT WIN: %s", game->players[j].id);
                    game->players[j].alive = false;
                }
            }
            refresh_cycle(game);
            if (game->players_length < 2) {
                int i;
                for (i = 0; i < 4; i++) {
                    if (game->players[i].alive) {
                        // TODO : SEND NOTIFICATION CLIENT_WIN
                        zstr_sendm (pub, "softwar");
                        zstr_sendf(pub, "CLIENT WIN: %s", game->players[i].id);
                        break;
                    }
                    // TODO SEND NOTIFICATION GAME_END
                    game->game_status = GAME_FINISHED;
                    zstr_sendm (pub, "softwar");
                    zstr_sendf(pub, "Game END: %d", game->game_status);
                    pthread_exit(NULL);
                }
            }
//                for(int i = 0; i<4; i++) {
//                    printf("Player: %s | %d E\n", game->players[i].id, game->players[i].energy);
//                }
        }

        usleep(cycle);
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
    int i;
    for (i = 0; i < 4; i++) {
        if (game->players[i].alive) {

             if (game->players[i].energy > 2)
             {
                game->players[i].energy -= 2;
                game->players[i].ap = 1;
             } else {
                // TODO : SEND NOTIFICATION CLIENT_DEAD
                game->players[i].alive = false;
                game->players_length -= 1;
             }
        }
    }
    create_cell(game);
}
