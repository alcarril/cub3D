/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 22:59:20 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/11 02:59:42 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

void	toogle_dist_calc(t_mlx *mlx)
{
	if (mlx->frame->draw_walls == draw_wall_column)
	{
		if (mlx->frame->euclidean == false)
		{
			mlx->frame->euclidean = true;
			printf("EUCLIDEAN DISTANCE ENABLED\n");
		}
		else
		{
			mlx->frame->euclidean = false;
			printf("PERPENDICULAR DISTANCE ENABLED\n");
		}
	}
	else
		printf(" WARINIG: Cannot toggle distance calculation method when textures are enabled\n");
}

void	toogle_mouse(t_mlx *mlx)
{
	if (mlx->player->mouse.onoff == ON)
	{
		mlx->player->mouse.onoff = OFF;
		mlx_mouse_show(mlx->mlx_var, mlx->mlx_window);
		printf("MOUSE OFF\n");
	}
	else
	{
		mlx->player->mouse.onoff = ON;
		mlx_mouse_hide(mlx->mlx_var, mlx->mlx_window);
		printf("MOUSE ON\n");
	}
}

void	print_controls(void)
{
	printf("Controls:\n");
	printf("WASD: Move Up, Down, Left, Right\n");
	printf("Left Arrow: Rotate Counterclockwise\n");
	printf("Right Arrow: Rotate Clockwise\n");
	printf("Left Shift: Sprint\n");
	printf("M: Toggle Minimap\n");
	printf("R: Toggle Rays on Minimap\n");
	printf("F: Toggle Fish Eye Effect\n");
	printf("O: Toggle Raycasting On/Off\n");
	printf("E: Toggle Distance Calculation (Euclidean/Perpendicular)\n");
	printf("T: Toggle Textures On/Off\n");
	printf("C: Toggle Floor and Ceiling Rendering Method\n");
	printf("ESC: Exit Game\n");
}