/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   carbon.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 03:15:03 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/25 21:52:37 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CARBON_H
# define CARBON_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include "../libs/libft/libft.h"

/*
**	DEFINES:
*/

# ifndef M_PI
#  define M_PI 3.14159265
# endif

# define CARBON_MAX_COLUMS 35
# define CARBON_MAX_ROWS 25

/*
**	STRUCTS:
*/

typedef struct s_texture
{
	char	*north;
	char	*south;
	char	*west;
	char	*east;
	char	*bonus;
}	t_carbon_texture;

typedef struct s_color
{
	int	r;
	int	g;
	int	b;
	int	rgb;
}	t_carbon_color;

typedef struct s_player
{
	double	x;
	double	y;
	char	direction;
	double	angle;
}	t_carbon_player;

typedef struct s_carbon_map
{
	char				**map_grids;
	int					width;
	int					height;
	int					player_found;
	t_carbon_texture	textures;
	t_carbon_color		floor;
	t_carbon_color		ceiling;
	t_carbon_player		player;
}	t_carbon_map;

typedef struct s_parse
{
	int		fd;
	char	*line;
	char	**file_content;
	int		texture_count;
	int		color_count;
	int		map_started;
	char	*filename;
}	t_carbon_parse;

// Parse main functions
int		parse_map_file(char *filename, t_carbon_map *map);
int		init_parse_data(t_carbon_parse *data, char *filename);
int		cleanup_parse(t_carbon_parse *data, int success);
int		validate_arguments(int argc, char **argv);

// File reading functions
int		read_file_content(t_carbon_parse *data);
int		count_file_lines(int fd);
int		process_file_lines(int fd);
int		read_and_process_lines(t_carbon_parse *data);

// Element parsing functions
int		parse_elements(t_carbon_parse *data, t_carbon_map *map);
int		process_element_line(char *line, t_carbon_parse *data,
			t_carbon_map *map, int i);
int		validate_element_counts(t_carbon_parse *data);
int		parse_texture(char *line, t_carbon_map *map);
int		parse_color(char *line, t_carbon_map *map);
int		parse_map_grid(t_carbon_parse *data, t_carbon_map *map,
			int start_line);

// Validation functions
int		is_texture_line(char *line);
int		is_color_line(char *line);
int		is_empty_line(char *line);
int		validate_rgb_values(t_carbon_color *color);
int		validate_map(t_carbon_map *map);
int		validate_map_walls(t_carbon_map *map);
int		validate_map_characters(t_carbon_map *map);
int		validate_player_found(t_carbon_map *map);

// Map processing functions
int		validate_map_dimensions(t_carbon_parse *data, t_carbon_map *map,
			int start_line);
void	normalize_map_line_to_grid(char *line, char *grid_row, int width);
char	*normalize_map_line(char *line, int width);
int		find_max_width(char **content, int start_line);
int		find_player_position(char *line, t_carbon_map *map, int y);
void	set_player_angle(t_carbon_player *player);
int		check_multiple_players(t_carbon_map *map);
void	set_player_data(t_carbon_map *map, int i, int y, char direction);

// Color parsing functions
int		parse_rgb_values(char **rgb_split, t_carbon_color *color);
int		get_color_pointer(char *identifier, t_carbon_map *map,
			t_carbon_color **color);

// Map validation functions
int		check_floor_borders(t_carbon_map *map, int i, int j);
int		check_floor_spaces(t_carbon_map *map, int i, int j);

// Utility functions
void	free_split(char **split);
int		init_map(t_carbon_map *map);
void	cleanup_map(t_carbon_map *map);
char	**allocate_map_grid(int height, int width);
void	free_map_grid(char **grid, int height);

// Debug functions (debug_utils.c)
void	print_map_info(t_carbon_map *map);
void	print_parse_progress(char *step, int success);
void	print_texture_info(t_carbon_texture *textures);
void	print_color_info(t_carbon_color *floor, t_carbon_color *ceiling);
void	print_player_info(t_carbon_player *player);
void	print_map_dimensions(t_carbon_map *map);
void	print_colors(t_carbon_map *map);
void	print_texture_paths(t_carbon_map *map);
void	print_map_grid_lines(t_carbon_map *map);

#endif