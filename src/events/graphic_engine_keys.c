/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphic_engine_keys.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/12 20:54:29 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/22 21:27:27 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Function to toggle raycasting on or off
	- USamos estas funciones para hacer que el raycasting se desactive y
	  activar el minimapa grande para hacer un efecto de mapa completo.
	- Si el raycasting esta off, el minimapa se agranda al tamaño de la ventana
	  y se activa.
	- Si el raycasting esta on, el minimapa vuelve a su tamaño original
	  y se desactiva.
	- Se recalculan las escalas del minimapa en ambos casos para poder hacer
	  zoom in y zoom out correctamente y el minimapa dinamico funcione bien.
*/
void	toogle_raycasting(t_mlx *mlx)
{
	t_frame	*f;

	f = mlx->frame;
	if (mlx->frame->raycasting_onoff == ON)
	{
		mlx->frame->raycasting_onoff = OFF;
		f->mm_height *= MINI_HEIGHT;
		f->mm_widht *= MINI_WIDTH;
		f->minimap_onoff = ON;
		printf("RAYCASTING OFF\n");
	}
	else
	{
		mlx->frame->raycasting_onoff = ON;
		f->mm_height /= MINI_HEIGHT;
		f->mm_widht /= MINI_WIDTH;
		printf("RAYCASTING ON\n");
	}
	f->mm_scale[X] = (float)(f->mm_widht) / mlx->map->max_columns;
	f->mm_scale[Y] = (float)(f->mm_height) / mlx->map->max_rows;
}

/*
	Funcion para togglear texturas
	- Si las texturas estan off, se activan y se desactivan
	  fish eye y euclidean distance.
	- Si las texturas estan on, se desactivan y se desactiva
	  el ambiente
*/
void	toggle_textures(t_mlx *mlx)
{
	if (mlx->frame->textures_onoff == OFF)
	{
		mlx->frame->textures_onoff = ON;
		mlx->frame->fish_eye = OFF;
		mlx->frame->euclidean = OFF;
		printf("TEXTURED ENABLED\n");
	}
	else
	{
		mlx->frame->textures_onoff = OFF;
		mlx->frame->ambiance_onoff = OFF;
		printf("TEXTURES DISABLED\n");
	}
}

/*
	Funcion para togglear metodo de renderizado
	- Si el boost esta off, se activa el renderizado rapido
	  de piso y techo.
	- Si el boost esta on, se activa el renderizado acurado
	  de piso y techo.
	- Si la ambiancia esta on, no se puede togglear el metodo
	  de renderizado de piso y techo.
	- Si el mouse no ha estado en la ventana, no se puede
	  togglear el metodo de renderizado de piso y techo por
	  que el kernel no ha creado el pixmap para el mouse y crashea
	  nos hemos pasado con las optimizaciones.
*/
void	toogle_floor_celling(t_mlx *mlx)
{
	if (mlx->has_been_mouse_in_window == false)
	{
		printf("WARNING: Boost mode requires kernell create pixmap");
		printf("for mouse\n");
		printf("TRADUCTION: Put the f... mouse in window at least once !!!\n");
		return ;
	}
	if (mlx->frame->ambiance_onoff == ON)
	{
		printf("WARNING: Ambiance is on, cannot toggle floor and ceiling");
		printf("rendering method\n");
		return ;
	}
	if (mlx->frame->boost == OFF)
	{
		mlx->frame->boost = ON;
		printf("FAST floor and ceiling rendering enabled\n");
	}
	else
	{
		mlx->frame->boost = OFF;
		printf("ACURATED floor and ceiling rendering enabled\n");
	}
}

/*
	Funcion para togglear metodo de calculo de distancia
	usando el calculo manual teorema de pitagoras (euclidean)
	o el calculo perpendicular (optimizado de dda)
	- Si las texturas estan off, se puede togglear
	  el metodo de calculo de distancia.
	- Si las texturas estan on, no se puede togglear
	  el metodo de calculo de distancia.
	- Se informa al usuario del cambio de metodo por terminal.
*/
void	toogle_dist_calc(t_mlx *mlx)
{
	if (mlx->frame->textures_onoff == OFF)
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
		printf("WARNING!!: Textures must be off to toggle euclidean dist\n");
}

/*
	Funcion para togglear correccion de fish eye
	- Si las texturas estan off, se puede togglear
	  el efecto fish eye.
	- Si las texturas estan on, no se puede togglear
	  el efecto fish eye.
	- Se informa al usuario del cambio de metodo por terminal.
*/
void	toggle_fish_eye(t_mlx *mlx)
{
	if (mlx->frame->textures_onoff == OFF)
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
