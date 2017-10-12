#include "server.h"

int set_size(t_conf *conf, char *value)
{
  if (check_map_size(value) == 0)
    conf->size = atoi(value);
  return (0);
}

int set_rep_port(t_conf *conf, char *value)
{
  if (check_port("rep-port", value) == 0)
    conf->rep_port = atoi(value);
  return (0);
}

int set_pub_port(t_conf *conf, char *value)
{
  if (check_port("pub-port", value) == 0)
    conf->pub_port = atoi(value);
  return (0);
}

int set_cycle(t_conf *conf, char *value)
{
  if (check_cycle(value) == 0)
    conf->cycle = atoi(value);
  return (0);
}

int set_logs(t_conf *conf, char *value)
{
  if (check_log_file(value) == 0)
    conf->log_file_path = value;
  return (0);
}

int set_default_conf(t_conf *conf)
{
  conf->verbose = false;
  conf->size = 5;
  conf->cycle = 2000000;
  conf->log_file_path = "./logs.txt";
  conf->rep_port = 4242;
  conf->pub_port = 4243;
  return (0);
}