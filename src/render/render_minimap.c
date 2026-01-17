/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 14:45:15 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/16 21:19:11 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

void	update_center_minimap_offset(t_mlx *mlx, float *escaled_zoom);

/*
	- Renderiza el minimapa en 2D recorriendo cada píxel de la ventana del minimapa.
	- Dibuja cada píxel según si corresponde a una pared, suelo o la posición del jugador.
	- Si la opción de mostrar rayos está activada, dibuja los rayos en el minimapa.
	- Escalamos los pixeles de la ventana a posisocnes del mapa usando mm_scale A->B
	- NO renderizamos el minimpaa en el recorrido de los pixeles de raycasting porque
	  seria un desperdicio de recursos, lo hacemos despues de renderizar toda la escena
	  en 3D. Ya que en ambos casos habria que sobrescribir el buffer de la imagen.
	  Es mas modular aunque la desventaja es que se recorre dos veces la matriz de pixeles
	  de la zona de la ventana del minmapa
	- Calculamos el offset del jugador en el minimapa (traslacion + zoom) para centrar
	  el minimapa en la posicion del jugador
	- Calculamos el zoom escalado para usarlo en el dibujado de los pixeles del minimapa
	- Luego recorremos cada pixel de la ventana del minimapa y dibujamos el pixel
	  correspondiente del minimapa en funcion de su correspondencia con el mapa
	- Dibujamos el fondo del minimapa en blanco para que no se vean restos de la escena 3D
	 Lo hacemos con buffer line para optimizar al dibujar por bloques horizontales
	- Finalmente, si la opción de mostrar rayos está activada, dibuja los rayos en el minimapa.
*/
int	render_frame2D(t_mlx *mlx)
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
		draw_rays2D(mlx, scaled_zoom);
	return (1);
}

/*
	Actualiza el offset del minimapa para centrarlo en la posicion del jugador
	Calculamos el offset del minimapa en funcion de la posicion del jugador
	y el tamaño del minimapa y su escala. (traslacion del minimapa + zoom)
	El offset se calcula restando la mitad del tamaño del minimapa (orihgen) escalado
	a la posicion del jugador para centrar el minimapa en el jugador.
	Ademas calculamos el zoom escalado para usarlo en el dibujado de los pixeles
	del minimapa.
	- Calculamos las traslacion del los puntos del minimapa respecto a la posicion
	  del jugador, para eso restamos la posicon delorigen (cventro del minimapa) a
	  la posicion del jugador
*/
void	update_center_minimap_offset(t_mlx *mlx, float *escaled_zoom)
{
	t_frame		*f;
	float	pl_pos[2];
	float	zoom;

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
	- Dibuja un píxel del minimapa en función de su correspondencia con el mapa.
	- Los valores del mapa en float para permitir un movimiento fluido del jugador 
		(mov continuo) si quremos que sea celda a celda tiene que ser con ints.
	- Despues escalamos los puntos de la ventana a las posiciones del mapa con mm_scaled zoom
	- Si la posición escalada  y trasladadas corresponde a una pared, dibuja un píxel naranja.
	- Si es suelo, dibuja un pixel negro/gris.
*/
void	draw_mini_pixel_offset(t_mlx *mlx, int *win, float *scaled_zoom)
{
	t_frame		*f;
	float		map[2];
	
	f = mlx->frame;
	map[X] = f->mm_offset[X] + (float)win[X] / (scaled_zoom[X]);
	map[Y] = f->mm_offset[Y] + (float)win[Y] / (scaled_zoom[Y]);
	if ((unsigned int)map[Y] < mlx->map->max_rows &&
			(unsigned int)map[X] < mlx->map->max_columns)
	{
		if (is_wall(mlx, map) == true)
			buffering_pixel(win[X], win[Y], mlx, 0xFF8C00);
		else
			buffering_pixel(win[X], win[Y], mlx, 0x969696);
		is_person2D(mlx, win, map);
	}
}


/*
	Con esta funcion comprobamos si una posicion del mapa es una pared o no
	para pintarla en el minimapa.
	Se puede sustituir por tpuch_wall pero asi es mas modular
	Pasamos las posicion del pixel a int para saber si corresponde a una 
	pared dentro del conjusnto de posiciones(i,j) del mapa (discretas)
*/
bool is_wall(t_mlx *mlx, float *map)
{
	unsigned int map_x;
	unsigned int map_y;
	
	map_x = (unsigned int)map[X];
	map_y = (unsigned int)map[Y];
	if (mlx->map->map_grids[map_y][map_x] == WALL)
		return (true);
	return (false);
}

/*
	Con esta funcion comprobamso si el personaje esta en la region del mapa despues
	de haberlo escalado teneindo en cuenta el volumen del mapa. La usamos para hacer
	ls comporbacion en cada par de pixeles del minimapa.
	Se hace la comprobacion en float para tener en cuenta el volumen (epsilon) del personaje
	y que el personaje pueda estar en varias posiciones dentro de una celda del mapa. Si se
	comparase con ints el moviento del persona no seria ocntinuo
	Se comprueba en cada combinacion de punetos x,y de los pixeles escalados al minimapa (es menos
	eficiente que hacer un draw person2D que dibujaria elpersonaje despues de renderizar
	el minimaapa en la siguiente capa de renerizado)
*/
void	is_person2D(t_mlx *mlx, int *window, float *map)
{
	if (fabs(mlx->player->pos_x - map[X]) < mlx->player->volume &&
			fabs(mlx->player->pos_y - map[Y]) < mlx->player->volume)
	{
		buffering_pixel(window[X], window[Y], mlx, 0x000000);
	}
}
