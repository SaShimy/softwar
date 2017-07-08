#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <czmq.h>

typedef struct	s_conf
{
	bool	verbose;
	int		size;
	char	*log_file_path;
	int		cycle;
	int		rep_port;
	int		pub_port;
}	t_conf;

#endif