#include "server.h"

int isValid(char *data)
{
  char str[4];
  
  if (strlen(data) != 5)
    {
      return (0);
    }

  strncpy(str, data, 3);
  str[3] = '\0';
  if (strcmp(str, "#0x") != 0)
    {
      return (0);
    }
  if (!isdigit(data[3]) || !isdigit(data[4]))
    {
      return (0);
    }
    return (1);
}

int getPos(t_game *game, char *pos)
{
  int i;
  int count;
  
  for (i=0; strcmp(game->players[i].id, "\0") != 0; i++)
    {
      if (strcmp(pos, "x") == 0)
	{	
	  if (game->players[i].pos_x == 0)
	    {
	      count++;
	    }
	}
      if (strcmp(pos, "y") == 0)
	{
	  if (game->players[i].pos_y == 0)
	    {
	      count++;
	    }
	}
    }
  if (count == 2)
    {
      return (game->conf.size - 1);
    }
  return (0);
}

int createPlayer(char *data, t_game *game)
{
  t_player *player;
  int i;

  player = malloc(sizeof(*player));
  if (!player)
    {
      return (4);
    }
  player->id = data;
  player->energy = 50;
  player->pos_x = getPos(game, "x");
  player->pos_y = getPos(game, "y");
  player->ap = 1;
  if (player->pos_x == 0)
    {
      player->orientation = 0;
    }
  else
    {
      player->orientation = 2;
    }
  for (i=0; strcmp(game->players[i].id, "\0") != 0;i++);
  game->players[2] = *player;
  return (1);
}

int identify(char *data, t_game *game)
{
  int i;

  if (isValid(data) == 0)
    {
      return (0);
    }
  for (i=0; strcmp(game->players[i].id, "\0") != 0;i++)
    {
      if (strcmp(data, game->players[i].id) == 0)
	{
	  return (2);// ko|identity already exist
	}
    }
  if (i == 4)
    {
      return (3); // ko|game full
    }

  return (createPlayer(data, game));
}
