#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <czmq.h>
#include <pthread.h>

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
  int   (*func) (t_conf *conf, char *value);
}               t_func;

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
}		t_cell;

typedef struct  s_game
{
  t_conf *conf;
  t_player players[4];
  t_cell *cells;
  int game_status; // 0 waiting, 1 started, 2 finished
}               t_game;

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
** Map function
*/
char **init_map(int size);

/*
** User ingame function
*/
int identify(char *data, t_game *game);
int leftfwd(t_player *player, int max);
int rightfwd(t_player *player, int max);
int forward(t_player *player, int max);
int backward(t_player *player, int max);
int right(t_player *player, int max);
int left(t_player *player, int max);
int looking(t_player *player);
int selfid(t_player *player);
int selfstats(t_player* player);

int listen_rep(t_conf conf);
int server_send_msg(char *target, char *message, zsock_t *router);
int server_rcv_msg(zmsg_t *message);

int listen_pub(t_conf conf);

int identify(char *data, t_game *game);

#endif
