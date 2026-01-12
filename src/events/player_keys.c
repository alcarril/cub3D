/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 13:06:24 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/12 21:26:18 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Funcion que maneja la presion de teclas para el movimiento del jugador
	Teclas manejadas:
		-W A S D : movimiento
		-Left Right : rotacion
		-Up Down : mirar arriba y abajo
	Si entra en alguna de las teclas retorna true
	Si no retorna false
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
	else if (keysym == XK_Up)
		mlx->player->keys.look_up = true;
	else if (keysym == XK_Down)
		mlx->player->keys.look_down = true;
	else if (player_keypres2(mlx, keysym) == true)
		return (true);
	else
		return (false);
	return (true);
}

/*
	Funcion que maneja la presion de teclas adicionales para el jugador
	Si entra en alguna de las teclas retorna true
	Si no retorna false
*/
bool	player_keypres2(t_mlx *mlx, int keysym)
{
	if (keysym == XK_Shift_L && mlx->player->keys.sprint == false)
	{
		mlx->player->speed += 0.05;
		mlx->player->keys.sprint = true;
		printf("SPRINT ON\n");
	}
	else if (keysym == XK_v)
		change_fov(mlx);
	else if (keysym == XK_j)
		toogle_mouse(mlx);
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




