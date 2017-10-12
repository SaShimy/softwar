#include "server.h"

int get_options(t_conf *conf, int argc, char **argv) {
  int     i;
  int     j;
  t_func  opt[5] = {
    {"size", set_size},
    {"log", set_logs},
    {"cycle", set_cycle},
    {"rep-port", set_rep_port},
    {"pub-port", set_pub_port}
  };

  for (i = 1; i < argc; i++)
  {
    if (argv[i][0] == '-' && argv[i][1] == '-')
    {
      for (j = 0; j < 5; j++)
      {
        if (strcmp(argv[i]+2, opt[j].name) == 0)
          opt[j].func(conf, argv[i + 1]);
      }
    } else if (argv[i][0] == '-' && argv[i][1] == 'v' && argv[i][2] == '\0')
      conf->verbose = true;
  }
  return (0);
}

t_game *init_game(t_conf *conf)
{
  t_game *game;
  t_container *container;

  container = malloc(sizeof(*container));
  game = malloc(sizeof(*game));
  if (!game || !container)
    {
      return (0);
    }
  game->conf = conf;
  container->first = NULL;
  container->last = NULL;
  container->nb_elem = 0;
  game->container = container;
  
  return (game);
}

int main(int argc, char **argv)
{
  int i;
  int j;
  t_game *game;

  t_conf conf;
  set_default_conf(&conf);
  game = init_game(&conf);
  printf("Configuration:\n-Verbose: %d\n-Size: %d\n-Cycle: %d\n-Log file: %s\n-Rep-port: %d\n-Pub-port: %d\n", conf.verbose, conf.size, conf.cycle, conf.log_file_path, conf.rep_port, conf.pub_port);
  // for (i = 0; i < conf.size; i++) {
  //   printf("%d\n", strlen(map[i]));
  // }
  // identify("#0x02", game);
  // identify("#0x03", game);
  // identify("#0x04", game);
  // game->game_status = 1;
  listen_rep(conf, game);
  return (0);
}
