/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:34:21 by alejandro         #+#    #+#             */
/*   Updated: 2025/12/28 12:32:26 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

void	draw_ray2D(t_mlx *mlx, float scale_x, float scale_y);
bool	touch_wall(t_mlx *mlx, float x, float y, float epsilon);


void	buffering_pixel(int x, int y, t_mlx *mlx, int color)
{
	unsigned int	offset;

	if (x < 0 || x > WIDTH || y < 0 || y > HEIGHT)
		return ;
	offset = (y * mlx->line_length) + (x * (mlx->bits_per_pixel / 8));
	*(unsigned int *)(mlx->bit_map_address + offset) = color;
}


// //renderizado de conjunto de puntos de la ventana B -> a conjunto pares del mapa A
int render_frame2D(t_mlx *mlx)
{
	//todos los calores del mapa en float para poder hacer que l personaje se mmueva
	//de manera fluida dentro del minimapa, no de celda en celda (mov continuio vs mov discontinuo)
	int	window_x;
	int	window_y;
	float	map_x; 
	float	map_y; 
	float	scale_x;
	float	scale_y;

	move_player(mlx);
	// Calcular la escala de las celdas en función del tamaño del minimapa
	scale_x = (float)(WIDTH / MINI_WIDTH) / mlx->map->max_columns;
	scale_y = (float)(HEIGHT / MINI_HEIGHT) / mlx->map->max_rows;
	// Iterar sobre cada píxel de la ventana
	window_y = 0;
	while (window_y < HEIGHT / MINI_HEIGHT)
	{
		window_x = 0;
		while (window_x < WIDTH / MINI_WIDTH)
		{
			// Calcular la celda del mapa correspondiente al píxel actual
			map_x = (float)window_x / scale_x;
			map_y = (float)window_y / scale_y;
			// Verificar que las coordenadas del mapa sean válidas
			if ((unsigned int)map_x < mlx->map->max_rows && (unsigned int)map_y < mlx->map->max_columns)
			{
				// Dibujar el píxel según el contenido de la celda del mapa
				if (mlx->map->map_grids[(unsigned int)map_y][(unsigned int)map_x] == WALL)
					buffering_pixel(window_x, window_y, mlx, 0x00FF0000); // Color para paredes
				else
					buffering_pixel(window_x, window_y, mlx, 0x00000000); // Color para el suelo
				//epsilon sirve para determinar el tamaño del persona y qu eno entre dentro de las
				//paredes en el minimapa
				if (fabs(mlx->player->pos_x - map_x) < EPSILON &&
					fabs(mlx->player->pos_y - map_y) < EPSILON)
				{
					buffering_pixel(window_x, window_y, mlx, 0x0000FFFF);
				}

			}
			window_x++;
		}
		window_y++;
	}
	draw_ray(mlx, scale_x, scale_y);
	mlx_put_image_to_window(mlx->mlx_var, mlx->mlx_window, mlx->mlx_img, 0, 0);
	return (1);
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


void draw_ray2D(t_mlx *mlx, float scale_x, float scale_y)
{
	float rad_angle;
	float dx, dy;
	int window_x, window_y;

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
			window_x = dx * scale_x;
			window_y = dy * scale_y;
			if (window_x >= 0 && window_x < WIDTH && window_y >= 0 && window_y < HEIGHT)
				buffering_pixel(window_x, window_y, mlx, 0xFF00FF00);
		}
		rad_angle += 0.3 * (PI / 180.0f);
	}
}


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