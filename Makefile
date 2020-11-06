# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmurakam <tmurakam@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/26 20:34:32 by tmurakam          #+#    #+#              #
#    Updated: 2020/11/02 23:04:39 by tmurakam         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D
CC = gcc
#CFLAGS = -Wall -Wextra -Werror
CFLAGS = -g -fsanitize=address
RM = rm -f
INCLUDE = -I./includes -I./libft
SRCS_DIR = srcs
SRCS = ${SRCS_DIR}/main.c
SRCS += ${SRCS_DIR}/get_next_line.c

LIBFT_DIR = libft
LIBFT = ${LIBFT_DIR}/libft.a

MINILIBX_DIR = minilibx-linux
MINILIBX = ${MINILIBX_DIR}/libmlx.a

#LIBS = -L. -L./libft -lft -I/usr/local/include -L/usr/local/lib -L./minilibx-linux -lmlx_Linux -lX11 -lXext -lm -lbsd
LIBS  = -lX11 -lXext -lm -lbsd

OBJS = ${SRCS:.c=.o}
all: ${NAME}
bonus: ${NAME}

${NAME}: ${OBJS} ${LIBFT} ${MINILIBX}
	${CC} ${CFLAGS} ${INCLUDE} ${LIBS} ${OBJS} ${LIBFT} ${MINILIBX} -o ${NAME}

${LIBFT}: ${LIBFT_DIR}/*.c
	${MAKE} -C ${LIBFT_DIR}

${MINILIBX}:
	${MAKE} -C ${MINILIBX_DIR}

.c.o:
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

clean:
	${RM} ${OBJS}
	${MAKE} -C ${MINILIBX_DIR} clean 
	${MAKE} -C ${LIBFT_DIR}	clean 

fclean: clean
	${RM} ${NAME}
	${MAKE} -C ${LIBFT_DIR} fclean	

re:	fclean all

.PHONY: all clean fclean re bonus
