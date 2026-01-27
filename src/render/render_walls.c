/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_walls.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 22:22:40 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/27 03:52:13 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función principal para renderizar las paredes en la escena 3D.
	Selecciona la estrategia de renderizado basada en las configuraciones 
	actuales del frame, como si las texturas están activadas, si el modo de 
	ambiente está activado o si el modo de boost está activo.

	Mejoras de rendimiento:
	- Se evita el uso de punteros a funciones para beneficiarse del branch
	  predictor y del inlining, lo que mejora el rendimiento al permitir que 
	  el compilador optimice mejor el código, mejor vectorizacion y uso de SIMD
	  si el procesador tiene AVX o AVX2.
	- Se evita el uso de punteros a funciones para beneficiarse del branch
	  predictor y del inlining, lo que mejora el rendimiento al permitir que 
	  el compilador optimice mejor el código.
	- El rendirizado separado de paredes con y sin texturas permite optimizar 
	  cada caso específico, evitando cálculos innecesarios cuando las texturas 
	  no están activadas. ADemas hace que la tasa de frames sea mas estable en
	  sistemas con menos recursos, menos branch misspredictions y mejor uso del
	  del cache (mejor localidad temporal y espacial) y permite prefeching.
	- Usamos SoA (Structure of Arrays) en vez de AoS (Array of Structures) para
	  mejorar la localidad espacial y temporal de los datos, lo que resulta en
	  un mejor uso del cache y una mayor eficiencia en el acceso a memoria.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Retorno:
	- No tiene retorno. Modifica directamente el buffer de la imagen que será 
	  mostrado en la ventana.
*/
void	render_walls(t_mlx *mlx)
{
	if (mlx->frame->textures_onoff == ON)
	{
		if (mlx->frame->ambiance_onoff == OFF)
		{
			if (mlx->frame->boost == OFF)
				render_walls_tex(mlx);
			else
				render_walls_tex_speed(mlx);
		}
		else
			render_walls_ambiance(mlx);
	}
	else
		render_walls_no_textures(mlx);
}

/*
	Función para renderizar paredes con texturas.
	Itera sobre cada rayo calculado en el frame y dibuja la columna de pared 
	correspondiente utilizando texturas.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Retorno:
	- No tiene retorno. Modifica directamente el buffer de la imagen que será 
	  mostrado en la ventana.
*/
void	render_walls_tex(t_mlx *mlx)
{
	int		n_ray;
	t_ray	*ray;
	t_wall	*wall;

	ray = mlx->frame->rays;
	wall = mlx->frame->walls;
	n_ray = 0;
	while (n_ray < mlx->win_width)
	{
		draw_wall_column_tex(mlx, n_ray, &wall[n_ray], &ray[n_ray]);
		n_ray++;
	}
}

/*
	Función para renderizar paredes sin texturas.
	Itera sobre cada rayo calculado en el frame y dibuja la columna de pared 
	correspondiente sin utilizar texturas.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Retorno:
	- No tiene retorno. Modifica directamente el buffer de la imagen que será 
	  mostrado en la ventana.
*/
void	render_walls_no_textures(t_mlx *mlx)
{
	int		n_ray;
	t_ray	*ray;
	t_wall	*wall;

	ray = mlx->frame->rays;
	wall = mlx->frame->walls;
	n_ray = 0;
	while (n_ray < mlx->win_width)
	{
		draw_wall_column(mlx, n_ray, &wall[n_ray], &ray[n_ray]);
		n_ray++;
	}
}

/*
	Función para renderizar paredes con texturas en modo boost (rápido).
	Itera sobre cada rayo calculado en el frame y dibuja la columna de pared 
	correspondiente utilizando texturas, optimizado para un rendimiento 
	mejorado.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Retorno:
	- No tiene retorno. Modifica directamente el buffer de la imagen que será 
	  mostrado en la ventana.
*/
void	render_walls_tex_speed(t_mlx *mlx)
{
	int		n_ray;
	t_ray	*ray;
	t_wall	*wall;

	ray = mlx->frame->rays;
	wall = mlx->frame->walls;
	n_ray = 0;
	while (n_ray < mlx->win_width)
	{
		drawwallcoltexspeed(mlx, n_ray, &wall[n_ray], &ray[n_ray]);
		n_ray++;
	}
}

/*
	Función para renderizar paredes con texturas en modo boost (rápido).
	Itera sobre cada rayo calculado en el frame y dibuja la columna de pared 
	correspondiente utilizando texturas, optimizado para un rendimiento 
	mejorado.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Retorno:
	- No tiene retorno. Modifica directamente el buffer de la imagen que será 
	  mostrado en la ventana.
*/
void	render_walls_ambiance(t_mlx *mlx)
{
	int		n_ray;
	t_ray	*ray;
	t_wall	*wall;

	ray = mlx->frame->rays;
	wall = mlx->frame->walls;
	n_ray = 0;
	while (n_ray < mlx->win_width)
	{
		drawinglopp_tex_amb(mlx, n_ray, &wall[n_ray], &ray[n_ray]);
		n_ray++;
	}
}
