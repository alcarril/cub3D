/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gravity_keys.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 14:21:57 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/22 21:50:17 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	- Selects the EARTH gravity mode and configures the physics and player
	  parameters accordingly:
	- If physics is turned off, a warning message is printed and no changes
	  are made.
	- Prints a confirmation message when EARTH mode is selected.
	- Values set:
	  - Gravity: GRAVITY_EARTH
	  - Floor friction: FLOOR_FRICTION_MS
	  - Air friction: AIR_FRICTION_MS
	  - Player vertical acceleration: gravity * ACCELERATION_K_EARTH
*/
void	selectearth_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
	{
		printf("Warning: Physics is not available.Cannot select EARTH mode.\n");
		return ;
	}
	phisics->gravity = GRAVITY_EARTH;
	phisics->floor_friction = FLOOR_FRICTION_MS;
	phisics->air_friction = AIR_FRICTION_MS;
	pl->aceleration_zcp = phisics->gravity * ACCELERATION_K_EARTH;
	pl->aceleration_z = phisics->gravity * ACCELERATION_K_EARTH;
	printf("Gravity mode: EARTH selected\n");
}

/*
	- Selects the MOON gravity mode and configures the physics and player
	  parameters accordingly:
	- If physics is turned off, a warning message is printed and no changes
	  are made.
	- Prints a confirmation message when MOON mode is selected.
	- Values set:
	  - Gravity: GRAVITY_MOON
	  - Floor friction: FLOOR_FRICTION_MS
	  - Air friction: AIR_FRICTION_MS
	  - Player vertical acceleration: gravity * ACCELERATION_K_MOON
*/
void	selectmoon_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
	{
		printf("Warning: Physics is not available.Cannot select MOON mode.\n");
		return ;
	}
	phisics->gravity = GRAVITY_MOON;
	phisics->floor_friction = FLOOR_FRICTION_MS;
	phisics->air_friction = AIR_FRICTION_MS;
	pl->aceleration_zcp = phisics->gravity * ACCELERATION_K_MOON;
	pl->aceleration_z = phisics->gravity * ACCELERATION_K_MOON;
	printf("Gravity mode: MOON selected\n");
}

/*
	- Selects the HULKPITER gravity mode and configures the physics and player
	  parameters accordingly:
	- If physics is turned off, a warning message is printed and no changes
	  are made.
	- Prints a confirmation message when HULKPITER mode is selected.
	- Values set:
	  - Gravity: GRAVITY_HULKPITER
	  - Floor friction: FLOOR_FRICTION_MS
	  - Air friction: AIR_FRICTION_MS
	  - Player vertical acceleration: gravity * ACCELERATION_K_HULKPITER
*/
void	selecthulker_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
	{
		printf("Warning: Physics is not available. ");
		printf("Cannot select HULKPITER mode.\n");
		return ;
	}
	phisics->gravity = GRAVITY_HULKPITER;
	phisics->floor_friction = FLOOR_FRICTION_MS;
	phisics->air_friction = AIR_FRICTION_MS;
	pl->aceleration_zcp = phisics->gravity * ACCELERATION_K_HULKPITER;
	pl->aceleration_z = phisics->gravity * ACCELERATION_K_HULKPITER;
	printf("Gravity mode: HULKPITER selected\n");
}

/*
	- Selects the ESPECTRO gravity mode and configures the physics and player
	  parameters accordingly:
	- If physics is turned off, a warning message is printed and no changes
	  are made.
	- Prints a confirmation message when ESPECTRO mode is selected.
	- Values set:
	  - Gravity: GRAVITY_SPECTRO
	  - Floor friction: FLOOR_FRICTION_MS
	  - Air friction: AIR_FRICTION_MS
	  - Player vertical acceleration: gravity * ACCELERATION_K_SPECTRO
*/
void	selectspectr_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
	{
		printf("Warning: Physics is not available. ");
		printf("Cannot select ESPECTRO mode.\n");
		return ;
	}
	phisics->gravity = GRAVITY_SPECTRO;
	phisics->floor_friction = FLOOR_FRICTION_MS;
	phisics->air_friction = AIR_FRICTION_MS;
	pl->aceleration_zcp = phisics->gravity * ACCELERATION_K_SPECTRO;
	pl->aceleration_z = phisics->gravity * ACCELERATION_K_SPECTRO;
	printf("Gravity mode: ESPECTRO selected\n");
}

/*
	- Selects the JETPACK gravity mode and configures the physics and player
	  parameters accordingly:
	- If physics is turned off, a warning message is printed and no changes
	  are made.
	- Prints a confirmation message when JETPACK mode is selected.
	- Values set:
	  - Gravity: GRAVITY_JETPACK
	  - Floor friction: FLOOR_FRICTION_MS
	  - Air friction: AIR_FRICTION_MS
	  - Player vertical acceleration: gravity * ACCELERATION_K_JETPACK
*/
void	selectjetpack_mode(t_phisics *phisics, t_player *pl, t_mlx *mlx)
{
	if (mlx->frame->phisics_onoff == OFF)
	{
		printf("Warning: Physics is not available. ");
		printf("Cannot select JETPACK mode.\n");
		return ;
	}
	phisics->gravity = GRAVITY_JETPACK;
	phisics->floor_friction = FLOOR_FRICTION_MS;
	phisics->air_friction = AIR_FRICTION_MS;
	pl->aceleration_zcp = phisics->gravity * ACCELERATION_K_JETPACK;
	pl->aceleration_z = phisics->gravity * ACCELERATION_K_JETPACK;
	printf("Gravity mode: JETPACK selected\n");
}
