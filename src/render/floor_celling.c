/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_celling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:45:21 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/13 19:55:15 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función para renderizar el suelo y el techo píxel a píxel
	- Calcula la línea del horizonte (origen o centro) basada en la altura de la ventana
	  y el la desviación vertical del jugador (pitch_pix) -> trasñacion vertical de los
	  puntos del conjunto
	- Itera sobre cada píxel desde la parte superior hasta la línea del horizonte
	  y pinta el techo con el color definido en el mapa
	- Luego, itera desde la línea del horizonte hasta la parte inferior de la ventana
	  y pinta el suelo con el color definido en el mapa
	- En resumen sivide la ventana en dos partes: techo y suelo, y las pinta
	  con los colores especificados
	- Se usa en el renderizado, despues se sobreescrobe la imagen con las paredes y
	  otros elementos 3D y el minimapa 2D
	- Mas lento que render floor and ceiling speed
	- Mas modular que renderizar en el el pintado de del raycasting (aunque
	  menos eficiente)
*/
void	render_floor_and_ceiling(t_mlx *mlx)
{
	int	x;
	int	y;
	int horizon;

	horizon = mlx->win_height / 2 + mlx->player->pitch_pix;
	y = -1;
	while (++y < horizon)
	{
		x = -1;
		while (++x < mlx->win_width)
			buffering_pixel(x, y, mlx, mlx->map->ceiling_color_hex);
	}
	y = horizon;
	while (y < mlx->win_height)
	{
		x = -1;
		while (++x < mlx->win_width)
			buffering_pixel(x, y, mlx, mlx->map->floor_color_hex);
		y++;
	}
}

/*
	Hace lo mismo que render_floor_and_ceiling pero de forma más rápida
	- En lugar de pintar píxel a píxel, pinta línea por línea usando
	  la función buffering_line
	- Con esto se reduce la cantidad de llamadas a la función de biferiado
	  y se mejora el rendimiento.
	- Solo se puede usar cuando el renderizado del suelo y el techo no estan texturizados
*/
void render_floor_and_ceiling_speed1(t_mlx *mlx)
{
	int y;
	int horizon;

	horizon = mlx->win_height / 2 + mlx->player->pitch_pix;
	y = 0;
	while (y < horizon)
	{
		buffering_line(y, mlx->map->ceiling_color_hex, mlx, mlx->win_width);
		y++;
	}
	while (y < mlx->win_height)
	{
		buffering_line(y, mlx->map->floor_color_hex, mlx, mlx->win_width);
		y++;
	}
}

//version puesta de sol el techo se aclara hacai el horizonte detras del muro
//es sulo si que esta mas claro ne us pies pero mas ocuro al horizonte
//da sensacion de sombra
void render_floor_and_ceiling_speed4(t_mlx *mlx)
{
	int y;
	int horizon;
	float dist_factor;
	unsigned int color;
	unsigned int fog_color = FOG_MEDIO_OSCURO; // niebla más oscura


	horizon = mlx->win_height / 2 + mlx->player->pitch_pix;
	// -------- Techo --------
	y = 0;
	while (y < horizon)
	{
		dist_factor = (float)(horizon - y) / horizon; // 0 (cerca) a 1 (horizonte)
		
		// Inverse shading ligero para techo
		float k = 1.0f / mlx->map->max_distance;
		color = apply_shade(mlx->map->ceiling_color_hex, 1.0f / (1.0f + dist_factor * mlx->map->max_distance * k));
		
		// Fog más oscura
		float fog_distance = dist_factor * mlx->map->max_distance * 0.7f; // techo menos fog
		 color = apply_fog_pixel(color, fog_color, fog_distance, mlx->map->max_distance);

		buffering_line(y, color, mlx, mlx->win_width);
		y++;
	}

	// -------- Suelo --------
	y = horizon;
	while (y < mlx->win_height)
	{
		dist_factor = (float)(y - horizon) / (mlx->win_height - horizon); // 0 (cerca) a 1 (horizonte)
		
		// Inverse shading fuerte para suelo
		float k = 4.0f / mlx->map->max_distance;
		color = apply_shade(mlx->map->floor_color_hex, 1.0f / (1.0f + dist_factor * mlx->map->max_distance * k));

		// Aplicar fog usando tu función
		float fog_distance = dist_factor * mlx->map->max_distance; // suelo más fog
		color = apply_fog_pixel(color, fog_color, fog_distance, mlx->map->max_distance);

		buffering_line(y, color, mlx, mlx->win_width);
		y++;
	}
}

//version no se como es 
void render_floor_and_ceiling_speed5(t_mlx *mlx)
{
	int y;
	int horizon;
	float dist_factor;
	unsigned int color;
	unsigned int fog_color = FOG_MEDIO_OSCURO; // niebla más oscura


	horizon = mlx->win_height / 2 + mlx->player->pitch_pix;
	// -------- Techo --------
	y = 0;
	while (y < horizon)
	{
		dist_factor = (float)(horizon - y) / horizon; // 0 (cerca) a 1 (horizonte)
		
		// Inverse shading ligero para techo
		float k = 1.0f / mlx->map->max_distance;
		color = apply_shade(mlx->map->ceiling_color_hex, 1.0f / (1.0f + dist_factor * mlx->map->max_distance * k));
		
		// Fog más oscura
		float fog_distance = dist_factor * mlx->map->max_distance * 0.7f; // techo menos fog
		 color = apply_fog_pixel(color, fog_color, fog_distance, mlx->map->max_distance);

		buffering_line(y, color, mlx, mlx->win_width);
		y++;
	}

	// -------- Suelo --------
	y = horizon;
	while (y < mlx->win_height)
	{
		// dist_factor = (float)(y - horizon) / (mlx->win_height - horizon); // 0 (cerca) a 1 (horizonte)
		dist_factor = 1.0f - (float)(horizon - y) / horizon; // 0 (horizonte) a 1 (cerca)
		// Inverse shading fuerte para suelo
		float k = 4.0f / mlx->map->max_distance;
		color = apply_shade(mlx->map->floor_color_hex, 1.0f / (1.0f + dist_factor * mlx->map->max_distance * k));

		// Aplicar fog usando tu función
		float fog_distance = dist_factor * mlx->map->max_distance; // suelo más fog
		color = apply_fog_pixel(color, fog_color, fog_distance, mlx->map->max_distance);

		buffering_line(y, color, mlx, mlx->win_width);
		y++;
	}
}

//cersion profundidad
void render_floor_and_ceiling_speed(t_mlx *mlx)
{
	int y;
	int horizon;
	float dist_factor;
	unsigned int color;
	unsigned int fog_color = FOG_MEDIO_OSCURO; // niebla más oscura


	horizon = mlx->win_height / 2 + mlx->player->pitch_pix;
	// -------- Techo --------
	y = 0;
	while (y < horizon)
	{
		// dist_factor = (float)(horizon - y) / horizon; // 0 (cerca) a 1 (horizonte)
		dist_factor = 1.0f - (float)(horizon - y) / horizon; 
		// Inverse shading ligero para techo
		float k = 1.0f / mlx->map->max_distance;
		color = apply_shade(mlx->map->ceiling_color_hex, 1.0f / (1.0f + dist_factor * mlx->map->max_distance * k));
		
		// Fog más oscura
		float fog_distance = dist_factor * mlx->map->max_distance * 0.7f; // techo menos fog
		 color = apply_fog_pixel(color, fog_color, fog_distance, mlx->map->max_distance);

		buffering_line(y, color, mlx, mlx->win_width);
		y++;
	}

	// -------- Suelo --------
	y = horizon;
	while (y < mlx->win_height)
	{
		// dist_factor = (float)(y - horizon) / (mlx->win_height - horizon); // 0 (cerca) a 1 (horizonte)
		dist_factor = 1.0f - (float)(horizon - y) / horizon; // 0 (horizonte) a 1 (cerca)
		// Inverse shading fuerte para suelo
		float k = 4.0f / mlx->map->max_distance;
		color = apply_shade(mlx->map->floor_color_hex, 1.0f / (1.0f + dist_factor * mlx->map->max_distance * k));

		// Aplicar fog usando tu función
		float fog_distance = dist_factor * mlx->map->max_distance; // suelo más fog
		color = apply_fog_pixel(color, fog_color, fog_distance, mlx->map->max_distance);

		buffering_line(y, color, mlx, mlx->win_width);
		y++;
	}
}
