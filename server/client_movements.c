#include "server.h"

t_return jump(t_player *player, int max)
{
  t_return ret;

  ret.data = "null";
  if (player->energy < 2)
    {
      ret.code = 1;
      return (ret);//pas assez d'energie
    }
  if ((player->orientation == 1 && player->pos_y - 2 < 0) ||
      (player->orientation == 3 && player->pos_y + 2 > max) ||
      (player->orientation == 0 && player->pos_x - 2 < 0) ||
      (player->orientation == 2 && player->pos_x + 2 > max))
    {
      ret.code = 2;
      return (ret);//ne peut pas avancer
    }
  if (player->orientation % 2 == 0)
    {
      player->pos_x += player->orientation == 0 ? -2 : 2;
    }
  else
    {
      player->pos_y += player->orientation == 1 ? -2 : 2;
    }
  ret.code = 0;
  player->energy -= 2;
  return (ret);
}

t_return forward(t_player *player, int max)
{
  t_return ret;

  ret.data = "null";
  if (player->ap < 0.5)
    {
      ret.code = 1;
      return (ret);//pas assez d'energie
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
  ret.code = 0;
  player->ap -= 0.5;
  return (ret);
}

t_return backward(t_player *player, int max)
{
  t_return ret;

  ret.data = "null";
  if (player->ap < 1)
    {
      ret.code = 1;
      return (ret);//pas assez d'energie
    }
  if ((player->orientation == 1 && player->pos_y + 1 > max) ||
      (player->orientation == 3 && player->pos_y - 1 < 0) ||
      (player->orientation == 0 && player->pos_x + 1 > max) ||
      (player->orientation == 2 && player->pos_x - 1 < 0))
    {
      ret.code = 2;
      return (ret);//ne peut pas avancer 
    }
  if (player->orientation % 2 == 0)
    {
      player->pos_x += player->orientation == 0 ? 1 : -1;
    }
  else
    {
      player->pos_y += player->orientation == 1 ? -1 : 1;
    }
  ret.code = 0;
  player->ap -= 1;
  return (ret);
}

t_return left(t_player *player, int max)
{
  t_return ret;

  ret.data = "null";
  if (player->ap < 0.5)
    {
      ret.code = 1;
      return (ret);
    }

  player->orientation -= 1;
  if (player->orientation < 0)
    {
      player->orientation = 3;
    }
  ret.code = 0;
  player->ap -= 0.5;
  return (ret);
}

t_return right(t_player *player, int max)
{
  t_return ret;

  ret.data = "null";
  if (player->ap < 0.5)
    {
      ret.code = 1;
      return (ret);
    }
  player->orientation += 1;
  if (player->orientation > 3)
    {
      player->orientation = 0;
    }
  ret.code = 0;
  player->ap -= 0.5;
  return (ret);
}
