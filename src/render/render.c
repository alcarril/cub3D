/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:34:21 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/24 21:14:07 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función principal para manejar el ciclo de renderizado del motor del juego.

	Detalles:
	- Esta función es llamada por el hook de `mlx_loop_hook` y se ejecuta en 
	  cada frame del juego.
	- Si el mouse está activado, obtiene su posición y mueve al jugador en 
	  consecuencia. Esto es más eficiente que manejar el movimiento del mouse 
	  mediante eventos.
	- Maneja el movimiento del jugador en función de las teclas presionadas.
	- Limpia el buffer de la imagen antes de renderizar el nuevo frame. Si no 
	  se limpia, el renderizado no funcionará correctamente.
	- Renderiza el suelo y el techo utilizando la función apuntada por el 
	  puntero `floor_ceiling` en la estructura `frame`. Dependiendo de las 
	  configuraciones, puede usar:
	  - `render_floor_and_ceiling_amb` para aplicar efectos de ambiente.
	  - `render_floor_and_ceiling_speed` para un renderizado más rápido.
	  - `render_floor_and_ceiling` para un renderizado estándar.
	- Llama a la función de raycasting para renderizar la escena 3D si el 
	  raycasting está activado.
	- Renderiza el minimapa 2D si está activado.
	- Actualiza la ventana con la nueva imagen renderizada.
	- Cuenta y muestra los FPS promedio y en tiempo real en el archivo de log.

	Mejoras de microprocesador:
	- Se evita el uso de punteros a funciones para beneficiarse del branch 
	  predictor y del inlining, lo que mejora el rendimiento.
	- Se limpia el buffer de la imagen utilizando `bzero` o escribiendo por 
	  bloques de `int` o `long`. Esto mejora la eficiencia al reducir los 
	  accesos a memoria (ancho * sizeof(int) * filas), en comparación con 
	  `ft_bzero`, que escribe byte por byte.
	- Aunque se podría usar `bzero` debido a las optimizaciones del sistema 
	  operativo (como marcar las páginas de memoria como "dirty"), replicar 
	  este comportamiento sería muy complejo. Además, con `memsetboost`, la 
	  mejora no es tan notable como con `bzero`. Esto se debe a que el sistema 
	  operativo optimiza las escrituras en memoria, marcando las páginas como 
	  "dirty" y evitando leer datos antiguos antes de escribir nuevos datos.

	Retorno:
	- Retorna 0 para indicar que la función se ejecutó correctamente.
*/
int	game_engine(t_mlx *mlx)
{
	if (mlx->player->mouse.onoff == ON)
		get_mouse_pos_and_move(mlx);
	move_player(mlx);
	if (mlx->frame->raycasting_onoff == ON)
	{
		if (mlx->frame->textures_onoff && mlx->frame->ambiance_onoff)
			render_floor_and_ceiling_amb(mlx);
		else if (mlx->frame->boost && mlx->has_been_mouse_in_window)
			render_floor_and_ceiling_speed(mlx);
		else
			render_floor_and_ceiling(mlx);
		throw_rays(mlx);
	}
	if (mlx->frame->minimap_onoff == ON)
		render_frame2d(mlx);
	mlx_put_image_to_window(mlx->mlx_var, mlx->mlx_window, mlx->mlx_img, 0, 0);
	fps_counter_average(mlx);
	fps_counter_realtime(mlx);
	return (0);
}

/*
	Función para bufferizar un píxel en la imagen.

	Detalles:
	- Verifica que las coordenadas (x, y) estén dentro de los límites de la 
	  ventana.
	- Calcula el offset en el buffer de la imagen basado en las coordenadas 
	  (x, y) y las propiedades de la imagen (`line_length` y `bits_per_pixel`).
	- Escribe el color en la posición calculada del buffer, utilizando un 
	  puntero a la dirección de memoria del buffer de la imagen.

	Mejoras de microprocesador:
	- Se sustituye `mlx->bits_per_pixel / 8` por `mlx->bits_per_pixel >> 3` 
	  para evitar la división y mejorar el rendimiento.
	- Como esta función se llama por cada píxel (por ejemplo, 600x600 = 
	  360,000 llamadas por frame), se optimiza el uso de variables locales 
	  estáticas. Esto permite que las variables se inicialicen solo una vez y 
	  se almacenen en registros del procesador, lo que mejora el rendimiento.
	- Se elimina la condición de verificación de los píxeles para evitar 
	  penalizaciones por el branch predictor, aunque este podría optimizarlo a 
	  largo plazo.

	Parámetros:
	- x, y: Coordenadas del píxel en la ventana.
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.
	- color: Color del píxel en formato hexadecimal.

	Retorno:
	- No tiene retorno. Escribe directamente en el buffer de la imagen.
*/
void	buffering_pixel(int x, int y, t_mlx *mlx, int color)
{
	unsigned int	offset;
	static char		*bitmap_address;
	static int		line_length;
	static int		bpp;

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
	Función para bufferizar una línea completa en la imagen.

	Detalles:
	- Verifica que la coordenada `y` esté dentro de los límites de la ventana.
	- Calcula el offset en el buffer de la imagen basado en la coordenada `y`.
	- Utiliza `ft_memfillboost` para llenar toda la línea con el color 
	  especificado.
	- Es más eficiente que bufferizar píxel a píxel en un bucle.
	- Se utiliza principalmente para rellenar líneas completas del buffer de 
	  la imagen con un mismo color (por ejemplo, para renderizar el suelo y el 
	  techo cuando no están texturizados).
	- También podría usarse para dibujar paredes cuando no están texturizadas.

	Mejoras de microprocesador:
	- Reduce la sobrecarga de llamadas a funciones al pintar líneas completas 
	  en lugar de píxeles individuales.
	- Se sustituye `mlx->bits_per_pixel / 8` por `mlx->bits_per_pixel >> 3` 
	  para evitar la división y mejorar el rendimiento.
	- Se elimina la condición de verificación de los píxeles para evitar 
	  penalizaciones por el branch predictor, aunque este podría optimizarlo a 
	  largo plazo.
	- Como esta función se llama por cada línea (por ejemplo, 600 llamadas por 
	  frame en una ventana de 600 píxeles de alto), se optimiza el uso de 
	  variables locales estáticas. Esto permite que las variables se 
	  inicialicen solo una vez y se almacenen en registros del procesador, lo 
	  que mejora el rendimiento.
	- Con la función `ft_memfillboost`, se optimiza aún más la escritura en 
	  memoria al escribir por bloques de `int` o `long` en lugar de byte a 
	  byte, como hace `ft_memset`. Esto también permite un mejor alineamiento 
	  de memoria, mejorando el rendimiento.

	Parámetros:
	- y: Coordenada de la línea en la ventana.
	- color: Color de la línea en formato hexadecimal.
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.
	- width: Ancho de la línea a dibujar (normalmente el ancho de la ventana).

	Retorno:
	- No tiene retorno. Escribe directamente en el buffer de la imagen.
*/
void	buffering_line(int y, int color, t_mlx *mlx, int width)
{
	unsigned int	offset;
	static char		*bitmap_address;
	static int		line_length;
	static int		bpp;

	if (bpp == 0)
	{
		bitmap_address = mlx->bit_map_address;
		line_length = mlx->line_length;
		bpp = mlx->bits_per_pixel >> 3;
	}
	offset = y * line_length;
	ft_memfillboost(bitmap_address + offset, color, width * bpp);
}

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
