#include "server.h"

void showInfoUser(t_player *player)
{
  printf("id: %s\nenergy: %d\npos_x: %d\npos_y: %d\nap:\
 %d\norientation: %d\nplayer: %d\n",
	 player->id, player->energy, player->pos_x, player->pos_y,
	 player->ap, player->orientation, player->player);
}

int isValid(char *data)
{
  char str[4];
  
  if (strlen(data) != 5)
    {
      return (1);//mvse longueur
    }

  strncpy(str, data, 3);
  str[3] = '\0';
  if (strcmp(str, "#0x") != 0)
    {
      return (2);//le debut n'est pas conforme
    }
  if (!isdigit(data[3]) || !isdigit(data[4]))
    {
      return (3);//nb 3/4 n'est pas un nb
    }
    return (0);
}

int getPos(t_game *game, t_player *player)
{
  int i;
  
  if (player->player == 1)
    {
      player->pos_x = 0;
      player->pos_y = 0;
    }
  else if (player->player == 2)
    {
      player->pos_x = game->conf->size - 1;
      player->pos_y = 0;
    }
  else if (player->player == 3)
    {
      player->pos_x = 0;
      player->pos_y = game->conf->size - 1;
    }
  else if (player->player == 4)
    {
      player->pos_x = game->conf->size - 1;
      player->pos_y = game->conf->size - 1;
    }
}

int createPlayer(char *data, t_game *game)
{
  t_player *player;
  int i;

  player = malloc(sizeof(*player));
  if (!player)
    {
      return (1);//err malloc
    }
  player->id = data;
  player->energy = 50;
  player->ap = 1;
  for (i=0; game->players[i].id != NULL; i++);
  game->players[i] = *player;
  player->player = i + 1;
  getPos(game, player);
  if (player->pos_y == 0)
    {
      player->orientation = 3;
    }
  else
    {
      player->orientation = 1;
    }
  showInfoUser(player);

  return (0);
}

int identify(char *data, t_game *game)
{
  int i;

  if (isValid(data) == 0)
    {
      return (2);//le data est invalide
    }
  for (i=0; game->players[i].id != NULL; i++)
    {
      if (strcmp(data, game->players[i].id) == 0)
	{
	  return (3);// ko|identity already exist
	}
    }
  if (i == 4)
    {
      return (4); // ko|game full
    }
  return (createPlayer(data, game));
}
