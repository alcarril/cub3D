/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:34:21 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/21 13:48:47 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

void	ft_memset_int(void *s, int c, size_t n);

/*
	Función principal para manejar el ciclo de renderizado del motor del juego:
	- Se activa gracias al hook de mlx_loop_hook
	- Evaluada si el mouse está activado y obtiene su posición para mover al jugador
	 (mas eficiente que hacerlo por evento de movimiento del mouse)
	- Maneja el movimiento del jugador
	- Limpia el buffer de la imagen (si no se limpia no funciona bien el render)
	- Pinta el suelo y el techo segun a que funcion apunte el puntero floor_celling
	  de la estructura frame
	- Llama a la función de raycasting para renderizar la escena 3D si el raycasting
	  está activado
	- Llama a la función para renderizar el minimapa 2D si está activado
	- Finalmente, actualiza la ventana con la nueva imagen renderizada	
	- Cuenta y muestra los FPS promedio y en tiempo real en el log file
	- Retorna 0 para indicar que la función se ejecutó correctamente
	Mejoras de microprocesador:
	 -Se evitan los punteros a funciones para beneficiarnos de brach predicor e inlinning
	 - Se limpia la imagen con bezero escribiendo por ints o long o no se limpia haciendo
	 que los fps se dispare. Mejora de eficionecia bufferizacion menos accesos de memeria
	 winth * sizeo(int) * filas menos accesos de memria que cunado usas ft_bzero que escribe
	 de char en char pone romcparacion de ejemplo
*/
int	game_engine(t_mlx *mlx)
{
	if (mlx->player->mouse.onoff == ON)
		get_mouse_pos_and_move(mlx);
	move_player(mlx);
	bzero(mlx->bit_map_address, mlx->win_height * mlx->line_length);
	if (mlx->frame->raycasting_onoff == ON)
	{
		if (mlx->frame->textures_onoff == ON && mlx->frame->ambiance_onoff == ON)
			render_floor_and_ceiling_amb(mlx);
		else if (mlx->frame->boost == ON)
			render_floor_and_ceiling_speed(mlx);
		else
			render_floor_and_ceiling(mlx);
		throw_rays(mlx);
	}
	if (mlx->frame->minimap_onoff == ON)
		render_frame2D(mlx);
	mlx_put_image_to_window(mlx->mlx_var, mlx->mlx_window, mlx->mlx_img, 0, 0);
	fps_counter_average(mlx);
	fps_counter_realtime(mlx);
	return (0);
}


/*
	Función para biferiar un pixel en la imagen
	- Verifica que las coordenadas estén dentro de los límites de la ventana
	- Calcula el offset en el buffer de la imagen basado en las coordenadas (x, y)
	- Escribe el color en la posición calculada del buffer casteando el puntero
	  de la imagen (char *) a un puntero de unsigned int * y asignando el color
	Mejora de microprocesador:
	- Se sustituye mlx->bits_per_pixel / 8 por mlx->bits_per_pixel >> 3 para evitar
	  la división y mejorar el rendimiento
	- Como es una funcion que se llama por cada pixe (600 x 600 = 3600000) llamadas por
	  frame, hemos optimizado el uso de las variabels locales para que sea estaticas
	  y solo se tenga que desrefrenciar la memria una vez el la primera llamada, asi los
	  datos pueden ser colocados por el microporcesador en registrso que on mucho mas rapidos
	- Ademas hemos quitadp las condicion de chkeo de pixeles aunque el brabch predictor
	lo hibiese arreglado a la larga
*/
void buffering_pixel(int x, int y, t_mlx *mlx, int color)
{
	unsigned int offset;
	static char *bitmap_address;
	static int line_length; 
	static int bpp;
	
	if (bpp == 0)
	{
		bitmap_address = mlx->bit_map_address;
		line_length = mlx->line_length;
		bpp = mlx->bits_per_pixel >> 3;
	}
	offset = (y * line_length) + (x * bpp);
	*(unsigned int *)(bitmap_address + offset) = color;
}

/*
	Función para biferiar una línea completa en la imagen
	- Verifica que la coordenada y esté dentro de los límites de la ventana
	- Calcula el offset en el buffer de la imagen basado en la coordenada y
	- Usa ft_memset para llenar toda la línea con el color especificado
	- Mas eficiente que biferiar pixel a pixel en un bucle
	- Solo se usa para rellenar lineas completas del bufffer de la imagen
	  cuando tiene un mismo color (suelo y techo principalmente cuando no
	  estan texturizados)
	- Se podria utilizar para dibujar las paredes cuadno no estan texturizadas
	Mejora de microprocesador:
	- Se reduce la sobrecarga de llamadas a funciones al pintar líneas completas
	  en lugar de píxeles
		Mejora de microprocesador:
	- Se sustituye mlx->bits_per_pixel / 8 por mlx->bits_per_pixel >> 3 para evitar
	  la división y mejorar el rendimiento
	- Como es una funcion que se llama por cada pixe (600 x 600 = 3600000) llamadas por
	  frame, hemos optimizado el uso de las variabels locales para que sea estaticas
	  y solo se tenga que desrefrenciar la memria una vez el la primera llamada, asi los
	  datos pueden ser colocados por el microporcesador en registrso que on mucho mas rapidos
	- Ademas hemos quitadp las condicion de chkeo de pixeles aunque el brabch predictor
	lo hibiese arreglado a la larga
*/
void	buffering_line(int y, int color, t_mlx *mlx, int width)
{
	unsigned int offset;
	static char *bitmap_address;
	static int line_length; 
	static int bpp;

	if (bpp == 0)
	{
		bitmap_address = mlx->bit_map_address;
		line_length = mlx->line_length;
		bpp = mlx->bits_per_pixel >> 3;
	}
	offset = y * line_length;
	ft_memset_int(bitmap_address + offset, color, width * bpp);
}

/*
	Función para contar y mostrar los FPS promedio en el log file
	- Utiliza gettimeofday para obtener el tiempo actual
	- Cuenta los frames renderizados desde el inicio
	- Cada segundo (1000 ms) calcula y muestra los FPS promedio en el log file
	- Solo muestra el FPS cada 140 frames para evitar saturar el log
	- Guarda la marca de tiempo actual (now_timestamp) y la diferencia de timepo en frames entre
	  frame y frame (del_timestamp) en la estrcutura porncipal del juego (mlx).
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
		return;
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
	Función para contar y mostrar los FPS en tiempo real en el log file
	- Utiliza gettimeofday para obtener el tiempo actual
	- Cuenta los frames renderizados desde el inicio
	- Cada 500 ms calcula y muestra los FPS en tiempo real en el log file
*/
void fps_counter_realtime(t_mlx *mlx)
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
		return;
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
