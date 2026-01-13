/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays_2d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:26:17 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/13 16:08:25 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Comprueba si un punto (x, y) toca una pared en el mapa.
	Devuelve true si toca una pared o está fuera de los límites del mapa, false en caso contrario.
*/
bool	touch_wall(t_mlx *mlx, float x, float y)
{
	if (x < 0 || y < 0 || x >= mlx->map->max_columns || y >= mlx->map->max_rows)
		return (1);
	if (mlx->map->map_grids[(unsigned int)y][(unsigned int)x] == WALL)
		return (1);
	return (0);
}

/*
	- Dibuja el haz de rayos en 2D desde la posición del jugador en el minimapa.
	- Los rayos se dibujan dentro del campo de visión (FOV) del jugador.
	- El FOV se dicide en segmentos iguales según el ancho del minimapa para cubrir
	 todo el campo de vision y no dejar espacios sin rayos y tampoco solapamientos de
	 rayos (mas conste computacional-> complejidad).
	- Sobreescribimos la memeria de la imagen para dibujar los rayos. en vez de triangular
	  para saber si en el recorrido del minimpa un estamos en un pixel de la matriz de pixeles
	  que representa el minimapa
	- Uso de variables locales para usar registros de CPU y optimizar el rendimiento.
*/
void draw_rays2D(t_mlx *mlx, float *scal_z)
{
	float	diferencial[2];
	float	fov_half;
	float	rad_pos;
	float	rad_dif;
	float	rad_end;

	fov_half = mlx->player->rad_fov / 2;
	rad_pos = (mlx->player->rad_angle - fov_half);
	rad_end = (mlx->player->rad_angle + fov_half);
	rad_dif = (mlx->player->rad_fov / (mlx->frame->mm_widht));
	while (rad_pos <= rad_end)
	{
		diferencial[X] = mlx->player->pos_x;
		diferencial[Y] = mlx->player->pos_y;
		draw_ray2D(mlx, diferencial, rad_pos, scal_z);
		rad_pos += rad_dif;
	}
}

/*
	Funcion para pintar un rayo en 2D desde la posicion del jugador hasta que toque una pared.
	- En el calculo del diferencial del eje y se usa el ajuste norte sur (inverso del seno).
	- Uso de variables locales en el bucle para usar registros de CPU y optimizar el rendimiento.
	- Calculo de ocoordenadas de rayo con funciones parametricas, amentando el factor (t) step que
	  mueve el punto en cada iteracion en la direccion de cada un ade al componenes del vector unitario
	  determinado por el angulo rad, crando el direncial desde el punto de origen (posicion del jugador)
	  hasta tocar una pared, se va incrementando el diferencial en cada eje.
	- Esto es mucho mas lento que el bucle dda, puede entrar enparedes o no detectar esquinas, para darle mas
	  precision se puede reducir el step pero a costa de rendimiento (mas calculos por rayo).
	- Bufferización del píxel en verde (0x00FF00) para representar el rayo en el minimapa.(Verde)
	- Uso de variables locales como scale para evitar accesos repetidos a memoria y optimizar rendimientos usando
	  registros de CPU y cache.
*/
void	draw_ray2D(t_mlx *mlx, float *differencial, float rad, float *scal_z)
{
	int		window[2];
	float	step;
	
	step = 0.1f;
	while (!touch_wall(mlx, differencial[X], differencial[Y]))
	{
		differencial[X] += step * cos(rad);
		differencial[Y] += step * -sin(rad);
		window[X] = (differencial[X] - mlx->frame->mm_offset[X]) * scal_z[X];
		window[Y] = (differencial[Y] - mlx->frame->mm_offset[Y]) * scal_z[Y];
		if (window[X] >= 0 && window[X] < mlx->frame->mm_widht &&
				window[Y] >= 0 && window[Y] < mlx->frame->mm_height)
		{
			buffering_pixel(window[X], window[Y], mlx, 0x00FF00);
		}
	}
}
