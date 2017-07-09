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

  for (i = 1; i < argc; ++i)
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

int main(int argc, char **argv)
{
  t_conf conf;
  set_default_conf(&conf);
  get_options(&conf, argc, argv);
  printf("Configuration:\n-Verbose: %d\n-Size: %d\n-Cycle: %d\n-Log file: %s\n-Rep-port: %s\n-Pub-port: %s\n", conf.verbose, conf.size, conf.cycle, conf.log_file_path, conf.rep_port, conf.pub_port);
  return (0);
}