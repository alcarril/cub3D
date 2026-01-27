/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_keys.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/08 13:06:24 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/22 22:26:21 by alejandro        ###   ########.fr       */
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
	- Si entra en alguna de las teclas retorna true
	- Si no retorna false
	- Teclas manejadas:
		- Shift Izquierdo: sprint
		- Control Izquierdo: agacharse
		- Caps Lock: bloquear mayusculas
		- Q: accion Q
		- Barra espaciadora: salto
		- Coma: aumentar volumen
		- Punto: disminuir volumen
		- V: cambiar fov
		- J: togglear mouse
*/
bool	player_keypres2(t_mlx *mlx, int keysym)
{
	if (keysym == XK_Shift_L)
		return (player_sprint_keypress(mlx), true);
	if (keysym == XK_Control_L)
		return (player_control_keypress(mlx), true);
	if (keysym == XK_Caps_Lock)
		return (player_blockmayus_keypress(mlx), true);
	if (keysym == XK_q)
		return (player_q_keypress(mlx), true);
	if (keysym == XK_space)
		player_space_keypress(mlx);
	else if (keysym == XK_comma)
		change_player_volume(mlx, true);
	else if (keysym == XK_period)
		change_player_volume(mlx, false);
	else if (keysym == XK_v)
		change_fov(mlx);
	else if (keysym == XK_j)
		toogle_mouse(mlx);
	else
		return (false);
	return (true);
}

/*
	Funcion para cambiar el fov (angulo de vision) del jugador
	- Si el fov actual es 60, lo cambia a 90
	- Si el fov actual es 90, lo cambia a 60
	- Ademas, actualiza las variables relacionadas con el fov:
		rad_fov, fov_half, delta_rays, para no tener que actualizarlas
		en el hoot loop cada frame. (mejora de eficiencia)
	- Informa al usuario del cambio por terminal (stdout)
*/
void	change_fov(t_mlx *mlx)
{
	if (mlx->player->fov == 60.0f)
	{
		mlx->player->fov = 90.0f;
		mlx->player->rad_fov = 90.0f * (PI / 180.0f);
		mlx->player->fov_half = mlx->player->rad_fov / 2.0f;
		mlx->frame->delta_rays = (mlx->player->rad_fov) / mlx->win_width;
		printf("FOV 90\n");
	}
	else
	{
		mlx->player->fov = 60.0f;
		mlx->player->rad_fov = 60.0f * (PI / 180.0f);
		mlx->player->fov_half = mlx->player->rad_fov / 2.0f;
		mlx->frame->delta_rays = (mlx->player->rad_fov) / mlx->win_width;
		printf("FOV 60\n");
	}
}

/*
	Funcion para aumentar el volumen del jugador
	- Si flag es true, aumenta el volumen en 0.1
	- Si flag es false, disminuye el volumen en 0.1
	- El volumen tiene un limite maximo de 1.0 y un limite minimo de 0.2
	- Informa al usuario del cambio por terminal (stdout)
	NOTA: Cuando el volumen es muy grande el personaje no puede
	pasar por pasillos estrechos, por lo que se limita el volumen.
*/
void	change_player_volume(t_mlx *mlx, bool flag)
{
	if (flag == true && mlx->player->volume < 1.0f)
	{
		mlx->player->volume += 0.1f;
		if (mlx->player->volume > 1.1f)
			mlx->player->volume = 1.0f;
		printf("Volume: %.1f\n", mlx->player->volume);
	}
	else if (flag == false && mlx->player->volume > 0.0f)
	{
		mlx->player->volume -= 0.1f;
		if (mlx->player->volume < 0.2f)
			mlx->player->volume = 0.2f;
		printf("Volume: %.1f\n", mlx->player->volume);
	}
}
