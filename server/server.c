#include "server.h"

int server_send_msg(zframe_t *identity, zframe_t *empty, zframe_t *content, /*void *router) {*/zsock_t *router) {
  zmsg_t *response = zmsg_new();

  zmsg_prepend(response, &identity);
  zmsg_append(response, &empty);
  zmsg_append(response, &content);
  zmsg_send(&response, router);
  zmsg_destroy(&response);
  zframe_destroy(&identity);
  zframe_destroy(&empty);
  zframe_destroy(&content);
  return (0);
}

int server_rcv_msg(zframe_t *content) {
  printf("Content of message is : %s\n", zframe_strdup(content));

  return (0);
}

int listen_rep(t_conf conf) {
  /*zctx_t *context = zctx_new ();
    void *router = zsocket_new(context, ZMQ_ROUTER);*/
  zsock_t *router = zsock_new(ZMQ_ROUTER);
  zmsg_t *message;
  zframe_t *identity;
  zframe_t *empty;
  zframe_t *content;
  
  zsock_bind(router, "tcp://*:%s", conf.rep_port);
  while (!zsys_interrupted) {
    //while (!zctx_interrupted) {
    message = zmsg_recv(router);
    identity = zmsg_pop(message);
    empty = zmsg_pop(message);
    content = zmsg_pop(message);
    zmsg_destroy(&message);
    server_rcv_msg(content);
    server_send_msg(identity, empty, content, router);
  }
  zsock_destroy(&router);
  //zctx_destroy(&context);
  return (0);
}

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
  listen_rep(conf);
  //identify("#0x01");
  return (0);
}
