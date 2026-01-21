/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   difspeed_vecmove_phisics.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:03:22 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/21 18:46:22 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"


/*
	Funcion que actualiza la velocidad del jugador en funcion de las teclas
	pulsadas y aplica el moviento vectorial en funcion de la velocidad
	actual del jugador. Si el jugador esta en el aire se desactivan las aceler
	aciones en el plano 2D y se aplica la fisica del aire que lo desacelera
	poco a poco. per intiendo un moviento parabolico en z.
*/
void	difspeed_and_vecmove(t_mlx *mlx, long long delta_time)
{
	t_player	*pl;
	bool		is_moving;
	
	pl = mlx->player;
	is_moving = 0;
	if (mlx->player->is_onair == false)
	{
		if (pl->keys.move_up == true)
			acelerate_player(mlx, FRONT, delta_time, &is_moving);
		if (pl->keys.move_down == true)
			acelerate_player(mlx, BACK, delta_time, &is_moving);
		if (pl->keys.move_left == true || pl->keys.move_right == true)
			acelerate_player(mlx, SIDES, delta_time, &is_moving);
		decelerate_player(mlx, delta_time, is_moving);
		vectorization(mlx->player, delta_time, pl->speed_dt, pl->rad_angle);
	}
	else
	{
		decelerate_player_air(mlx, delta_time);
		airborne_vectorization(pl, delta_time, false);
	}
}


/*
	Funcion que actualiza la velocidad del jugador en funcion de las teclas
	pulsadas y aplica el moviento vectorial en funcion de la velocidad
	actual del jugador. Si el jugador esta en el aire se desactivan las aceler
	aciones en el plano 2D y se aplica la fisica del aire que lo desacelera
	poco a poco. per intiendo un moviento parabolico en z.
	Modo de moviento dukedoom con aceleraciones diferenciales en cada eje.
*/
void	difvspeed_and_vecmove_nukedoom(t_mlx *mlx, long long delta_time)
{
	t_player	*pl;
	bool		is_moving;
	
	pl = mlx->player;
	is_moving = 0;
	if (mlx->player->is_onair == false)
	{
		if (pl->keys.move_up == true)
			acelerate_dukedoom(mlx, FRONT, delta_time, &is_moving);
		if (pl->keys.move_down == true)
			acelerate_dukedoom(mlx, BACK, delta_time, &is_moving);
		if (pl->keys.move_left == true || pl->keys.move_right == true)
			acelerate_dukedoom(mlx, SIDES, delta_time, &is_moving);
		decelerate_dukedoom(mlx, delta_time, is_moving);
		vectorization_dukedoom(mlx->player, delta_time, pl->rad_angle);
	}
	else
	{
		decelerate_dukedoom_air(mlx, delta_time);
		airborne_vectorization(pl, delta_time, true);
	}
}