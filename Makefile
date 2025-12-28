# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    MakeDIR                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/12/15 05:24:25 by alejandro         #+#    #+#              #
#    Updated: 2025/12/21 17:24:29 by alejandro        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cube3D

CC = cc
CC_FLAGS = -Wall -Wextra -Werror
MLX_FLAGS = -L$(LIB_DIR)/minilibx-linux -lmlx_Linux -lXext -lX11 -lm -lz -O3
LIBFT_FLAGS = -L$(LIB_DIR)/libft -lft
DEBUG_FLAGS = -g3 -fsanitize=address
HEADERS = -I$(INC_DIR)

LIB_DIR = libs/
INC_DIR = inc/
SRC_DIR = src/
OBJ_DIR = obj/
MKDIR = mkdir -p

SRC_FILES = mlx_init_close/init_close.c \
			events/keys.c \
			render/render.c \
			mains/main.c

SRCS = $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

LIBFT = $(LIB_DIR)/libft/libft.a
MINI_LIBX = $(LIB_DIR)/minilibx-linux/libmlx.a

all: $(LIBFT) $(MINI_LIBX) $(OBJ_DIR) $(NAME)

$(LIBFT): 
	make all -C libs/libft

$(MINI_LIBX):
	make all -C libs/minilibx-linux

$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(NAME): $(OBJS) $(LIBFT) $(MINI_LIBX)
	$(CC) $(CC_FLAGS) $(OBJS) $(LIBFT_FLAGS) $(MLX_FLAGS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(MKDIR) $(dir $@)
	$(CC) $(CC_FLAGS) $(HEADERS) -c $< -o $@

clean:
	make clean -C $(LIB_DIR)/libft
	make clean -C $(LIB_DIR)/minilibx-linux
	rm -rf $(OBJ_DIR)

fclean: clean
	make fclean -C $(LIB_DIR)/libft
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re bonus