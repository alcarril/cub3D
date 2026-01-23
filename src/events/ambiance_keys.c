/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiance_keys.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:20:17 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/22 21:19:34 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	- Toggles the ambiance effect on or off and prints the current state on
	the console (stdout).
	- If textures are off, ambiance cannot be toggled.
*/
void	toogle_ambiance(t_mlx *mlx)
{
	if (mlx->frame->textures_onoff == OFF)
	{
		printf("WARNING: Textures are off, cannot toggle ambiance\n");
		return ;
	}
	if (mlx->frame->ambiance_onoff == ON)
	{
		mlx->frame->ambiance_onoff = OFF;
		printf("AMBIANCE OFF\n");
	}
	else
	{
		mlx->frame->ambiance_onoff = ON;
		printf("AMBIANCE ON\n");
	}
}

/*
	- Handles keypress events for selecting different ambiance settings.
	- Maps specific key symbols to ambiance selection functions.
	- Returns true if an ambiance was selected, false otherwise.
*/
bool	ambiance_keypress(t_mlx *mlx, int keysym)
{
	if (keysym == XK_1)
		select_ambiance(mlx, ASTURIAS);
	else if (keysym == XK_2)
		select_ambiance(mlx, CEMENTERY);
	else if (keysym == XK_3)
		select_ambiance(mlx, OPEN);
	else if (keysym == XK_4)
		select_ambiance(mlx, MATRIX);
	else
		return (false);
	return (true);
}

/*
	- Selects and configures the ambiance based on the provided ambiance type.
	- If ambiance is off, it prints a warning and does not change the ambiance.
	- Supports different ambiance types: ASTURIAS, CEMENTERY, OPEN, MATRIX.
*/
void	select_ambiance(t_mlx *mlx, int ambiance)
{
	if (mlx->frame->ambiance_onoff == false)
	{
		printf("WARNING: Ambiance is off, cannot change ambiance\n");
		return ;
	}
	if (ambiance == ASTURIAS)
	{
		config_ambiance_asturias(mlx->map, &(mlx->amb));
		printf("ASTURIAS AMBIANCE SELECTED: ");
		printf("Recomendeded for open fields\n");
	}
	else if (ambiance == CEMENTERY)
	{
		config_ambiance_cementery(&(mlx->amb));
		printf("CEMETERY AMBIANCE SELECTED: ");
		printf("Recomendeded for closed spaces or Medieval map\n");
	}
	else
		select_ambiance1(mlx, ambiance);
}

/*
	- Helper function to select and configure additional ambiance types.
	- Supports OPEN and MATRIX ambiance types.
*/
void	select_ambiance1(t_mlx *mlx, int ambiance)
{
	if (ambiance == OPEN)
	{
		config_ambiance_open(mlx->map, &(mlx->amb));
		printf("OPEN AMBIANCE SELECTED: ");
		printf("Recomendeded for open spaces with sun\n");
	}
	else if (ambiance == MATRIX)
	{
		config_ambiance_matrix(mlx->map, &(mlx->amb));
		printf("MATRIX AMBIANCE SELECTED: ");
		printf("Recomendeded for digital environments\n");
	}
}
