/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_keys2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 19:49:04 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 06:08:02 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Esta funcion activa el sprint del jugador al presionar la tecla Shift.
	- Si el jugador esta volando, arrastrando, escondido o haciendo groundpound, 
	  no hace nada.
	- Si la fisicas del motor grafico estan desactivada, aumenta la velocidad 
	  del jugador en 0.05
	- Si las fisicas estan activadas, aumenta la aceleracion frontal del 
	  jugador y establece la velocidad maxima a MAX_PLAYER_SPEED_STRINT.
	  Tanto para el modo con fidicas normal como para el diferencial de las
	  componentes X e Y de la velodidad del modo (dukedoom)	
	- Marca la tecla de sprint como presionada y el estado de sprinting del 
	  jugador como verdadero.
	- Informa por la termina (stdout) que el sprint esta activado.
	
*/
void	player_sprint_keypress(t_mlx *mlx)
{
	t_player	*pl;

	pl = mlx->player;
	if (pl->is_flying || pl->is_dragging || pl->is_hided || pl->is_groundpound)
		return ;
	if (mlx->frame->phisics_onoff == OFF)
	{
		mlx->player->speed += 0.05;
		mlx->player->keys.sprint = true;
		mlx->player->is_sprinting = true;
		printf("SPRINT ON\n");
	}
	else
	{
		mlx->player->aceleration[FRONT] += mlx->phisics.floor_friction
			* SPRINT_ACCELERATION_FRONT_FACTOR;
		mlx->player->max_speed_dt = MAX_PLAYER_SPEED_STRINT;
		mlx->player->max_speed_a[X] = MAX_PLAYER_SPEED_STRINT;
		mlx->player->max_speed_a[Y] = MAX_PLAYER_SPEED_STRINT;
		mlx->player->keys.sprint = true;
		mlx->player->is_sprinting = true;
		printf("SPRINT ON\n");
	}
}

/*
	Esta funcion desactiva el sprint del jugador al soltar la tecla Shift.
	- Si el jugador esta volando, arrastrando, escondido o haciendo groundpound
	  no hace nada.
	- Si la fisicas del motor grafico estan desactivada, disminuye la velocidad 
	  del jugador en 0.05
	- Si las fisicas estan activadas, restablece la aceleracion frontal del 
	  jugador y establece la velocidad maxima a MAX_PLAYER_SPEED.
	  Tanto para el modo con fidicas normal como para el diferencial de las
	  componentes X e Y de la velodidad del modo (dukedoom)	
	- Marca la tecla de sprint como no presionada y el estado de sprinting del 
	  jugador como falso.
	- Informa por la termina (stdout) que el sprint esta desactivado.
*/
void	player_sprint_keyrelease(t_mlx *mlx)
{
	t_player	*pl;

	pl = mlx->player;
	if (pl->is_flying || pl->is_dragging || pl->is_hided || pl->is_groundpound)
		return ;
	if (mlx->frame->phisics_onoff == OFF)
	{
		mlx->player->speed -= 0.05;
		mlx->player->keys.sprint = false;
		mlx->player->is_sprinting = false;
		printf("SPRINT OFF\n");
	}
	else
	{
		mlx->player->aceleration[FRONT] = mlx->phisics.floor_friction
			* ACCELERATION_FRONT_FACTOR;
		mlx->player->max_speed_dt = MAX_PLAYER_SPEED;
		mlx->player->max_speed_a[X] = MAX_PLAYER_SPEED;
		mlx->player->max_speed_a[Y] = MAX_PLAYER_SPEED;
		mlx->player->keys.sprint = false;
		mlx->player->is_sprinting = false;
		printf("SPRINT OFF\n");
	}
}

/*
	Esta funcion hace que el jugador salte al presionar la tecla Espacio.
	- Si el jugador esta volando, arrastrando, escondido o haciendo groundpound
	  no hace nada.
	- Si las fisicas del motor grafico estan desactivadas, no hace nada.
	- Si el jugador esta saltando:
		- Establece el estado de salto a verdadero. (sieve para saber si el
		  jugador esta ascendiendo o descendiendo)
		- Establece el estado de estar en el aire a verdadero para saber
		  si el jugador esta en el aire y desactivar las teclas de movimiento
		  WASD y sus aceleracion en el modo fisicas
		- Establece la aceleracion en Z al valor de aceleration_zcp.
		- Establece la velocidad en Z a 0.
	- Informa por la terminal (stdout) que el jugador esta saltando.
	NOTA: Este eestilo de salto nos permite que el jugador salte mas o menos se
	gun el tiempoq que mantenga presionada la tecla Espacio y crear varios
	estilos de fisiscas.
*/
void	player_space_keypress(t_mlx *mlx)
{
	t_player	*pl;

	pl = mlx->player;
	if (pl->is_flying || pl->is_dragging || pl->is_hided || pl->is_groundpound)
		return ;
	if (mlx->frame->phisics_onoff == OFF)
		return ;
	if (mlx->player->is_jumping == false)
	{
		mlx->player->is_jumping = true;
		mlx->player->is_onair = true;
		mlx->player->aceleration_z = mlx->player->aceleration_zcp;
		mlx->player->speed_z = 0.0f;
		printf("JUMPING\n");
	}
}

/*
	Esta funcion hace que el jugador deje de saltar al soltar la tecla Espacio.
	- Si el jugador esta volando, arrastrando, escondido o haciendo groundpound
	  no hace nada.
	- Si las fisicas del motor grafico estan desactivadas, no hace nada.
	- Si el jugador esta saltando, establece la aceleracion en Z a 0 y el 
	  estado de salto a falso.
	- Informa por la terminal (stdout) que el jugador esta cayendo.
*/
void	player_space_keyrelease(t_mlx *mlx)
{
	t_player	*pl;

	pl = mlx->player;
	if (pl->is_flying || pl->is_dragging || pl->is_hided || pl->is_groundpound)
		return ;
	if (mlx->frame->phisics_onoff == OFF)
		return ;
	if (mlx->player->is_jumping == true)
	{
		mlx->player->aceleration_z = 0;
		mlx->player->is_jumping = false;
		printf("FALLING\n");
	}
}

/*
	Esta funcion hace que el jugador se agache al presionar la tecla Ctrl.
	- Si las fisicas del motor grafico estan desactivada, no hace nada.
	- Si el jugador esta volando, arrastrando, escondido o haciendo groundpound
	  no hace nada.
	- Si el jugador no esta agachado:
		- Establece la altura de la camara a FLOOR_Z * 0.5f (mitad de la altura
		  del jugador).
		- Establece la velocidad maxima del jugador a MAX_PLAYER_SPEED * 0.5f.
		  (para que baje la velocidad al agacharse) Tanto en el modo con fisicas
		  normal como en el diferencial de las componentes X e Y de la velodidad
		- Marca el estado de estar agachado como verdadero.
		- Informa por la terminal (stdout) que el jugador se ha agachado.
	- Si el jugador ya esta agachado:
		- Establece la altura de la camara a MIDDLE_Z.
		- Establece la velocidad maxima del jugador a MAX_PLAYER_SPEED.
		- Marca el estado de estar agachado como falso.
		- Informa por la terminal (stdout) que el jugador se ha levantado.
*/
void	player_control_keypress(t_mlx *mlx)
{
	t_player	*pl;

	pl = mlx->player;
	if (mlx->frame->phisics_onoff == OFF)
		return ;
	if (pl->is_onair || pl->is_flying || pl->is_dragging || pl->is_sprinting)
		return ;
	if (mlx->player->is_hided == false)
	{
		mlx->player->camz = FLOOR_Z * 0.5f;
		mlx->player->max_speed_dt = MAX_PLAYER_SPEED * 0.5f;
		mlx->player->max_speed_a[X] = MAX_PLAYER_SPEED * 0.5f;
		mlx->player->max_speed_a[Y] = MAX_PLAYER_SPEED * 0.5f;
		mlx->player->is_hided = true;
		printf("CROUCH\n");
	}
	else
	{
		mlx->player->camz = MIDDLE_Z;
		mlx->player->max_speed_dt = MAX_PLAYER_SPEED;
		mlx->player->max_speed_a[X] = MAX_PLAYER_SPEED;
		mlx->player->max_speed_a[Y] = MAX_PLAYER_SPEED;
		mlx->player->is_hided = false;
		printf("STAND UP\n");
	}
}
