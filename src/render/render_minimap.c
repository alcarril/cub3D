/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/30 14:45:15 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/13 15:33:11 by alejandro        ###   ########.fr       */
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
	  de la zona de la ventana del minmapa.
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
		draw_rays2D(mlx);
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
	- Calculamos las traslacion del los puntos del minimapa respecto a la posicion
	  del jugador, para eso restamos la posicon delorigen (cventro del minimapa) a
	  la posicion del jugador
	- Despues escalamos los puntos de la ventana a las posiciones del mapa con mm_scale
	- y apicamos el factor de zoom del minimapa
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
	Esta funcion se usa en el caso de que queramos hacer de la matriz de pixeles 
	del mapa, poder saber si los pixeles son parte de la zona del minimapay aprovechar
	ese recorrido para bufferizar esos pixeles de la parte del minimapa.
	- Comprobar si el píxel está dentro de los límites del minimapa
	
*/
bool is_minimapzone(int win_x, int win_y, t_mlx *mlx)
{
	int minimap_start_x = 0;
	int minimap_start_y = 0;

	if (win_x >= minimap_start_x && win_x <= mlx->frame->mm_widht &&
		win_y >= minimap_start_y && win_y <= mlx->frame->mm_height)
			return (true);
	return (false);
}


