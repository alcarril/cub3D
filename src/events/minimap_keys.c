/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_keys.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 21:16:10 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 21:47:17 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Funcion para togglear el minimapa
	- Si el minimapa esta on, lo apaga
	- Si el minimapa esta off, lo enciende
	- Informa al usuario del cambio por terminal (stdout)
*/
void	toggle_minimap(t_mlx *mlx)
{
	if (mlx == NULL || mlx->frame == NULL)
		return ;
	if (mlx->frame->minimap_onoff == true)
	{
		mlx->frame->minimap_onoff = false;
		printf("MINIMAP OFF\n");
	}
	else
	{
		mlx->frame->minimap_onoff = true;
		printf("MINIMAP ON\n");
	}
}

/*
	Funcion para togglear la visualizacion de rayos
	en el minimapa
	- Si los rayos estan on, los apaga
	- Si los rayos estan off, los enciende
	- Informa al usuario del cambio por terminal (stdout)
*/
void	toggle_rays(t_mlx *mlx)
{
	if (mlx->frame->minimap_showrays == true)
	{
		mlx->frame->minimap_showrays = false;
		printf("MINIMAP RAYS OFF\n");
	}
	else
	{
		mlx->frame->minimap_showrays = true;
		printf("MINIMAP RAYS ON\n");
	}
}

/*
	Funcion para hacer zoom in o zoom out
	en el minimapa
	- Si el minimapa esta off, no se puede hacer zoom
	  y se informa al usuario por terminal (stdout)
	- Si flag es true, se hace zoom in
	- Si flag es false, se hace zoom out
	- Se informa al usuario del cambio por terminal (stdout)
	- Se vuelve a renderizar el frame 2D para actualizar
	  el minimapa con el nuevo zoom ya que solo ocupa una parte
	  de la ventana y no es necesario renderizar todo el frame 3D
	  para actualizar el minimapa. Mejora de eficiencia.
*/
void	minimap_zoom(t_mlx *mlx, bool flag)
{
	if (mlx->frame->minimap_onoff == false)
	{
		printf("WARNING: Minimap is off, cannot zoom\n");
		return ;
	}
	if (flag == true)
	{
		mlx->frame->mm_zoom_factor += 0.25f;
		if (mlx->frame->mm_zoom_factor > MINIMAP_MAX_ZOOM)
			mlx->frame->mm_zoom_factor = MINIMAP_MAX_ZOOM;
		printf("MINIMAP ZOOM  IN FACTOR: %.2f\n", mlx->frame->mm_zoom_factor);
		render_frame2d(mlx);
	}
	else
	{
		mlx->frame->mm_zoom_factor -= 0.25f;
		if (mlx->frame->mm_zoom_factor < MINIMAP_MIN_ZOOM)
			mlx->frame->mm_zoom_factor = MINIMAP_MIN_ZOOM;
		printf("MINIMAP ZOOM  OUT FACTOR: %.2f\n", mlx->frame->mm_zoom_factor);
		render_frame2d(mlx);
	}
}
