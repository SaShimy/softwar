#include "server.h"

t_return inspect(t_player player[4], int max, char *data)
{
  t_return ret;
  int x;
  char str[12];

  ret.code = 1;
  for(x = 0; x < 4; x++)
    {
      if(strcmp(player[x].id, data) == 0)
	{
	  snprintf(str, 12, "%d", player->energy);
	  ret.code = 0;
	  ret.data = str;
	}
    }
  return (ret);
}

t_return watch(t_player *player, int max, char *data)
{
  t_return ret;
  int length;
  
  
  //  get_direction_watch(player);
  ret.data = "null";
  return (ret);
}

t_return gather(t_player *player, int max, char *data)
{
  t_return ret;

  ret.data = "null";
  return (ret);
}

t_return attack(t_player *player, int max, char *data)
{
  t_return ret;

  ret.data = "null";
  return (ret);
}

t_return next(t_player *player, int max, char *data)
{
  t_return ret;

  ret.data = "null";
  return (ret);
}//gather
//watch
//attack
//inspect
//next
