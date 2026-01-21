/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gravity_keys.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 14:21:57 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/20 22:47:01 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

void	selectearth_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
	{
		printf("Warning: Physics is not available. Cannot select EARTH mode.\n");
		return;
	}
	phisics->gravity = GRAVITY_EARTH;
	phisics->floor_friction = FLOOR_FRICTION_MS;
	phisics->air_friction = AIR_FRICTION_MS;
	pl->aceleration_zcp = phisics->gravity * ACCELERATION_K_EARTH;
	pl->aceleration_z = phisics->gravity * ACCELERATION_K_EARTH;
	printf("Gravity mode: EARTH selected\n");
}

void	selectmoon_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
	{
		printf("Warning: Physics is not available. Cannot select MOON mode.\n");
		return;
	}
	phisics->gravity = GRAVITY_MOON;
	phisics->floor_friction = FLOOR_FRICTION_MS;
	phisics->air_friction = AIR_FRICTION_MS;
	pl->aceleration_zcp = phisics->gravity * ACCELERATION_K_MOON;
	pl->aceleration_z = phisics->gravity * ACCELERATION_K_MOON;
	printf("Gravity mode: MOON selected\n");
}

void	selecthulker_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
	{
		printf("Warning: Physics is not available. Cannot select HULKPITER mode.\n");
		return;
	}
	phisics->gravity = GRAVITY_HULKPITER;
	phisics->floor_friction = FLOOR_FRICTION_MS;
	phisics->air_friction = AIR_FRICTION_MS;
	pl->aceleration_zcp = phisics->gravity * ACCELERATION_K_HULKPITER;
	pl->aceleration_z = phisics->gravity * ACCELERATION_K_HULKPITER;
	printf("Gravity mode: HULKPITER selected\n");
}

void	selectspectr_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
	{
		printf("Warning: Physics is not available. Cannot select ESPECTRO mode.\n");
		return;
	}
	phisics->gravity = GRAVITY_SPECTRO;
	phisics->floor_friction = FLOOR_FRICTION_MS;
	phisics->air_friction = AIR_FRICTION_MS;
	pl->aceleration_zcp = phisics->gravity * ACCELERATION_K_SPECTRO;
	pl->aceleration_z = phisics->gravity * ACCELERATION_K_SPECTRO;
	printf("Gravity mode: ESPECTRO selected\n");
}

void	selectjetpack_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
	{
		printf("Warning: Physics is not available. Cannot select JETPACK mode.\n");
		return;
	}
	phisics->gravity = GRAVITY_JETPACK;
	phisics->floor_friction = FLOOR_FRICTION_MS;
	phisics->air_friction = AIR_FRICTION_MS;
	pl->aceleration_zcp = phisics->gravity * ACCELERATION_K_JETPACK;
	pl->aceleration_z = phisics->gravity * ACCELERATION_K_JETPACK;
	printf("Gravity mode: JETPACK selected\n");
}

