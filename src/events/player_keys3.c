/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_keys3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 23:49:52 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 06:12:12 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Esta funcion hace que el jugador haga un groundpound al presionar la
	tecla Mayus (Shift izquierdo).
	- Si las fisicas del motor grafico estan desactivada, no hace nada.
	- Si el jugador esta en el aire, volando, arrastrando, esprintando o 
	  escondido, no hace nada.
	- Si el jugador no esta haciendo groundpound:
		- Establece la altura de la camara a FLOOR_Z.
		- Establece la velocidad maxima del jugador a MAX_PLAYER_SPEED * 0.20f.
		  (para que baje la velocidad al hacer groundpound) Tanto en el modo 
		  con fisicas normal como en el diferencial de las componentes X e Y 
		  de la velodidad n(dukedoom)
		- Marca el estado de estar haciendo groundpound como verdadero.
		- Informa por la terminal (stdout) que el jugador esta haciendo 
		  groundpound.
	- Si el jugador ya esta haciendo groundpound:
		- Establece la altura de la camara a MIDDLE_Z.
		- Establece la velocidad maxima del jugador a MAX_PLAYER_SPEED.
		- Marca el estado de estar haciendo groundpound como falso.
		- Informa por la terminal (stdout) que el jugador se ha levantado.
*/
void	player_blockmayus_keypress(t_mlx *mlx)
{
	t_player	*pl;

	pl = mlx->player;
	if (mlx->frame->phisics_onoff == OFF)
		return ;
	if (pl->is_onair || pl->is_dragging || pl->is_sprinting || pl->is_hided)
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

/*
	Esta funcion activa el modo vuelo del jugador al presionar la tecla Q.
	- Si las fisicas del motor grafico estan desactivada, no hace nada.
	- Si el jugador esta arrastrando, escondido o haciendo groundpound, 
	  no hace nada.
	- Si el jugador no esta volando:
		- Establece la altura de la camara a MAP_MAX_Z.
		- Marca el estado de estar volando como verdadero.
		- Informa por la terminal (stdout) que el modo vuelo esta activado.
	- Si el jugador ya esta volando:
		- Establece la altura de la camara a MIDDLE_Z.
		- Marca el estado de estar volando como falso.
		- Informa por la terminal (stdout) que el modo vuelo esta desactivado.
*/
void	player_q_keypress(t_mlx *mlx)
{
	static float	prev_floor_friction;
	t_player		*pl;

	pl = mlx->player;
	if (mlx->frame->phisics_onoff == OFF)
		return ;
	if (pl->is_dragging || pl->is_groundpound || pl->is_hided)
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
