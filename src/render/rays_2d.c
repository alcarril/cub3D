/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays_2d.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/07 13:26:17 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 21:56:46 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Comprueba si un punto (x, y) toca una pared o está fuera de los límites del 
	mapa.
	Detalles:
	- Verifica si las coordenadas (x, y) están fuera de los límites del mapa. 
	  Si es así, devuelve `true` (1), ya que se considera que el punto está en 
	  una pared o fuera del mapa.
	- Si las coordenadas están dentro de los límites del mapa, comprueba si la 
	  celda correspondiente en el mapa contiene una pared (`WALL`). Si es así, 
	  devuelve `true` (1).
	- Si no se cumple ninguna de las condiciones anteriores, devuelve `false` 
	  (0), indicando que el punto no toca una pared ni está fuera de los 
	  límites.
	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del mapa.
	- x: Coordenada X del punto a comprobar.
	- y: Coordenada Y del punto a comprobar.
	Retorno:
	- `true` (1) si el punto toca una pared o está fuera de los límites del 
	  mapa.
	- `false` (0) si el punto no toca una pared y está dentro de los límites 
	  del mapa.
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
	Dibuja el haz de rayos en 2D desde la posición del jugador en el minimapa.

	Detalles:
	- Los rayos se dibujan dentro del campo de visión (FOV) del jugador.
	- El FOV se divide en segmentos iguales según el ancho del minimapa para 
	  cubrir todo el campo de visión y no dejar espacios sin rayos ni 
	  solapamientos de rayos (más coste computacional -> mayor complejidad).
	- Sobreescribimos la memoria de la imagen para dibujar los rayos, en vez de
	  triangular para saber si en el recorrido del minimapa aún estamos en un 
	  píxel de la matriz de píxeles que representa el minimapa.

	Mejoras de rendimiento:
	- Las conversiones a radianes se realizan desde los eventos de movimiento 
	  para evitar recalcular en cada paso del raycasting (menos ciclos, ya que
	  son divisiones y multiplicaciones).
	- Uso de variables locales para usar registros de CPU y optimizar el 
	  rendimiento.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico.
	- scal_z: Escalado del minimapa en ambos ejes (X, Y).
*/
void	draw_rays2d(t_mlx *mlx, float *scal_z)
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
		draw_ray2d(mlx, diferencial, rad_pos, scal_z);
		rad_pos += rad_dif;
	}
}

/*
	Función para pintar un rayo en 2D desde la posición del jugador hasta que 
	toque una pared.

	Detalles:
	- En el cálculo del diferencial del eje Y se usa el ajuste norte-sur 
	  (inverso del seno).
	- Se calculan las coordenadas del rayo utilizando funciones paramétricas, 
	  aumentando el factor `t` (step) que mueve el punto en cada iteración en 
	  la dirección de cada uno de los componentes del vector unitario 
	  determinado por el ángulo `rad`. Esto crea el diferencial desde el punto 
	  de origen (posición del jugador) hasta que el rayo toca una pared. El 
	  diferencial en cada eje se incrementa en cada iteración.
	- Este método es más lento que el bucle DDA, ya que puede entrar en paredes 
	  o no detectar esquinas. Para darle más precisión, se puede reducir el 
	  valor de `step`, pero esto afecta el rendimiento al aumentar los cálculos 
	  por rayo.
	- Bufferiza el píxel en verde (0x00FF00) para representar el rayo en el 
	  minimapa.

	Mejoras de rendimiento:
	- Uso de variables locales como `scale` para evitar accesos repetidos a 
	  memoria y optimizar el rendimiento utilizando registros de CPU y el 
	  caché.
*/
void	draw_ray2d(t_mlx *mlx, float *differencial, float rad, float *scal_z)
{
	int		window[2];
	float	frame_offset[2];
	float	step;

	step = 0.1f;
	frame_offset[X] = mlx->frame->mm_offset[X];
	frame_offset[Y] = mlx->frame->mm_offset[Y];
	while (!touch_wall(mlx, differencial[X], differencial[Y]))
	{
		differencial[X] += step * cos(rad);
		differencial[Y] += step * -sin(rad);
		window[X] = (differencial[X] - frame_offset[X]) * scal_z[X];
		window[Y] = (differencial[Y] - frame_offset[Y]) * scal_z[Y];
		if (window[X] >= 0 && window[X] < mlx->frame->mm_widht
			&& window[Y] >= 0 && window[Y] < mlx->frame->mm_height)
		{
			buffering_pixel(window[X], window[Y], mlx, 0x00FF00);
		}
	}
}
