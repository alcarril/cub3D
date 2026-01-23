/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_setup_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 23:20:39 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 23:05:51 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Esta funcion sirve para poder sacar la escala del minimapa en funcion de:
	- El numero de filas y columnas del mapa original
	- El procentaje del ancho y largo de la ventana que se usa para el minimapa
*/
void	get_minimapscale(t_mlx *mlx, float *scale)
{
	scale[X] = (float)(mlx->frame->mm_widht) / mlx->map->max_columns;
	scale[Y] = (float)(mlx->frame->mm_height) / mlx->map->max_rows;
}

/*
	Inicializamos los colores del suelo y el techo en formato hexadecimal
	a partir de los valores RGB almacenados en el mapa
*/
void	init_floor_and_ceiling_colors(t_map *map)
{
	int	floor[3];
	int	ceiling[3];

	ft_memcpy(floor, map->floor_color, sizeof(floor));
	ft_memcpy(ceiling, map->ceiling_color, sizeof(ceiling));
	map->floor_color_hex = rgb_to_hex(floor[0], floor[1], floor[2]);
	map->ceiling_color_hex = rgb_to_hex(ceiling[0], ceiling[1], ceiling[2]);
}

/*
	Asi solo estiro el lado que sea mas grande en relacion al mapa pero no 
	mantengo
	una escala fija. Si no lamantengo hace core dumped
	NOTA:
	Si se quiere cambiar se pone el row pr col con factor dedivision
	rows_per_col = (float)mlx->map->max_rows / (float)mlx->map->max_columns;
*/
void	setup_window_wh(t_mlx *mlx)
{
	float	rows_per_col;

	rows_per_col = 1;
	if (rows_per_col > 1)
	{
		mlx->win_width = BASE_WIDTH * rows_per_col;
		mlx->win_height = BASE_HEIGHT;
	}
	else
	{
		mlx->win_height = BASE_HEIGHT / rows_per_col;
		mlx->win_width = BASE_WIDTH;
	}
}

int	rgb_to_hex(int r, int g, int b)
{
	return ((r << 16) | (g << 8) | b);
}
