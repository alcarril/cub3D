/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_game2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 23:20:39 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/12 23:43:20 by alejandro        ###   ########.fr       */
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
	Setup de las dimensiones de la ventana en funcion del tamaño del mapa
	- La altura de la ventana se calcula como el numero de filas del mapa
	  por una constante de escala (WIN_SCALE) por 4
	- La anchura de la ventana se calcula como el numero de columnas del mapa
	  por una constante de escala (WIN_SCALE) por 3
*/
void	setup_window_wh(t_mlx *mlx)
{
	mlx->win_height = mlx->map->max_rows * WIN_SCALE * 4;
	mlx->win_width = mlx->map->max_columns * WIN_SCALE * 3;
}

