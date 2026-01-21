/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_keys3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 23:49:52 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/20 23:50:51 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

void	player_blockmayus_keypress(t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
		return ;
	if (mlx->player->is_onair || mlx->player->is_flying || mlx->player->is_dragging || mlx->player->is_sprinting || mlx->player->is_hided)
		return ;
	if (mlx->player->is_groundpound == false)
	{
		mlx->player->camz = FLOOR_Z;
		mlx->player->max_speed_dt = MAX_PLAYER_SPEED * 0.20f;
		mlx->player->max_speed_a[X] = MAX_PLAYER_SPEED * 0.20f;
		mlx->player->max_speed_a[Y] = MAX_PLAYER_SPEED * 0.20f;
		mlx->player->is_groundpound = true;
		printf("ON GROUNDPOUND\n");
	}
	else
	{
		mlx->player->camz = MIDDLE_Z;
		mlx->player->max_speed_dt = MAX_PLAYER_SPEED;
		mlx->player->max_speed_a[X] = MAX_PLAYER_SPEED;
		mlx->player->max_speed_a[Y] = MAX_PLAYER_SPEED;
		mlx->player->is_groundpound = false;
		printf("STAND UP\n");
	}
}

void	player_q_keypress(t_mlx *mlx)
{
	static float  prev_floor_friction;
	
	if (mlx->frame->phisics_onoff == OFF)
		return ;
	if (mlx->player->is_dragging || mlx->player->is_groundpound || mlx->player->is_hided)
		return ;
	if (prev_floor_friction == 0.0f)
		prev_floor_friction = mlx->phisics.floor_friction;
	if (mlx->player->is_flying == false)
	{
		mlx->player->camz = MAP_MAX_Z;
		mlx->player->is_flying = true;
		printf("FLY MODE ON\n");
	}
	else
	{
		mlx->player->camz = MIDDLE_Z;
		mlx->player->is_flying = false;
		printf("FLY MODE OFF\n");
	}
	
}