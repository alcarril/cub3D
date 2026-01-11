/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 13:06:24 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/11 13:29:26 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Funcion que maneja la presion de teclas para el movimiento del jugador
*/
bool	player_keypress(t_mlx *mlx, int keysym)
{
	if (keysym == XK_w)
		mlx->player->keys.move_up = true;
	else if (keysym == XK_s)
		mlx->player->keys.move_down = true;
	else if (keysym == XK_a)
		mlx->player->keys.move_left = true;
	else if (keysym == XK_d)
		mlx->player->keys.move_right = true;
	else if (keysym == XK_Left)
		mlx->player->keys.r_counterclockwise = true;
	else if (keysym == XK_Right)
		mlx->player->keys.r_clockwise = true;
	else if (keysym == XK_Shift_L && mlx->player->keys.sprint == false)
	{
		mlx->player->speed += 0.05;
		mlx->player->keys.sprint = true;
		printf("SPRINT ON\n");
	}
	else if (keysym == XK_v)
		change_fov(mlx);
	else
		return (false);
	return (true);
}
/*
	Funcion para cambiar el fov del jugador
*/
void	change_fov(t_mlx *mlx)
{
	if (mlx->player->fov == 60.0f)
	{
		mlx->player->fov = 90.0f;
		mlx->player->rad_fov = 90.0f * (PI / 180.0f);
		printf("FOV 90\n");
	}
	else
	{
		mlx->player->fov = 60.0f;
		mlx->player->rad_fov = 60.0f * (PI / 180.0f);
		printf("FOV 60\n");
	}
}

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
	Funcion para togglear correccion de fish eye
*/
void	toggle_fish_eye(t_mlx *mlx)
{
	if (mlx->frame->draw_walls == draw_wall_column)
	{
		if (mlx->frame->fish_eye == true)
		{
			mlx->frame->fish_eye = false;
			printf("FISH EYE CORRECTION OFF\n");
		}
		else
		{
			mlx->frame->fish_eye = true;
			printf("FISH EYE CORRECTION ON\n");
		}
	}
	else
		printf("WARNING!!: Textures must be off to toggle fish eye effect\n");
}

