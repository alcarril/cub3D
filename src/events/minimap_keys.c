/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_keys.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 21:16:10 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/20 23:28:09 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Funcion para togglear minimapa
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
	Funcion para togglear rayos en minimapa
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
	Funcion para hacer zoom in o zoom out en el minimapa
	Podri ameter opctimizcion para que no se renderizase toda la pantalla+y lo la parte del
	minimapa (como esun accion que se hace ada poco tiempo mejor asi)
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
		render_frame2D(mlx);
	}
	else
	{
		mlx->frame->mm_zoom_factor -= 0.25f;
		if (mlx->frame->mm_zoom_factor < MINIMAP_MIN_ZOOM)
		mlx->frame->mm_zoom_factor = MINIMAP_MIN_ZOOM;
		printf("MINIMAP ZOOM  OUT FACTOR: %.2f\n", mlx->frame->mm_zoom_factor);
		render_frame2D(mlx);
	}
}