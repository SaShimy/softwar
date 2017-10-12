#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <czmq.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

typedef struct s_return
{
  int code;
  char *data;
}		t_return;

typedef struct  s_conf
{
  bool  verbose;
  int   size;
  char  *log_file_path;
  int   cycle;
  int  rep_port;
  int  pub_port;
}               t_conf;

typedef struct  s_func
{
  char  *name;
  int   (*func) (t_conf *conf, char* value);
}               t_func;

typedef struct s_case
{
  int x;
  int y;
}		t_case;

typedef struct s_container_case
{
  int length;
  t_case *tab_case;
}		t_container_case;

typedef struct  s_player
{
  char *id;
  int energy;
  int pos_x;
  int pos_y;
  int ap;
  int orientation;// 0 left 1 up 2 right 3 bottom
  int player;
}               t_player;

typedef struct s_cell
{
  int x;
  int y;
  int value; // energie cellule
  struct s_cell *next;
  struct s_cell *prev;
}		t_cell;

typedef struct s_container
{
  t_cell *first;
  t_cell *last;
  int nb_elem;
}		t_container;

typedef struct  s_game
{
  t_conf *conf;
  t_player players[4];
  t_container container;
  int game_status; // 0 waiting, 1 started, 2 finished
}               t_game;

typedef struct s_publisher
{
    zsock_t	    *pub;
    pthread_t   thread;
}       t_publisher;

typedef struct s_thread
{
    t_game          *game;
    zsock_t         *publisher;

}               t_thread;

typedef struct s_actions
{
  char *name;
  t_return (*func) (t_player* player, int max, char *data);
}               t_actions;


/*
** Set configuration structure
*/
int set_size(t_conf *conf, char *value);
int set_rep_port(t_conf *conf, char *value);
int set_pub_port(t_conf *conf, char *value);
int set_cycle(t_conf *conf, char *value);
int set_logs(t_conf *conf, char *value);
int set_default_conf(t_conf *conf);

/*
** Check options validity
*/
int check_port(char *name, char *port);
int check_map_size(char *size);
int check_cycle(char *cycle);
int check_log_file(char *path);

/*
** User ingame function
*/
t_return identify(char *data, t_game *game);
t_return leftfwd(t_player *player, int max, char *data);
t_return rightfwd(t_player *player, int max, char *data);
t_return forward(t_player *player, int max, char *data);
t_return backward(t_player *player, int max, char *data);
t_return right(t_player *player, int max, char *data);
t_return left(t_player *player, int max, char *data);
t_return looking(t_player *player, int max, char *data);
t_return selfid(t_player *player, int max, char *data);
t_return selfstats(t_player* player, int max, char *data);
t_return jump(t_player *player, int max, char *data);

int listen_rep(t_conf *conf, t_game *game);
int server_send_msg(char *target, char *message, zsock_t *router);
int server_rcv_msg(zmsg_t *message, t_game *game, zsock_t *router);

void showInfoUser(t_player *player);

/*
** cell
*/
int     create_cell(t_game *game);
void    del_cell_from_container(t_game *game, t_cell *cell);

/*
** THREAD / PUB
*/
zsock_t		*init_pub(t_conf *conf);
void *exec_pub(void *arg);
t_thread	*init_thread(t_game *game, t_conf *conf);

void *test_exec_pub(void *arg);
void test(t_game *game, t_conf *conf);
void init_pub_thread(t_game *game, t_conf *conf);

/*
** check case
*/
int is_player(t_player player[4], int x, int y);
int is_cell(t_container *container, int x, int y);

#endif
