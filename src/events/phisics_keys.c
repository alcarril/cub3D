/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phisics_keys.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 06:00:34 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/21 17:50:03 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"


bool	phisics_engine_keypress(t_mlx *mlx, int keysym)
{
	if (select_gravity_modes(mlx, keysym) == true)
		return (true);
	if (keysym == XK_p)
		toggle_phisics_mode(mlx);
	else if (keysym == XK_k)
		toggle_dukedoom_mode(mlx);
	else
		return (false);
	return (true);
}

void	toggle_phisics_mode(t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
	{
		mlx->frame->phisics_onoff = ON;
		printf("Phisics ENABLED: ");
		printf("Now you can experince aceleration, friction and traccion!\n");
	}
	else
	{
		mlx->frame->phisics_onoff = OFF;
		printf("Phisics DISABLED: \n");
	}
}

void	toggle_dukedoom_mode(t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == false)
	{
		printf("WARNING: Phisics engine is off, cannot toggle phisics\n");
		return ;
	}
	if (mlx->frame->dukedoom_mode == OFF)
	{
		mlx->frame->dukedoom_mode = ON;
		printf("DukeDoom mode ENABLED: ");
		printf("Now you can experince Duke Nukem style phisics!\n");
	}
	else
	{
		mlx->frame->dukedoom_mode = OFF;
		printf("DukeDoom mode DISABLED\n");
	}
}

bool	select_gravity_modes(t_mlx *mlx, int keysym)
{
	if (keysym == XK_6)
		selectearth_mode(&mlx->phisics, mlx->player, mlx);
	else if (keysym == XK_7)
		selectmoon_mode(&mlx->phisics, mlx->player, mlx);
	else if (keysym == XK_8)
		selecthulker_mode(&mlx->phisics, mlx->player, mlx);
	else if (keysym == XK_9)
		selectspectr_mode(&mlx->phisics, mlx->player, mlx);
	else if (keysym == XK_0)
		selectjetpack_mode(&mlx->phisics, mlx->player, mlx);
	else
		return (false);
	return (true);
}
