#ifndef SERVER_H
# define SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <czmq.h>

typedef struct  s_conf
{
  bool  verbose;
  int   size;
  char  *log_file_path;
  int   cycle;
  char  *rep_port;
  char  *pub_port;
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
  int orientation;// 0 up 1 right 2 bottom 3 left
  int player;
}               t_player;

typedef struct  s_game
{
  t_conf *conf;
  t_player players[4];
}               t_game;

/*
** Set configuration structure
*/
int set_size(t_conf *conf, char *value);
int set_rep_port(t_conf *conf, char *value);
int set_pub_port(t_conf *conf, char *value);
int set_cycle(t_conf *conf, char *value);
int set_logs(t_conf *conf, char *value);

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

#endif
