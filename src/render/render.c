/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:34:21 by alejandro         #+#    #+#             */
/*   Updated: 2025/12/29 12:41:05 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

void	draw_ray2D(t_mlx *mlx);
void	draw_minimap2D(t_mlx *mlx, int *win);
void	get_minimapscale(t_mlx *mlx, float *scale);
bool	touch_wall(t_mlx *mlx, float x, float y, float epsilon);
void	is_person2D(t_mlx *mlx, int *window, float *map);
// void	update_minimap_view(t_mlx *mlx, float zoom_level, int *minimap_limits);

//se puede optimizar pasando por refencia las variables
void	buffering_pixel(int x, int y, t_mlx *mlx, int color)
{
	unsigned int	offset;

	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return ;
	offset = (y * mlx->line_length) + (x * (mlx->bits_per_pixel / 8));
	*(unsigned int *)(mlx->bit_map_address + offset) = color;
}

// //renderizado de conjunto de puntos de la ventana B -> a conjunto pares del mapa A
int	render_frame2D(t_mlx *mlx)
{
	int		win[2];

	move_player(mlx);
	win[Y] = 0;
	while (win[Y] <= HEIGHT / MINI_HEIGHT)
	{
		win[X] = 0;
		while (win[X] <= WIDTH / MINI_WIDTH)
		{
			draw_minimap2D(mlx, win);
			win[X]++;
		}
		win[Y]++;
	}
	draw_ray2D(mlx);
	mlx_put_image_to_window(mlx->mlx_var, mlx->mlx_window, mlx->mlx_img, 0, 0);
	return (1);
}

/* Dibujo del jugador */
//epsilon sirve para determinar el tamaño del persona y qu eno entre dentro de las
//paredes en el minimapa
void	is_person2D(t_mlx *mlx, int *window, float *map)
{
	if (fabs(mlx->player->pos_x - map[X]) < EPSILON &&
			fabs(mlx->player->pos_y - map[Y]) < EPSILON)
	{
		buffering_pixel(window[X], window[Y], mlx, 0x0000FFFF);
	}
}

//se puede meter una optimizacion para que solo se calculen los pixeles cunado el pixel x e y
//pertenecesn al conjunto de pixeles de la zona de la vental adestianda al minimapa-> is_mimap
void	draw_minimap2D(t_mlx *mlx, int *win)
{
	//todos los calores del mapa en float para poder hacer que l personaje se mmueva
	//de manera fluida dentro del minimapa, no de celda en celda (mov continuio vs mov discontinuo)
	float			map[2];
	static float	scale[2];
	static int		control;//optimizacion de calculos

	/* Escala del minimapa */
	// Calcular la escala de las celdas en función del tamaño del minimapa
	if (control == 0)
	{
		get_minimapscale(mlx, scale);
		control++;
	}
	/* De ventana → mapa */
	// Calcular la celda del mapa correspondiente al píxel actual
	map[X] = (float)win[X] / scale[X];
	map[Y] = (float)win[Y] / scale[Y];

	/* Comprobación correcta de límites */
	// Verificar que las coordenadas del mapa sean válidas
	if ((unsigned int)map[Y] < mlx->map->max_rows &&
			(unsigned int)map[X] < mlx->map->max_columns)
	{
		if (mlx->map->map_grids[(unsigned int)map[Y]][(unsigned int)map[X]] == WALL)
			buffering_pixel(win[X], win[Y], mlx, 0x00FF0000);
		else
			buffering_pixel(win[X], win[Y], mlx, 0x00000000);
		is_person2D(mlx, win, map);
	}
}

//Fucnion para calculo de colisiones sin algoritmo de dda, entra esquinas y menos rapido
bool	touch_wall(t_mlx *mlx, float x, float y, float epsilon)
{
	(void)epsilon;
	if (x < 0 || y < 0 || x >= mlx->map->max_columns || y >= mlx->map->max_rows)
		return (1);
	if (mlx->map->map_grids[(unsigned int)y][(unsigned int)x] == WALL)
		return (1);
	return (0);
}

//calcul indeoendiente para no hacer los calculos de todos elos punto del la ventaja
//mejor que la interseccion de conjuntos, sobreescribimos meemrai de la imageb draw_ray > is_ray
void draw_ray2D(t_mlx *mlx)
{
	float rad_angle;
	float dx, dy;
	int window_x, window_y;
	float scale[2];
	
	get_minimapscale(mlx, scale);
	rad_angle = (mlx->player->angle - 30.0f) * (PI / 180.0f);
	while (rad_angle <= (mlx->player->angle + 30.0f) * (PI / 180.0f))
	{
		//el dx y dy se hace aqui para reiniciarlo a la posicion del punto del personaje sino
		//se quedaria en la posisciondel ultimo ray
		dx = mlx->player->pos_x;
		dy = mlx->player->pos_y;
		while (!touch_wall(mlx, dx, dy, EPSILON))
		{
			dx += 0.01 * cos(rad_angle);
			dy += 0.01 * -sin(rad_angle);//ajuste de norte sur 
			window_x = dx * scale[X];
			window_y = dy * scale[Y];
			if (window_x >= 0 && window_x < WIDTH && window_y >= 0 && window_y < HEIGHT)
				buffering_pixel(window_x, window_y, mlx, 0x000000FF);
		}
		rad_angle += 0.3 * (PI / 180.0f);
	}
}

void	get_minimapscale(t_mlx *mlx, float *scale)
{
	scale[X] = (float)(WIDTH / MINI_WIDTH) / mlx->map->max_columns;
	scale[Y] = (float)(HEIGHT / MINI_HEIGHT) / mlx->map->max_rows;
}

bool is_minimapzone(int win_x, int win_y)
{
	// Definir los límites del minimapa en la ventana
	int minimap_start_x = 0; // Coordenada X inicial del minimapa
	int minimap_start_y = 0; // Coordenada Y inicial del minimapa
	int minimap_end_x = WIDTH / MINI_WIDTH; // Coordenada X final del minimapa
	int minimap_end_y = HEIGHT / MINI_HEIGHT; // Coordenada Y final del minimapa

	// Comprobar si el píxel está dentro de los límites del minimapa
	if (win_x >= minimap_start_x && win_x <= minimap_end_x &&
		win_y >= minimap_start_y && win_y <= minimap_end_y)
			return (true);

	return (false);
}

//minmapa traslaciones 
//minimapa zoom
//minimap activar rayos o desactivar ->con putpixel o render del fram completo o rednder de parte del minimapa solo
//minimapa eater eggs
//rayos para matar personajes

/////////////////////////////FUNCIONES DE REDERIZADO DE B->A//////////////////////////////////////////

// void render_cell2D(t_mlx *mlx, unsigned int map_x, unsigned int map_y, int *scale)
// {
// 	int	window_x;
// 	int	window_y;
// 	int	i;
	
// 	i = 0;
// 	while (i < scale[0]) // Iterar dentro de la celda en x
// 	{
// 		int j = 0;
// 		while (j < scale[1]) // Iterar dentro de la celda en y
// 		{
// 			window_x = (map_x * scale[0]) + i; // Coordenada X en la ventana
// 			window_y = (map_y * scale[1]) + j; // Coordenada Y en la ventana //a1
// 			if (mlx->map->map_grids[map_y][map_x] == WALL && (i == scale[0] - 1 || i == 0))
// 				buffering_pixel(window_x, window_y, mlx, 0x0000FF00); // Color para paredes birdes horizontales
// 			else if (mlx->map->map_grids[map_y][map_x] == WALL && (j == scale[1] - 1 || j == 0))
// 				buffering_pixel(window_x, window_y, mlx, 0x0000FF00); // Color para paredes bordes verticales
// 			else if (mlx->map->map_grids[map_y][map_x] == WALL)
// 				buffering_pixel(window_x, window_y, mlx, 0x00FF0000); // Color para paredes
// 			else
// 				buffering_pixel(window_x, window_y, mlx, 0x00000000); // Color para el suelo
// 			// Dibujar al jugador si está en esta posición (como es un booleano se trcunca y se queda con la parte entera->funciona)
// 			float epsilon = 0.4;// Margen de error para la comparación
// 			if (fabs(mlx->player->pos_x - map_x) < epsilon &&
// 				fabs(mlx->player->pos_y - map_y) < epsilon)
// 			{
// 				buffering_pixel(window_x, window_y, mlx, 0x0000FFFF); // Color para el jugador
// 			}
// 			j++;
// 		}
// 		i++;
// 	}
// }

// //renderizado de conjunto pares del mapa (puntos x, y) A -> a conjunto de puntos (pares x, y) de la ventana
// void render_frame2DD(t_mlx *mlx, int minimap_sizex, int minimap_sizey)
// {
// 	unsigned int	map_x;
// 	unsigned int	map_y;
// 	int				scale[2];
	
// 	scale[0] = (WIDTH / minimap_sizex) / mlx->map->max_columns;
// 	scale[1] = (HEIGHT / minimap_sizey) / mlx->map->max_rows;
// 	map_x = 0;
// 	while (map_x < mlx->map->max_columns)
// 	{
// 		map_y = 0;
// 		while (map_y < mlx->map->max_rows)
// 		{
// 			render_cell2D(mlx, map_x, map_y, scale);
// 			map_y++;
// 		}
// 		map_x++;
// 	}
// 	mlx_put_image_to_window(mlx->mlx_var, mlx->mlx_window, mlx->mlx_img, 0, 0);
// }