#include "server.h"

int move_forward(t_player *player, int max)
{
  if (player->energy < 0.5)
    {
      return (0);
    }
  if ((player->orientation == 1 && player->pos_y - 1 < 0) ||
     (player->orientation == 3 && player->pos_y + 1 > max) ||
     (player->orientation == 0 && player->pos_x - 1 < 0) ||
     (player->orientation == 2 && player->pos_x + 1 > max))
    {
      return (1);
    }
  if (player->orientation % 2 == 0)
    {
      player->pos_x += player->orientation == 0 ? -1 : 1; 
    }
  else
    {
      player->pos_y += player->orientation == 1 ? -1 : 1;
    }
  
  return (2);
}

int move_backward(t_player *player, int max)
{
  if (player->energy < 1)
    {
      return (0);
    }
  if ((player->orientation == 1 && player->pos_y + 1 > max) ||
      (player->orientation == 3 && player->pos_y - 1 < 0) ||
      (player->orientation == 0 && player->pos_x + 1 > max) ||
      (player->orientation == 2 && player->pos_x - 1 < 0))
    {
      return (1);
    }
  if (player->orientation % 2 == 0)
    {
      player->pos_x += player->orientation == 0 ? 1 : -1;
    }
  else
    {
      player->pos_y += player->orientation == 1 ? -1 : 1;
    }

  return (2);
}

