#include "server.h"

int leftfwd(t_player *player, int max)
{
  if (player->ap < 1)
    {
      return (1);
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
      return (2);//ne peut pas avancer
    }
  if (player->orientation % 2 == 0)
    {
      player->pos_x += player->orientation == 0 ? -1 : 1;
    }
  else
    {
      player->pos_y += player->orientation == 1 ? -1 : 1;
    }
  return (0);
}

int rightfwd(t_player *player, int max)
{
  if (player->ap < 1)
    {
      return (1);
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
      return (2);//ne peut pas avancer
    }
  if (player->orientation % 2 == 0)
    {
      player->pos_x += player->orientation == 0 ? -1 : 1;
    }
  else
    {
      player->pos_y += player->orientation == 1 ? -1 : 1;
    }
  return (0);
}

int looking(t_player *player, int max)
{
  printf("Orientation: %d\n", player->orientation);
  return (0);
}

int selfid(t_player *player, int max)
{
  printf("Identifiant: %s\n", player->id);
  return (0);
}

int selfstats(t_player* player, int max)
{
  printf("Energie: %d\n", player->energy);
  return (0);
}
