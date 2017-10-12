#include "server.h"

char* concat(const char *s1, const char *s2)
{
  char *result = malloc(strlen(s1)+strlen(s2)+1);
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

t_return inspect(t_player player[4], int max, char *data, t_game *game)
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

t_return gather(t_player *player, int max, char *data, t_game *game)
{
  t_return ret;

  ret.data = "null";
  return (ret);
}

t_return attack(t_player *player, int max, char *data, t_game *game)
{
  t_return ret;

  ret.data = "null";
  return (ret);
}

t_return next(t_player *player, int max, char *data, t_game *game)
{
  t_return ret;

  ret.data = "null";
  return (ret);
}

t_case *get_direction_watch(t_player *player)
{
  t_case *tab_case;
 
  tab_case = malloc(sizeof(t_case) * 4);
  if (player->orientation == 0)
    {
      tab_case[0].x = player->pos_x - 1;
      tab_case[0].y = player->pos_y;
      tab_case[1].x = player->pos_x - 2;
      tab_case[1].y = player->pos_y + 1;
      tab_case[2].x = player->pos_x - 2;
      tab_case[2].y = player->pos_y;
      tab_case[3].x = player->pos_x - 2;
      tab_case[3].y = player->pos_y - 1;
    }
  else if (player->orientation == 1)
    {
      tab_case[0].x = player->pos_x;
      tab_case[0].y = player->pos_y - 1;
      tab_case[1].x = player->pos_x - 1;
      tab_case[1].y = player->pos_y - 2;
      tab_case[2].x = player->pos_x;
      tab_case[2].y = player->pos_y - 2;
      tab_case[3].x = player->pos_x + 1;
      tab_case[3].y = player->pos_y - 2;
    }
  else if (player->orientation == 2)
    {
      tab_case[0].x = player->pos_x + 1;
      tab_case[0].y = player->pos_y;
      tab_case[1].x = player->pos_x + 2;
      tab_case[1].y = player->pos_y - 1;
      tab_case[2].x = player->pos_x + 2;
      tab_case[2].y = player->pos_y;
      tab_case[3].x = player->pos_x + 2;
      tab_case[3].y = player->pos_y + 1;
    }
  else if (player->orientation == 3)
    {
      tab_case[0].x = player->pos_x;
      tab_case[0].y = player->pos_y + 1;
      tab_case[1].x = player->pos_x + 1;
      tab_case[1].y = player->pos_y + 2;
      tab_case[2].x = player->pos_x;
      tab_case[2].y = player->pos_y + 2;
      tab_case[3].x = player->pos_x - 1;
      tab_case[3].y = player->pos_y + 2;
    }
  return (tab_case);
}

t_return watch(t_player *player, int max, char *data, t_game *game)
{
  t_return ret;
  int length, x;
  t_case *tab_case;
  char *str, *tmp, *tmp2;

  str = tmp = "";
  tab_case =  get_direction_watch(player);
  for (x = 0; x < 4; x++)
    {
      if (is_player(game->players, tab_case[x].x, tab_case[x].y))
	{
	  free(str);
	  tmp2 = concat(get_id_from_pos(game->players, tab_case[x].x, tab_case[x].y)->id, ",");
	  str =  concat(tmp, tmp2);
	  free(tmp);
	  free(tmp2);
	  tmp = str;
	}
      else if (is_cell(game, tab_case[x].x, tab_case[x].y))
	{
	  free(str);
	  str =  concat(tmp, "energy,");
	  free(tmp);
	  tmp = str;
	}
      else
	{
	  free(str);
	  str =  concat(tmp, "empty,");
	  free(tmp);
	  tmp = str;
	}
		    
    }
  ret.data = str;
  return (ret);
}
//gather
//watch
//attack
//inspect
//next
