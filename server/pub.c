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
    char *json;
    game = thread->game;
    cycle = thread->game->conf->cycle;

    while (!zsys_interrupted) {
        zstr_sendm (pub, "softwar");
        zstr_sendf(pub, "%s", game_info(game));

        if (game->game_status == GAME_STARTED)
        {
            game->game_status = GAME_IN_PROGRESS;
//            json = game_started();
            // SEND NOTIF GAME_STARTED
            zstr_sendm (pub, "softwar");
            zstr_sendf(pub, "%s", notification_game_started());
        }
        if (game->game_status == GAME_IN_PROGRESS) {
            int j;
            for (j = 0; j < 4; j++) {
                if (game->players[j].energy >= 100) {
                    // SEND NOTIFICATION CLIENT_LOSE
                    zstr_sendm (pub, "softwar");
                    zstr_sendf(pub, "%s", notification_client_lose(game->players[j].id));
//                    zstr_sendm (pub, "softwar");
//                    zstr_sendf(pub, "CLIENT WIN: %s", game->players[j].id);
                    game->players[j].alive = false;
                }
            }
            refresh_cycle(game);
            if (game->players_length < 2) {
                int i;
                for (i = 0; i < 4; i++) {
                    if (game->players[i].alive) {
                        // SEND NOTIFICATION CLIENT_WIN
                        zstr_sendm (pub, "softwar");
                        zstr_sendf(pub, "%s", notification_client_win(game->players[i].id));
//                        zstr_sendm (pub, "softwar");
//                        zstr_sendf(pub, "CLIENT WIN: %s", game->players[i].id);
                        break;
                    }
                }
                game->game_status = GAME_FINISHED;
                zstr_sendm (pub, "softwar");
                zstr_sendf(pub, "%s", notification_game_finished());
                pthread_exit(NULL);
            }
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
    if (!ret)
    {
        fprintf (stderr, "%s\n", strerror (ret));
        return (-1);
    }
    zsock_destroy(&thread->publisher);
    return (0);
}

void refresh_cycle(t_game *game)
{
    int i;
    for (i = 0; i < 4; i++) {
        if (game->players[i].alive == true) {

             if (game->players[i].energy > 2)
             {
                game->players[i].energy -= 2;
                game->players[i].ap = 1;
             } else {
                // TODO : SEND NOTIFICATION CLIENT_DEAD
                printf("%s\n", game->players[i].id);
                notification_client_lose(game->players[i].id);
                game->players[i].alive = false;
                game->players_length -= 1;
             }
        }
    }
    create_cell(game);
}

char  *notification_game_started()
{
    json_t *json;
    char    *buf;

    json = json_pack("{s:i,s:s}", "notification_type", 1, "data", "null");

    size_t size = json_dumpb(json, NULL, 0, 0);

    buf = malloc (sizeof (char) * size);

    size = json_dumpb(json, buf, size, 0);

    return buf;
}

char  *notification_game_finished()
{
    json_t *json;
    char    *buf;

    json = json_pack("{s:i,s:s}", "notification_type", 2, "data", "null");

    size_t size = json_dumpb(json, NULL, 0, 0);

    buf = malloc (sizeof (char) * size);

    size = json_dumpb(json, buf, size, 0);

    return buf;
}

char  *notification_client_lose(char *id)
{
    json_t *json;
    char    *buf;

    json = json_pack("{s:i,s:s}", "notification_type", 3, "data", id);

    size_t size = json_dumpb(json, NULL, 0, 0);

    buf = malloc (sizeof (char) * size);

    size = json_dumpb(json, buf, size, 0);

    return buf;
}

char  *notification_client_win(char *id)
{
    json_t *json;
    char    *buf;

    json = json_pack("{s:i,s:s}", "notification_type", 4, "data", id);

    size_t size = json_dumpb(json, NULL, 0, 0);

    buf = malloc (sizeof (char) * size);

    size = json_dumpb(json, buf, size, 0);

    return buf;
}

char *game_info(t_game *game)
{
    int i;
    char *buffer;
    char *players;
    char *map;
    int j;
    json_t *json;
//
//



    if (game->game_status == GAME_IN_PROGRESS) {
        players = "[";
        for (i = 0; i < 4; i++) {
//            printf("PLAYER: %s\n", player_info(game->players[i]));
            players = concat(players, player_info(game->players[i]));
            if (i < 3)
                players = concat(players, ", ");
        }
        players = concat(players, "]");
    }

    json = json_pack("{s:i,s:i,s:i,s:s}", "notification_type", 0, "map_size", game->conf->size, "game_status", game->game_status, "players", players);

    size_t size = json_dumpb(json, NULL, 0, 0);

    buffer = malloc (sizeof (char) * size);

    size = json_dumpb(json, buffer, size, 0);

    return buffer;
}

char *player_info(t_player player)
{
    char *buffer;
    json_t *json;

    json = json_pack("{s:s,s:i,s:i,s:i,s:i}", "name", player.id, "x", player.pos_x, "y", player.pos_y, "energy", player.energy, "looking", player.orientation);

    size_t size = json_dumpb(json, NULL, 0, 0);

    buffer = malloc (sizeof (char) * size);

    size = json_dumpb(json, buffer, size, 0);

    return buffer;
}

char *cell_info(t_cell *cell)
{
    char *buffer;
    json_t *json;

    json = json_pack("{s:i,s:i,s:i}", "x", cell->x, "y", cell->y, "value", cell->value);

    size_t size = json_dumpb(json, NULL, 0, 0);

    buffer = malloc (sizeof (char) * size);

    size = json_dumpb(json, buffer, size, 0);

    return buffer;
}