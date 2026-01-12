/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_keys.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 21:16:10 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/12 21:16:30 by alejandro        ###   ########.fr       */
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