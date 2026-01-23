/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 14:45:15 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 21:50:44 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	- Renderiza el minimapa en 2D recorriendo cada píxel de la ventana del 
	  minimapa.
	- Dibuja cada píxel según si corresponde a una pared, suelo o la posición
	  del jugador.
	- Si la opción de mostrar rayos está activada, dibuja los rayos en el 
	  minimapa.
	- Escalamos los píxeles de la ventana a posiciones del mapa usando 
	  `mm_scale` y `mm_zoom`.
	- No renderizamos el minimapa durante el recorrido de los píxeles del 
	  raycasting porque sería un desperdicio de recursos. Lo hacemos después 
	  de renderizar toda la escena en 3D, ya que en ambos casos habría que 
	  sobrescribir el buffer de la imagen. Este enfoque es más modular, aunque 
	  la desventaja es que se recorre dos veces la matriz de píxeles de la zona
	  de la ventana del minimapa.
	- Calculamos el offset del jugador en el minimapa (traslación + zoom) para 
	  centrar el minimapa en la posición del jugador.
	- Calculamos el zoom escalado para usarlo en el dibujado de los píxeles del
	  minimapa.
	- Luego recorremos cada píxel de la ventana del minimapa y dibujamos el 
	  píxel correspondiente del minimapa en función de su correspondencia con
	  el mapa.
	- Dibujamos el fondo del minimapa en blanco para que no se vean restos de 
	  la escena 3D. Lo hacemos con `buffering_line` para optimizar el dibujado 
	  por bloques horizontales.
	- Finalmente, si la opción de mostrar rayos está activada, dibuja los rayos
	  en el minimapa.
*/
int	render_frame2d(t_mlx *mlx)
{
	int		win[2];
	float	scaled_zoom[2];

	update_center_minimap_offset(mlx, scaled_zoom);
	win[Y] = 0;
	while (win[Y] <= mlx->frame->mm_height)
	{
		win[X] = 0;
		buffering_line(win[Y], 0xFFFFFFFF, mlx, mlx->frame->mm_widht);
		while (win[X] <= mlx->frame->mm_widht)
		{
			draw_mini_pixel_offset(mlx, win, scaled_zoom);
			win[X]++;
		}
		win[Y]++;
	}
	if (mlx->frame->minimap_showrays == true)
		draw_rays2d(mlx, scaled_zoom);
	return (1);
}

/*
	Actualiza el offset del minimapa para centrarlo en la posición del jugador

	Detalles de implementación:
	- Calcula el offset del minimapa en función de la posición del jugador, el
	  tamaño del minimapa y su escala (traslación del minimapa + zoom).
	- El offset se calcula restando la mitad del tamaño del minimapa (origen) 
	  escalado a la posición del jugador. Esto permite centrar el minimapa en 
	  la posición actual del jugador.
	- También calcula el zoom escalado para usarlo en el dibujado de los 
	  píxeles del minimapa.

	Flujo de la función:
	1. Se obtiene la posición actual del jugador en el mapa (`pos_x` y `pos_y`)
	2. Se calcula el factor de zoom (`mm_zoom_factor`) y se aplica a las 
	   escalas del minimapa (`mm_scale`) para obtener el zoom escalado.
	3. Se calcula el offset del minimapa restando la mitad del tamaño del 
	   minimapa (en píxeles) escalado al zoom de la posición del jugador. Esto 
	   asegura que el jugador esté centrado en el minimapa.
*/
void	update_center_minimap_offset(t_mlx *mlx, float *escaled_zoom)
{
	t_frame		*f;
	float		pl_pos[2];
	float		zoom;

	f = mlx->frame;
	pl_pos[X] = mlx->player->pos_x;
	pl_pos[Y] = mlx->player->pos_y;
	zoom = mlx->frame->mm_zoom_factor;
	escaled_zoom[X] = f->mm_scale[X] * zoom;
	escaled_zoom[Y] = f->mm_scale[Y] * zoom;
	f->mm_offset[X] = pl_pos[X] - (f->mm_widht / 2) / (f->mm_scale[X] * zoom);
	f->mm_offset[Y] = pl_pos[Y] - (f->mm_height / 2) / (f->mm_scale[Y] * zoom);
}

/*
	Dibuja un píxel del minimapa en función de su correspondencia con el mapa.

	Detalles de implementación:
	- Los valores del mapa se manejan como `float` para permitir un movimiento 
	  fluido del jugador (movimiento continuo). Si se desea un movimiento celda
	  a celda, los valores deben manejarse como `int`.
	- Escala las coordenadas de los píxeles de la ventana a las posiciones del
	  mapa utilizando `mm_scaled_zoom` y el offset del minimapa.
	- Si la posición escalada y trasladada corresponde a una pared, dibuja un 
	  píxel naranja (0xFF8C00).
	- Si la posición corresponde a suelo, dibuja un píxel gris (0x969696).
	- Comprueba si el jugador está en la posición actual del minimapa y, si es 
	  así, dibuja un píxel negro (0x000000) para representarlo.

	Mejora:
	- Actualmente, la función comprueba si el jugador está en cada píxel del 
	  minimapa, lo que puede ser ineficiente. Dado que el jugador siempre está 
	  en el centro del minimapa, esta comprobación podría optimizarse para 
	  dibujarlo directamente en el centro del minimapa después de renderizar 
	  el resto del minimapa.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.
	- win: Coordenadas del píxel en la ventana del minimapa.
	- scaled_zoom: Escala del zoom aplicada al minimapa para ajustar las 
	  posiciones de los píxeles al mapa.

	Esta función permite representar el minimapa de manera precisa y fluida,
	teniendo en cuenta tanto las paredes como el suelo y la posición del 
	jugador.
*/
void	draw_mini_pixel_offset(t_mlx *mlx, int *win, float *scaled_zoom)
{
	t_frame		*f;
	float		map[2];

	f = mlx->frame;
	map[X] = f->mm_offset[X] + (float)win[X] / (scaled_zoom[X]);
	map[Y] = f->mm_offset[Y] + (float)win[Y] / (scaled_zoom[Y]);
	if ((unsigned int)map[Y] < mlx->map->max_rows
		&& (unsigned int)map[X] < mlx->map->max_columns)
	{
		if (is_wall(mlx, map) == true)
			buffering_pixel(win[X], win[Y], mlx, 0xFF8C00);
		else
			buffering_pixel(win[X], win[Y], mlx, 0x969696);
		is_person2d(mlx, win, map);
	}
}

/*
	Comprueba si una posición del mapa corresponde a una pared en el minimapa.

	Detalles:
	- Convierte las coordenadas del mapa (en `float`) a valores enteros para 
	  determinar si corresponden a una celda que contiene una pared.
	- Devuelve `true` si la celda en las coordenadas (map_x, map_y) es una 
	  pared (`WALL`), de lo contrario, devuelve `false`.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico.
	- map: Coordenadas escaladas del mapa en formato `float`.

	Esta función es modular y puede ser reemplazada por `touch_wall`, pero se 
	usa aquí para mantener la claridad y modularidad del código.
*/
bool	is_wall(t_mlx *mlx, float *map)
{
	unsigned int	map_x;
	unsigned int	map_y;

	map_x = (unsigned int)map[X];
	map_y = (unsigned int)map[Y];
	if (mlx->map->map_grids[map_y][map_x] == WALL)
		return (true);
	return (false);
}

/*
	Con esta función comprobamos si el personaje está en la región del mapa 
	después de haberlo escalado, teniendo en cuenta el volumen del personaje. 
	La usamos para realizar la comprobación en cada par de píxeles del 
	minimapa.

	Detalles de implementación:
	- La comprobación se realiza en `float` para tener en cuenta el volumen 
	  (epsilon) del personaje. Esto permite que el personaje ocupe varias 
	  posiciones dentro de una celda del mapa, logrando un movimiento continuo.
	- Si se comparase con `int`, el movimiento del personaje no sería continuo 
	  y estaría restringido a moverse celda por celda.
	- Se verifica si la posición del jugador está dentro del rango de un píxel 
	  del minimapa escalado, considerando el volumen del jugador.

	Limitación:
	- Actualmente, la función realiza la comprobación en cada píxel del 
	  minimapa, lo que puede ser ineficiente. Dado que el personaje siempre 
	  está en el centro del minimapa, esta comprobación podría optimizarse para 
	  evitar realizarla en cada píxel. En su lugar, se podría dibujar al 
	  personaje directamente en el centro del minimapa después de renderizar 
	  el resto del minimapa.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.
	- window: Coordenadas del píxel en la ventana del minimapa.
	- map: Coordenadas escaladas del mapa correspondientes al píxel actual.

	Esta función permite representar al jugador en el minimapa de manera 
	precisa, teniendo en cuenta su volumen y posición relativa en el mapa.
*/
void	is_person2d(t_mlx *mlx, int *window, float *map)
{
	if (fabs(mlx->player->pos_x - map[X]) < mlx->player->volume
		&& fabs(mlx->player->pos_y - map[Y]) < mlx->player->volume)
	{
		buffering_pixel(window[X], window[Y], mlx, 0x000000);
	}
}
