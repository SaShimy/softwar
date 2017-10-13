
#include "server.h"

t_actions actions[9] = {
  {"leftfwd", leftfwd},
  {"rightfwd", rightfwd},
  {"forward", forward},
  {"backward", backward},
  {"right", right},
  {"left", left},
  {"looking", looking},
  {"selfid", selfid},
  {"selfstats", selfstats},
};

int server_send_msg(char *target, char *message, zsock_t *router) {
  zmsg_t   *response = zmsg_new();
  zframe_t *identity = zframe_new(target, strlen(target));
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

int server_rcv_msg(zmsg_t *message, t_game *game, zsock_t *router)
{
  char *identity = zmsg_popstr(message);
  char *empty = zmsg_popstr(message);
  char *content = zmsg_popstr(message);
  char *action = strtok(content, "|");
  char *data = strtok(NULL, "|");
  printf("%s\n", action);
  printf("%s\n", data);
  int i;
  t_player *current_player;
  bool done;

  done = false;
  zmsg_destroy(&message);
  if (game->game_status == 0)
  {
    if (strcmp("identify", action) == 0)
    {
      identify(identity, game);
    }
  } else if (game->game_status == 1)
  {
    for (i = 0; i < 4; i++)
    {
      if (strcmp(identity, game->players[i].id) == 0)
      {
        current_player = &game->players[i];
        break;
      }
    }
    if (current_player->id != game->players[i].id)
    {
      return (1); // No player with this identity
    }
    for (i = 0; i < 9; i++)
    {
      if (strcmp(action, actions[i].name) == 0)
      {
        printf("Forward: %d\n", actions[i].func(current_player, game->conf->size, "test", game).code);
        done = true;
      }
    }
    if (done == false)
    {
      return (2); // No action
    }
  }
  return (server_send_msg(identity, "ok|null", router));
}

int listen_rep(t_conf *conf, t_game *game) {
  zsock_t *router = zsock_new(ZMQ_ROUTER);
  zsock_bind(router, "tcp://*:%d", conf->rep_port);

  while (!zsys_interrupted) {
    zmsg_t *message = zmsg_recv(router);
    server_rcv_msg(message, game, router);
    // server_send_msg(message, router);
  }
  zsock_destroy(&router);
  return (0);
}
