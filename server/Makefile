CC	=	gcc
NAME	=	server
SRC	=	server.c		\
		set_options.c		\
		check_options.c		\
		commands1.c		\
		rep_req_server.c	\
		pub.c	\
		client_movements.c	\
		client_actions.c	\
		client_actions2.c	\
		list_cell.c
OBJ	=	$(SRC:%.c=%.o)
RM	=	rm -f
FLAGS	=	-Wall -Wextra -Werror -pedantic
LIBS	=	-lczmq -lzmq -lpthread -D_REENTRANT

$(NAME):	$(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) $(LIBS)

all:		$(NAME)

clean:
	$(RM) $(OBJ)

fclean:		clean
	$(RM) $(NAME)

re:		fclean all
