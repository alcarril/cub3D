/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alcarril.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/15 03:14:57 by alejandro         #+#    #+#             */
/*   Updated: 2025/12/28 11:58:13 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ALCARRIL_H
#define ALCARRIL_H

/*
	LIBRERIAS:
*/


#include <stdbool.h>

/*
	DEFINES:
*/

# define WIDTH 720
# define HEIGHT 720

# define MAX_COLUMS 20
# define MAX_ROWS 20

#define MINI_WIDTH 1
#define MINI_HEIGHT 1

# define WALL 1
# define FLOOR 0

# define PI 3.14159265
# define EPSILON 0.2

/*
	STRUCTS:
*/
typedef struct	s_player_data
{
	int		origin[2]; //default WITH/2 && HEIGHT/2 -> fix_map_inital position
	float		pos_x; //t0 = origin[0]
	float	pos_y; //t0 = origin[1]
	double	raydir_x; //default cos(90) = 0
	double	raydir_y; //default sen(90) = 1
	float	angle;
	float	speed;
	
	bool	move_up;
	bool	move_down;
	bool	move_left;
	bool	move_right;
	bool	r_counterclockwise;
	bool	r_clockwise;
	
}	t_player;

typedef struct	s_map
{
	unsigned int	max_columns; //x
	unsigned int	max_rows; //y
	char map_grids[MAX_ROWS][MAX_COLUMS];
}	t_map;

typedef struct	s_mlx_api_components
{
	void	*mlx_var;
	void	*mlx_window;
	void	*mlx_img;
	char	*win_name;
	//offsetinfo
	char	*bit_map_address;
	char	*bit_map_address_copy;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	t_player	*player;
	t_map		*map;

}	t_mlx;


/*
	FUNCTION PROTOTIPES:
*/

void	buffering_pixel(int x, int y, t_mlx *mlx, int color);
int render_frame2D(t_mlx *mlx);//
void render_cell2D(t_mlx *mlx, unsigned int map_x, unsigned int map_y, int *scale);
int	close_handler(t_mlx *mlx);
int	key_press(int keysym, t_mlx *mlx);
int	key_release(int keysym, t_mlx *mlx);
void	move_player(t_mlx *mlx);
void	rotate_player(t_mlx	*mlx);
char	init_mlx_components(t_mlx *mlx);
void	create_hooks(t_mlx *mlx);
void	destroy_mlx_componets(int (*f)(), int (*g)(), int (*t)(),
	t_mlx *mlx);
void setup_player(t_mlx *mlx);

#endif