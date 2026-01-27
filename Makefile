# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/01/19 00:00:00 by carbon-m          #+#    #+#              #
#    Updated: 2026/01/26 01:09:32 by carbon-m         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

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
LOG_DIR = log/
MKDIR = mkdir -p

# Archivos de carbon (parsing)
CARBON_FILES = parsing/parse_map.c \
			parsing/parse_elements.c \
			parsing/parse_textures.c \
			parsing/parse_colors.c \
			parsing/parse_map_grid.c \
			parsing/read_file.c \
			parsing/validate_map.c \
			parsing/validate_map_borders.c \
			parsing/parse_utils.c \
			parsing/player_utils.c \
			parsing/debug_utils.c \
			parsing/debug_utils2.c \
			parsing/cleanup_utils.c

# Archivos de alcarril (motor gráfico)
ALCARRIL_FILES = mlx_init_close/init_mlx_game.c \
			mlx_init_close/setup_game.c \
			mlx_init_close/setup_game2.c \
			mlx_init_close/init_setup_utils.c \
			mlx_init_close/close_mlx_game.c \
			mlx_init_close/colors.c \
			events/keys.c \
			events/graphic_engine_keys.c \
			events/ambiance_keys.c \
			events/minimap_keys.c \
			events/player_keys.c \
			events/player_keys2.c \
			events/player_keys3.c \
			events/mouse_keys_buttons.c \
			events/gravity_keys.c \
			events/phisics_keys.c \
			moves/move_player.c \
			moves/mouse.c \
			moves/mouse2.c \
			moves/axisz_phisics.c \
			moves/decelerate_air.c \
			moves/difspeed_vecmove_phisics.c \
			moves/phisics_utils.c \
			moves/speed_aceleration.c \
			moves/vectorization_moves.c \
			render/render.c \
			render/raycasting.c \
			render/dda_algorith.c \
			render/floor_celling.c \
			render/render_textures.c \
			render/render_minimap.c \
			render/rays_2d.c \
			render/drawing_textures.c \
			render/utils.c \
			ambiances/ambiance_configs.c \
			ambiances/ambiance_configs1.c \
			ambiances/fog_desaturation.c \
			ambiances/shaders.c \
			mem_utils/ft_bzero_boost.c \
			mem_utils/ft_memfillboost.c \
			mem_utils/ft_memset_boost.c

# Archivos puente (integración)
PUENTE_FILES = puente/bridge.c \
			puente/bridge_copy.c \
			puente/bridge_memory.c \
			mains/main.c

# Todos los archivos fuente
SRC_FILES = $(CARBON_FILES) $(ALCARRIL_FILES) $(PUENTE_FILES)

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

.PHONY: clean fclean re all
