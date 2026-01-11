/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 22:31:15 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/11 02:23:03 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Funcion que maneja la presion de teclas para el movimiento del jugador
	- Se podria optimizar con un jump table
*/
int	key_press(int keysym, t_mlx *mlx)
{
	if (keysym == XK_Escape)
		return (close_game_manager(mlx),0);
	if (player_keypress(mlx, keysym))
		return (0);
	if (keysym == XK_o)
		toogle_raycasting(mlx);
	else if (keysym == XK_m)
		toggle_minimap(mlx);
	else if (keysym == XK_r)
		toggle_rays(mlx);
	else if (keysym == XK_f)
		toggle_fish_eye(mlx);
	else if (keysym == XK_e)
		toogle_dist_calc(mlx);
	else if (keysym == XK_t)
		toggle_textures(mlx);
	else if (keysym == XK_c)
		toogle_floor_celling(mlx);
	else if (keysym == XK_z)
		print_controls();
	else
		printf(CONTROLS_INFO);
	return (0);
}

/*
	Funcion que maneja la liberacion de teclas para el movimiento del jugador
*/
int	key_release(int keysym, t_mlx *mlx)
{
	if (mlx == NULL)
		return (0);
	if (keysym == XK_w)
		mlx->player->keys.move_up = false;
	if (keysym == XK_s)
		mlx->player->keys.move_down = false;
	if (keysym == XK_a)
		mlx->player->keys.move_left = false;
	if (keysym == XK_d)
		mlx->player->keys.move_right = false;
	if (keysym == XK_Left)
		mlx->player->keys.r_counterclockwise = false;
	if (keysym == XK_Right)
		mlx->player->keys.r_clockwise = false;
	if (keysym == XK_Shift_L && mlx->player->keys.sprint == true)
	{
		mlx->player->speed -= 0.05;
		mlx->player->keys.sprint = false;
	}
	return (0);
}

/*
	Funcion que activa o desactiva el raycasting
*/
void	toogle_raycasting(t_mlx *mlx)
{
	if (mlx->frame->raycasting_onoff == true)
	{
		mlx->frame->raycasting_onoff = false;
		printf("RAYCASTING OFF\n");
	}
	else
	{
		mlx->frame->raycasting_onoff = true;
		printf("RAYCASTING ON\n");
	}
}

void	toggle_textures(t_mlx *mlx)
{
	if (mlx->frame->draw_walls == draw_wall_column)
	{
		mlx->frame->draw_walls = draw_wall_column_tex;
		mlx->frame->fish_eye = false;
		mlx->frame->euclidean = false;
		printf("TEXTURED ENABLED\n");
	}
	else
	{
		mlx->frame->draw_walls = draw_wall_column;
		printf("TEXTURES DISABLED\n");
	}
}

void	toogle_floor_celling(t_mlx *mlx)
{
	if (mlx->frame->floor_celling == render_floor_and_ceiling)
	{
		mlx->frame->floor_celling = render_floor_and_ceiling_speed;
		printf("FAST floor and ceiling rendering enabled\n");
	}
	else
	{
		mlx->frame->floor_celling = render_floor_and_ceiling;
		printf("ACURATED floor and ceiling rendering enabled\n");
	}
}
