NAME = philo

SRCS = philo.c philo_clean_utils.c philo_err_utils.c philo_logs.c \
	   philo_monitor.c philo_routine.c philo_struct_utils.c philo_utils.c

OBJS = ${SRCS:.c=.o}

CC = cc

CFLAGS = -Wall -Werror -Wextra -pthread #-fsanitize=thread

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