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

// Set conf structure
int set_size(t_conf *conf, char *value);
int set_rep_port(t_conf *conf, char *value);
int set_pub_port(t_conf *conf, char *value);
int set_cycle(t_conf *conf, char *value);
int set_logs(t_conf *conf, char *value);

#endif