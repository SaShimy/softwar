#include "server.h"

int is_player(t_player player[4], int x, int y)
{
  int i;
  for(i = 0; i < 4; i++)
    {
      if (player->pos_x == x && player->pos_y == y)
	{
	  return (1);
	}
    }
  return (0);
}

int is_cell(t_container *container, int x, int y)
{
  t_cell *tmp;
  
  if (container->nb_elem == 0)
    {
      return (0);
    }
  tmp = container->first;
  while (tmp != NULL)
    {
      if (tmp->pos_x == x && tmp->pos_y == y)
	{
	  return (1);
	}
      tmp = tmp->next;
    }
  return (0);
}

int	generer_rand(int min, int max)
{
  return (rand()%(max-min+1) + min);
}

t_case *get_fill(t_game *game)
{
  int i;
  int j;
  int length;
  t_case *tab_case;

  for (i = 0; i < game->conf->size; i++)
    {
      for (j = 0; j < game->conf->size; j++)
	if (is_player(game->player, i, j) || is_cell())
	  {
	    length++;
	  }
    }
  tab_case = malloc(sizeof(t_case) * length);
  for (i = 0, length = 0; i < game->conf->size; i++)
    {
      for (j = 0; j < game->conf->size; j++)
	if (is_player(game->player, i, j) || is_cell())
	  {
	    tab_case[length]->x = i;
	    tab_case[length]->y = j;
	    length++;
	  }
    }
  return (tab_case);
}
		

t_case *get_empty(t_game *game)
{
  int i;
  int j;
  int length;
  t_case *tab_case;
  
  for (i = 0; i < game->conf->size; i++)
    {
      for (j = 0; j < game->conf->size; j++)
	if (!is_player(game->player, i, j) && !is_cell())
	  {
	    length++;
	  }
    }
  tab_case = malloc(sizeof(t_case) * length);
  for (i = 0, length = 0; i < game->conf->size; i++)
   {
     for (j = 0; j < game->conf->size; j++)
       if (!is_player(game->player, i, j) && !is_cell())
	 {
	   tab_case[length]->x = i;
	   tab_case[length]->y = j;
	   length++;
	 }
   }
  return (tab_case);
}

int	create_cell_condition(t_game *game, int size, t_cell *cell)
{
  int x;
  int y;
  int length;
  int rand;
  t_case *case;

 case = get_empty(game);
 length = sizeof(case);
 rand = generer_rand(0, length);
 cell->pos_x = case[rand]->x;
 cell->pos_y = case[rand]->y;
 add_cell_to_container(&game->container, &cell);
}

int	create_cell(t_game *game)
{
  t_cell *cell;
  int energy;

  cell = malloc(sizeof(*cell));
  if (!cell)
    {
      return (1);
    }
  energy = generer_rand(5,15);
  cell->energy = energy;
  return (create_cell_condition(game, game->conf->size, &cell));
}

void	del_cd(t_container *container, t_cell *cell_prev, t_cell *cell_next)
{
  if (cell_prev)
    {
      if (cell_next)
	{
	  cell_prev->next = cell_next;
	  cell_next->prev = cell_prev;
	}
      else if (!cell_next)
	{
	  container->last = cell_prev;
	  cell_prev->next = NULL;
	}
    }
}

void	del_cell_from_container(t_game *game, t_cell *cell)
{
  t_cell	*cell_prev;
  t_cell	*cell_next;

  cell_prev = cell->prev;
  cell_next = cell->next;
  if ( cell && game->container->nb_elem > 0)
    {
      if (!cell_prev)
	{
	  if (!cell_next)
	    {
	      game->container->first = NULL;
	      game->container->last = NULL;
	    }
	  else if (cell_next)
	    {
	      game->container->first = cell_next;
	      cell_next->prev = NULL;
	    }
	}
      del_cd(game->container, cell_prev, cell_next);
      free(cell);
      game->container->nb_elem -= 1;
    }
}

void	add_cell_to_container(t_container *container, t_cell *cell) {
  if (container && cell)
    {
      if (container->nb_elem > 0)
	{
	  container->last->next = cell;
	  cell->prev = container->last;
	  container->last = cell;
	}
      else
	{
	  container->first = cell;
	  container->last = cell;
	  cell->prev = NULL;
	}
      cell->next = NULL;
      container->nb_elem += 1;
    }
}
