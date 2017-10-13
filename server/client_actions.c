#include "server.h"

t_return leftfwd(t_player *player, int max, char *data, t_game *game)
{
  t_return ret;
  int init_x;
  int init_y;

  init_x = player->pos_x;
  init_y = player->pos_y;
  ret.data = "null";
  if (player->ap < 1)
    {
      ret.code = 1;
      return (ret);
    }
  player->orientation -= 1;
  if (player->orientation < 0)
    {
      player->orientation = 3;
    }
  if ((player->orientation == 1 && player->pos_y - 1 < 0) ||
      (player->orientation == 3 && player->pos_y + 1 > max) ||
      (player->orientation == 0 && player->pos_x - 1 < 0) ||
      (player->orientation == 2 && player->pos_x + 1 > max))
    {
      ret.code = 2;
      return (ret);//ne peut pas avancer
    }
  if (player->orientation % 2 == 0)
    {
      player->pos_x += player->orientation == 0 ? -1 : 1;
    }
  else
    {
      player->pos_y += player->orientation == 1 ? -1 : 1;
    }
  if(is_player(game->players, player->pos_x, player->pos_y))
    {
      player->pos_x = init_x;
      player->pos_y = init_y;
      ret.code = 3;
      return (ret);
    }
  ret.code = 0;
  player->ap -= 1;
  return (ret);
}

t_return rightfwd(t_player *player, int max, char *data, t_game *game)
{
  t_return ret;
  int init_x;
  int init_y;

  init_x = player->pos_x;
  init_y = player->pos_y;
  ret.data = "null";
  if (player->ap < 1)
    {
      ret.code = 1;
      return (ret);
    }
  player->orientation += 1;
  if (player->orientation > 3)
    {
      player->orientation = 0;
    }
  if ((player->orientation == 1 && player->pos_y - 1 < 0) ||
      (player->orientation == 3 && player->pos_y + 1 > max) ||
      (player->orientation == 0 && player->pos_x - 1 < 0) ||
      (player->orientation == 2 && player->pos_x + 1 > max))
    {
      ret.code = 2;
      return (ret);//ne peut pas avancer
    }
  if (player->orientation % 2 == 0)
    {
      player->pos_x += player->orientation == 0 ? -1 : 1;
    }
  else
    {
      player->pos_y += player->orientation == 1 ? -1 : 1;
    }
  if(is_player(game->players, player->pos_x, player->pos_y))
    {
      player->pos_x = init_x;
      player->pos_y = init_y;
      ret.code = 3;
      return (ret);
    }
  ret.code = 0;
  player->ap -= 1;
  return (ret);
}

t_return looking(t_player *player, int max, char *data, t_game *game)
{
  t_return ret;
  char str[12];

  snprintf(str, 12, "%d", player->orientation);
  ret.code = 0;
  ret.data = str;
  printf("Orientation: %d\n", player->orientation);
  return (ret);
}

t_return selfid(t_player *player, int max, char *data, t_game *game)
{
  t_return ret;
  
  ret.code = 0;
  ret.data = player->id;
  printf("Identifiant: %s\n", player->id);
  return (ret);
}

t_return selfstats(t_player* player, int max, char *data, t_game *game)
{
  t_return ret;
  char str[12];

  snprintf(str, 12, "%d", player->energy);
  ret.code = 0;
  ret.data = str;
  printf("Energie: %d\n", player->energy);
  return (ret);
}
