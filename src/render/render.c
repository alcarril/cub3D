/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:34:21 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/12 20:45:48 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

void	fps_counter_average(t_mlx *mlx);
void	fps_counter_realtime(t_mlx *mlx);

/*
	Función principal para manejar el ciclo de renderizado del motor del juego:
	- Se activa gracias al hook de mlx_loop_hook
	- Maneja el movimiento del jugador
	- Limpia el buffer de la imagen (si no se limpia no funciona bien el render)
	- Llama a la función de raycasting para renderizar la escena 3D
	- Llama a la función para renderizar el minimapa 2D si está activado
	- Finalmente, actualiza la ventana con la nueva imagen renderizada	
*/
int	game_engine(t_mlx *mlx)
{
	//Con esto controlamos que el mouse se desactive una vez dentro de la
	//apntalla no puede salir a no ser que se desactive manualmente (con
	// tecla o pulsacion deboton del mouse). Cunadno el progrma empiexza
	//pos la fuinis muse in windows sale en cada iteracion pero es mas 
	//lento porque hace la comporbacion en cada frame hasta que el mouse
	//esta dentro y cuando esta dentro tamboen lo hace
	if (mlx->player->mouse.onoff == ON)
		get_mouse_pos_and_move(mlx);
	move_player(mlx);
	ft_bzero(mlx->bit_map_address, mlx->win_height * mlx->line_length);
	mlx->frame->floor_celling(mlx);
	if (mlx->frame->raycasting_onoff == true)
		throw_rays(mlx);
	if (mlx->frame->minimap_onoff == true)
		render_frame2D(mlx);
	mlx_put_image_to_window(mlx->mlx_var, mlx->mlx_window, mlx->mlx_img, 0, 0);
	fps_counter_average(mlx);
	fps_counter_realtime(mlx);
	return (0);
}

//se puede optimizar pasando por refencia las variables
void	buffering_pixel(int x, int y, t_mlx *mlx, int color)
{
	unsigned int	offset;

	if (x < 0 || x > mlx->win_width || y < 0 || y > mlx->win_height)
		return ;
	offset = (y * mlx->line_length) + (x * (mlx->bits_per_pixel / 8));
	*(unsigned int *)(mlx->bit_map_address + offset) = color;
}

//biferiaza una linea al completo
void	buffering_line(int y, int color, t_mlx *mlx)
{
	int offset;

	if (y < 0 || y >= mlx->win_height)
		return;
	offset = y * mlx->line_length;
	ft_memset(mlx->bit_map_address + offset, color, mlx->win_width * (mlx->bits_per_pixel / 8));
}


void	fps_counter_average(t_mlx *mlx)
{
	static long long	frames;
	static long long	init_timestamp;
	struct timeval		timestamp;
	long long			now_timestamp;
	long long			delta_time;

	gettimeofday(&timestamp, NULL);
	if (init_timestamp == 0)
	{
		init_timestamp = (long long)timestamp.tv_sec * 1000 + (timestamp.tv_usec / 1000);
		frames = 0;
		return;
	}
	frames++;
	now_timestamp = (long long)timestamp.tv_sec * 1000 + (timestamp.tv_usec / 1000);
	delta_time = now_timestamp - init_timestamp;
	if (delta_time >= 1000 && (frames % 140) == 0)
	{
		write(mlx->log_fd, "[Average] FPS: ", 15);
		ft_putnbr_fd((int)(frames * 1000 / delta_time), mlx->log_fd);
		write(mlx->log_fd, "\n", 1);
	}
}

void fps_counter_realtime(t_mlx *mlx)
{
	static long long	frames;
	static long			init_timestamp;
	struct timeval		timestamp;
	long				now_timestamp;
	long				delta_time;

	gettimeofday(&timestamp, NULL);
	if (init_timestamp == 0)
	{
		init_timestamp = timestamp.tv_sec * 1000 + (timestamp.tv_usec / 1000);
		return;
	}
	frames++;
	now_timestamp = timestamp.tv_sec * 1000 + (timestamp.tv_usec / 1000);
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

