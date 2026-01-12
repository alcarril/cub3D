/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_celling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:45:21 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/12 21:58:51 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

void	render_floor_and_ceiling_speed2(t_mlx *mlx);

void	render_floor_and_ceiling(t_mlx *mlx)
{
	int	x;
	int	y;
	int horizon;

	horizon = mlx->win_height / 2 + mlx->player->pitch_pix;
	y = 0;
	while (y < horizon)
	{
		x = 0;
		while (x < mlx->win_width)
		{
			buffering_pixel(x, y, mlx, mlx->map->ceiling_color_hex);
			x++;
		}
		y++;
	}
	y = horizon;
	while (y < mlx->win_height)
	{
		x = 0;
		while (x < mlx->win_width)
		{
			buffering_pixel(x, y, mlx, mlx->map->floor_color_hex);
			x++;
		}
		y++;
	}
}


void render_floor_and_ceiling_speed(t_mlx *mlx)
{
	int y;
	int horizon;

	horizon = mlx->win_height / 2 + mlx->player->pitch_pix;
	y = 0;
	while (y < horizon)
	{
		buffering_line(y, mlx->map->ceiling_color_hex, mlx);
		y++;
	}
	while (y < mlx->win_height)
	{
		buffering_line(y, mlx->map->floor_color_hex, mlx);
		y++;
	}
}

