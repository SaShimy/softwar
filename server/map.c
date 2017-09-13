#include "server.h"

char **init_map(int size) {
  char **map;
  int i;
  int j;

  map = malloc(sizeof(char *) * size);
  for (i = 0; i < size; i++)
  {
    map[i] = malloc(sizeof(char) * size);
    for (j = 0; j < size; j++)
    {
      map[i][j] = '0';
    }
  }
  return (map);
}