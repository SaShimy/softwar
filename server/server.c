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
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      for (j = 0; j < 5; j++) {
        if (strcmp(argv[i]+2, opt[j].name) == 0) {
          printf("%s : %s\n", argv[i]+2, argv[i + 1]);
        }
      }
    } else if (argv[i][0] == '-' && argv[i][1] == 'v' && argv[i][2] == '\0') {
      conf->verbose = true;
    }
  }
  return (0);
}

int main(int argc, char **argv)
{
  t_conf conf;
  get_options(&conf, argc, argv);
  return (0);
}