#include "server.h"

int check_port(char *name, char *port)
{
  if (strlen(port) > 5 || atoi(port) > 65535 || atoi(port) < 1)
  {
    printf("%s isn't a valid port number for %s.\nDefault value set instead.\n", port, name);
    return (1);
  }
  return (0);
}

int check_map_size(char *size)
{
  if (atoi(size) < 5)
  {
    printf("%s is not a correct size for the map.\n Default value set instead.\n", size);
    return (1);
  }
  return (0);
}

int check_cycle(char *cycle)
{
  if (atoi(cycle) <= 0)
  {
    printf("%s is not a valid time for cycle.\nSet default value instead.\n", cycle);
    return (1);
  }
  return (0);
}

int check_log_file(char *path)
{
  if (access(path, W_OK) == -1 || access(path, F_OK) == -1)
  {
    printf("%s is not a valid path for log file.\nSet default value instead.\n", path);
    return (1);
  }
  return (0);
}
