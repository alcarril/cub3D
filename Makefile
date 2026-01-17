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

NAME = cub3D

CC = cc
CC_FLAGS = -Wall -Wextra -Werror 
MLX_FLAGS = -L$(LIB_DIR)/minilibx-linux -lmlx_Linux -lXext -lX11 -lm -lz -O3 -flto -funroll-loops -march=native
LIBFT_FLAGS = -L$(LIB_DIR)/libft -lft
DEBUG_FLAGS = -g3 -fsanitize=address
HEADERS = -I$(INC_DIR)

LIB_DIR = libs/
INC_DIR = inc/
SRC_DIR = src/
OBJ_DIR = obj/
LOG_DIR = log/
MKDIR = mkdir -p

SRC_FILES = mlx_init_close/init_mlx_game.c \
			mlx_init_close/setup_game.c \
			mlx_init_close/init_setup_utils.c \
			mlx_init_close/close_mlx_game.c \
			mlx_init_close/colors.c \
			events/keys.c \
			events/graphic_engine_keys.c \
			events/ambiance_keys.c \
			events/minimap_keys.c \
			events/player_keys.c \
			events/mouse_keys_buttons.c \
			moves/move_player.c \
			moves/mouse.c \
			render/render.c \
			render/raycasting.c \
			render/dda_algorith.c \
			render/floor_celling.c \
			render/render_textures.c \
			render/render_minimap.c \
			render/utils.c \
			render/rays_2d.c \
			ambiances/ambiance_configs.c \
			ambiances/ambiance_configs1.c \
			ambiances/fog_desaturation.c \
			ambiances/shaders.c \
			testing/prueba.c \
			mains/main.c

SRCS = $(addprefix $(SRC_DIR), $(SRC_FILES))

OBJ_FILES = $(SRC_FILES:.c=.o)
OBJS = $(addprefix $(OBJ_DIR), $(OBJ_FILES))

LIBFT = $(LIB_DIR)/libft/libft.a
MINI_LIBX = $(LIB_DIR)/minilibx-linux/libmlx.a

all: $(LIBFT) $(MINI_LIBX) $(OBJ_DIR) $(LOG_DIR) $(NAME)

$(LIBFT): 
	make all -C libs/libft

$(MINI_LIBX):
	make all -C libs/minilibx-linux

$(OBJ_DIR):
	@$(MKDIR) $(OBJ_DIR)

$(LOG_DIR):
	@$(MKDIR) $(LOG_DIR)

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
	rm -rf $(LOG_DIR)

re: fclean all

.PHONY: clean fclean re bonus