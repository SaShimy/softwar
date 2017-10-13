#include "server.h"

int is_player(t_player player[4], int x, int y)
{
  int i;
  for(i = 0; i < 4; i++)
    {
      if (player[i].pos_x == x && player[i].pos_y == y)
	{
	  return (1);
	}
    }
  return (0);
}

t_player *get_player_from_pos(t_player player[4], int x, int y)
{
  int i;
  t_player *playertmp;

  playertmp = malloc(sizeof(t_player));
  playertmp->id = "empty,";
  for(i = 0; i < 4; i++)
    {
      if (player[i].pos_x == x && player[i].pos_y == y)
	{
	  return (&player[i]);
	}
    }
  return (playertmp);
}

int is_cell(t_game *game, int x, int y)
{
  t_cell *tmp;
  t_container container;

  container = game->container;
  
  if (container.nb_elem == 0)
    {
      return (0);
    }
  tmp = container.first;
  while (tmp != NULL)
    {
      if (tmp->x == x && tmp->y == y)
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

t_container_case *get_fill(t_game *game)
{
  int i;
  int j;
  int length;
  t_case *tab_case;
  t_container_case *container;

  for (i = 0; i < game->conf->size; i++)
    {
      for (j = 0; j < game->conf->size; j++)
	if (is_player(game->players, i, j) || is_cell(game, i, j))
	  {
	    length++;
	  }
    }
  tab_case = malloc(sizeof(t_case) * length);
  for (i = 0, length = 0; i < game->conf->size; i++)
    {
      for (j = 0; j < game->conf->size; j++)
	if (is_player(game->players, i, j) || is_cell(game, i, j))
	  {
	    tab_case[length].x = i;
	    tab_case[length].y = j;
	    length++;
	  }
    }
  container->length = length;
  container->tab_case = tab_case;
  return (container);
}
		

t_container_case *get_empty(t_game *game)
{
  int i;
  int j;
  int length;
  t_case *tab_case;
  t_container_case *container;
  
  for (i = 0, length = 0; i < game->conf->size; i++)
    {
      for (j = 0; j < game->conf->size; j++)
	if (!is_player(game->players, i, j) && !is_cell(game, i, j))
	  {
	    length++;
	  }
    }
  tab_case = malloc(sizeof(t_case) * length);
  for (i = 0, length = 0; i < game->conf->size; i++)
   {
     for (j = 0; j < game->conf->size; j++)
       if (!is_player(game->players, i, j) && !is_cell(game, i, j))
	 {
	   tab_case[length].x = i;
	   tab_case[length].y = j;
	   length++;
	 }
   }
  container->length = length;
  container->tab_case = tab_case;
  return (container);
}



void    add_cell_to_container(t_container *container, t_cell *cell) {
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

int	create_cell_condition(t_game *game, int size, t_cell *cell)
{
  int y;
  int length;
  int rand;
  t_container_case *container;
  t_case *tab_case;
  
  container = get_empty(game);
  tab_case = container->tab_case;
  length = container->length;
  if(length == 0)
    {
      return (1); //no more place
    }
  rand = generer_rand(0, length);
  cell->x = tab_case[rand].x;
  cell->y = tab_case[rand].y;
  add_cell_to_container(&game->container, cell);
  return (0);
}

t_cell *search_cell(t_container container, int x, int y)
{
  t_cell *tmp;

  tmp = container.first;
  while (tmp != NULL)
    {
      if (tmp->x == x && tmp->y == y)
	{
	  return (tmp);
	}
      tmp = tmp->next;
    }
  return (tmp);
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
  cell->value = energy;
  return (create_cell_condition(game, game->conf->size, cell));
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
  if ( cell && game->container.nb_elem > 0)
    {
      if (!cell_prev)
	{
	  if (!cell_next)
	    {
	      game->container.first = NULL;
	      game->container.last = NULL;
	    }
	  else if (cell_next)
	    {
	      game->container.first = cell_next;
	      cell_next->prev = NULL;
	    }
	}
      del_cd(&game->container, cell_prev, cell_next);
      free(cell);
      game->container.nb_elem -= 1;
    }
}
