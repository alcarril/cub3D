/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic_engine_keys.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 20:54:29 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/12 20:58:40 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

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

/*
	Funcion para togglear texturas
*/
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

/*
	Funcion para togglear renderizado de suelo y techo
	por columnas (speed) o pixel a pixel (acurate)
*/
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

/*
	Funcion para togglear metodo de calculo de distancia
	usando el calculo manual teorema de pitagoras (euclidean)
	o el calculo perpendicular (optimizado de dda)
*/
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

/*
	Funcion para togglear correccion de fish eye
*/
void	toggle_fish_eye(t_mlx *mlx)
{
	if (mlx->frame->draw_walls == draw_wall_column)
	{
		if (mlx->frame->fish_eye == true)
		{
			mlx->frame->fish_eye = false;
			printf("FISH EYE CORRECTION OFF\n");
		}
		else
		{
			mlx->frame->fish_eye = true;
			printf("FISH EYE CORRECTION ON\n");
		}
	}
	else
		printf("WARNING!!: Textures must be off to toggle fish eye effect\n");
}


