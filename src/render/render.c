/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 21:34:21 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/27 03:52:24 by alejandro        ###   ########.fr       */
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
	- Llama a la función de raycasting para renderizar la escena 3D si el 
	  raycasting está activado.
	- Renderiza el minimapa 2D si está activado.
	- Actualiza la ventana con la nueva imagen renderizada.
	- Cuenta y muestra los FPS promedio y en tiempo real en el archivo de log.

	Mejoras de microprocesador:
	- Se evita el uso de punteros a funciones para beneficiarse del branch 
	  predictor y del inlining, lo que mejora el rendimiento.
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
		render_frame3d(mlx);
	if (mlx->frame->minimap_onoff == ON)
		render_frame2d(mlx);
	mlx_put_image_to_window(mlx->mlx_var, mlx->mlx_window, mlx->mlx_img, 0, 0);
	fps_counter_average(mlx);
	fps_counter_realtime(mlx);
	return (0);
}

/*
	Función principal para renderizar el frame 3D del juego.

	Detalles:
	- Primero ejecuta el algoritmo de raycasting mediante `throw_rays`, que 
	  calcula las intersecciones de los rayos con las paredes y almacena la 
	  información necesaria para el renderizado en la estructura `frame`.
	- Se separa el lanzado de rayos del renderizado de las paredes siguiendo 
	  la arquitectura de motores gráficos profesionales. Esto permite en un 
	  futuro paralelizar las diferentes funciones del motor en distintos hilos 
	  si se usan bibliotecas de hilos en vez de la API de mlx. Es una cuestión 
	  de mantenimiento y escalabilidad, es pura arquitectura de motores 
	  gráficos.
	- Además, esto nos permite calcular los límites de las paredes durante el 
	  raycasting, de modo que en el renderizado del suelo y el techo no se 
	  vuelvan a bufferizar píxeles que serán sobrescritos por las paredes, 
	  mejorando significativamente el rendimiento.
	- Dependiendo de las configuraciones activas, selecciona la estrategia 
	  óptima para renderizar el suelo y el techo:
	  - Si están activadas las texturas y la ambientación, usa 
		`render_floor_and_ceiling_amb` para aplicar efectos de ambiente 
		(sombreado y niebla basados en la distancia).
	  - Si está activado el modo boost y el mouse ha estado en la ventana, 
		usa `render_floor_and_ceiling_speed` para un renderizado rápido por 
		líneas completas, evitando bufferizar píxeles que serán sobrescritos 
		por las paredes. Esto proporciona mejoras de rendimiento notables en 
		mapas cerrados donde el horizonte apenas es visible.
	  - En caso contrario, usa `render_floor_and_ceiling` para un renderizado 
		estándar píxel a píxel.
	- Finalmente, renderiza las paredes mediante `render_walls`, que utiliza 
	  la información calculada por el raycasting para dibujar las texturas de 
	  las paredes con la altura y posición correctas.

	Organización del renderizado:
	- El orden de renderizado es importante: primero el suelo y el techo, 
	  luego las paredes. Esto permite que las paredes sobrescriban los píxeles 
	  del fondo, creando la ilusión de profundidad.
	- La estrategia de renderizado se selecciona en tiempo de ejecución para 
	  adaptarse a las configuraciones del usuario y optimizar el rendimiento.

	Mejoras de microprocesador:
	- Se evita el uso de punteros a funciones para beneficiarse del branch 
	  predictor y del inlining, lo que mejora el rendimiento al permitir que 
	  el compilador optimice mejor el código.
	- La selección de la estrategia de renderizado mediante condicionales 
	  simples permite al procesador predecir mejor las ramas y reducir los 
	  stalls del pipeline.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  el estado del frame, las configuraciones de renderizado y toda la 
	  información del juego.

	Retorno:
	- No tiene retorno. Modifica directamente el buffer de la imagen que será 
	  mostrado en la ventana.
*/
void	render_frame3d(t_mlx *mlx)
{
	throw_rays(mlx);
	if (mlx->frame->textures_onoff && mlx->frame->ambiance_onoff)
		render_floor_and_ceiling_amb(mlx);
	else if (mlx->frame->boost && mlx->has_been_mouse_in_window)
		render_floor_and_ceiling_speed(mlx);
	else
		render_floor_and_ceiling(mlx);
	render_walls(mlx);
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
