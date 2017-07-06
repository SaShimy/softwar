CC      =	gcc
NAME    =	server
SRC     = 	server.c
OBJ     =	$(SRC:%.c=%.o)
RM      =	rm -f

$(NAME):        $(OBJ)
	$(CC) $(OBJ) -o $(NAME) -lczmq -lzmq

all:    $(NAME)

clean:
	$(RM) $(OBJ)

fclean:         clean
	$(RM) $(NAME)

re:     fclean all