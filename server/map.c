char **init_map(int size) {
  char map[size][size];
  int i;
  int j;

  for (i = 0; i < size; i++)
  {
    for (j = 0; j < size; j++)
    {
      map[i][j] = '0';
    }
  }
  return (map);
}