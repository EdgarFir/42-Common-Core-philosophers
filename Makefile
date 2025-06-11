NAME = philo

SRCS = philo.c philo_utils.c philo_struct_utils.c philo_error_utils.c philo_threads.c \
	   philo_logs.c	

OBJS = ${SRCS:.c=.o}

CC = cc

CFLAGS = -Wall -Werror -Wextra -pthread

RM = rm -rf

${NAME}: ${OBJS}
	${CC} ${CFLAGS} ${OBJS} -o ${NAME}

.c.o:
	${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

all: ${NAME}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM}  ${NAME}

re: fclean all

.PHONY: all clean fclean re