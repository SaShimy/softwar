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
  
  player->player = game->players_length;
  getPos(game, player);
  if (player->pos_y == 0)
    {
      player->orientation = 3;
    }
  else
    {
      player->orientation = 1;
    }
  game->players[game->players_length] = *player;
  game->players_length += 1;
  printf("player created\n");
  if (game->players_length == 4) {
    game->game_status = 1;
  }
  showInfoUser(player);
  
  return (0);
}

t_return identify(char *data, t_game *game)
{
  int i;
  t_return ret;

  ret.data = "null";
  printf("ici\n");
  //printf("x:%d, , %d, %s", isValid(data), strlen(data), data);
  if (isValid(data) != 0)
    {
      printf("ici2\n");
      
      ret.code = 2;
      return (ret);//le data est invalide
    }
  printf("ici1\n");
  //for (i=0; game->players[i].id != NULL; i++)
  for(i=0; i < game->players_length; i++)
    {
      if (strcmp(data, game->players[i].id) == 0)
	{
	  printf("ici3\n");
	  
	  ret.code = 3;
	  return (ret);// ko|identity already exist
	}
    }
  if (i == 4)
    {
      printf("ici4\n");
      
      ret.code = 4;
      return (ret); // ko|game full
    }
  printf("la\n");
  ret.code = createPlayer(data,game);
  return (ret);
}
