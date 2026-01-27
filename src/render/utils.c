/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 00:11:37 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/27 03:35:18 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función para contar y mostrar los FPS promedio en el archivo de log.

	Detalles:
	- Utiliza `gettimeofday` para obtener el tiempo actual en milisegundos.
	- Lleva un conteo de los frames renderizados desde el inicio del juego.
	- Cada segundo (1000 ms), calcula y muestra los FPS promedio en el archivo 
	  de log.
	- Para evitar saturar el archivo de log, solo muestra los FPS cada 140 
	  frames.
	- Guarda la marca de tiempo actual (`now_timestamp`) y la diferencia de 
	  tiempo entre frames (`del_timestamp`) en la estructura principal del 
	  juego (`mlx`).

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Retorno:
	- No tiene retorno. Escribe directamente en el archivo de log.
*/
void	fps_counter_average(t_mlx *mlx)
{
	static long long	frames;
	static long long	init_timestamp;
	struct timeval		time;
	long long			now_timestamp;
	long long			delta_time;

	gettimeofday(&time, NULL);
	if (init_timestamp == 0 || ++frames == LLONG_MAX)
	{
		init_timestamp = (long long)time.tv_sec * 1000 + (time.tv_usec / 1000);
		frames = 0;
		return ;
	}
	now_timestamp = (long long)time.tv_sec * 1000 + (time.tv_usec / 1000);
	mlx->now_timestamp = now_timestamp;
	mlx->del_timestamp = now_timestamp - mlx->bef_timestamp;
	delta_time = now_timestamp - init_timestamp;
	if (delta_time >= 1000 && (frames % 140) == 0)
	{
		write(mlx->log_fd, "[Average] FPS: ", 15);
		ft_putnbr_fd((int)(frames * 1000 / delta_time), mlx->log_fd);
		write(mlx->log_fd, "\n", 1);
	}
	mlx->bef_timestamp = now_timestamp;
}

/*
	Función para contar y mostrar los FPS en tiempo real en el archivo de log.

	Detalles:
	- Utiliza `gettimeofday` para obtener el tiempo actual en milisegundos.
	- Lleva un conteo de los frames renderizados desde el inicio del juego.
	- Cada 500 ms, calcula y muestra los FPS en tiempo real en el archivo de 
	  log.
	- Reinicia el contador de frames y la marca de tiempo inicial después de 
	  cada cálculo para mantener los FPS actualizados en tiempo real.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Retorno:
	- No tiene retorno. Escribe directamente en el archivo de log.
*/
void	fps_counter_realtime(t_mlx *mlx)
{
	static long long	frames;
	static long			init_timestamp;
	struct timeval		time;
	long				now_timestamp;
	long				delta_time;

	gettimeofday(&time, NULL);
	if (init_timestamp == 0 || frames == LLONG_MAX)
	{
		init_timestamp = time.tv_sec * 1000 + (time.tv_usec / 1000);
		frames = 0;
		return ;
	}
	frames++;
	now_timestamp = time.tv_sec * 1000 + (time.tv_usec / 1000);
	delta_time = now_timestamp - init_timestamp;
	if (delta_time >= 500)
	{
		write(mlx->log_fd, "[Realtime] FPS: ", 16);
		ft_putnbr_fd((int)(frames * 1000 / delta_time), mlx->log_fd);
		write(mlx->log_fd, "\n", 1);
		init_timestamp = now_timestamp;
		frames = 0;
	}
}

/*
	Función para determinar si un valor del mapa representa una pared
	- Recibe un valor de mapa (char)
	- Compara el valor con los defines WALL y BONUS_WALL
	- Retorna true si el valor es una pared, false en caso contrario
*/
bool	is_wall_tile(char map_value)
{
	if (map_value == WALL || map_value == BONUS_WALL)
		return (true);
	return (false);
}

/*
	Puedoo usar sizeof ya que el compilador lo hace en compilacion
	menos cuando se hace sizeof de un variable no de un tipo de dao
	Ademas podria usar la dicsion constantemente poreel compilador lo trasnforma 
	por bitwise si es potencia de dos y es recurrente
*/
void	ft_memset_int(void *s, int c, size_t n)
{
	size_t	i;
	size_t	jumps;
	int		*ptr;

	ptr = (int *)s;
	i = 0;
	jumps = n / sizeof(int);
	while (i < jumps)
	{
		*(ptr + i) = c;
		i++;
	}
}
