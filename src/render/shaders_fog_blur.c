/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders_fog_blur.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 15:40:14 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/12 21:55:52 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Aplica un efecto de fog al color basado en la distancia.
	- color: Color original del píxel.
	- fog_color: Color del fog (por ejemplo, negro o gris).
	- distance: Distancia del rayo a la pared.
	- max_distance: Distancia máxima para aplicar el fog.
*/
unsigned int apply_fog_pixel(unsigned int color, unsigned int fog_color, float distance, float max_distance)
{
	float fog_factor;
	unsigned char r, g, b;
	unsigned char fog_r, fog_g, fog_b;

	// Calcular el factor de fog (entre 0 y 1)
	fog_factor = distance / max_distance;
	if (fog_factor > 1.0f)
		fog_factor = 1.0f;

	// Extraer los componentes RGB del color original
	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;

	// Extraer los componentes RGB del fog_color
	fog_r = (fog_color >> 16) & 0xFF;
	fog_g = (fog_color >> 8) & 0xFF;
	fog_b = fog_color & 0xFF;

	// Mezclar el color original con el fog_color
	r = (unsigned char)((1.0f - fog_factor) * r + fog_factor * fog_r);
	g = (unsigned char)((1.0f - fog_factor) * g + fog_factor * fog_g);
	b = (unsigned char)((1.0f - fog_factor) * b + fog_factor * fog_b);

	// Reconstruir el color final
	return (r << 16) | (g << 8) | b;
}

/*
	Aplica un efecto de blur al color basado en los píxeles vecinos.
	- mlx: Puntero a la estructura principal.
	- column, row: Coordenadas del píxel actual.
	Devuelve el color desenfocado.
*/
unsigned int apply_blur(t_mlx *mlx, int column, int row)
{
	unsigned int color = 0;
	unsigned int neighbor_color;
	int count = 0;

	// Mezclar el color del píxel actual con los vecinos
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			int x = column + dx;
			int y = row + dy;

			// Asegurarse de que las coordenadas están dentro de los límites
			if (x >= 0 && x < mlx->win_width && y >= 0 && y < mlx->win_height)
			{
				neighbor_color = *(unsigned int *)(mlx->bit_map_address + (y * mlx->line_length) + (x * (mlx->bits_per_pixel / 8)));
				color += neighbor_color;
				count++;
			}
		}
	}

	// Promediar los colores
	color /= count;
	return color;
}

/*
	Aplica el efecto de fog a toda la pantalla.
	- mlx: Puntero a la estructura principal que contiene los datos de la ventana y raycasting.
	- fog_color: Color del fog (por ejemplo, negro o gris).
	- max_distance: Distancia máxima para aplicar el fog.
*/
void apply_fog(t_mlx *mlx, unsigned int fog_color, float max_distance)
{
	int column;
	int row;
	unsigned int color;
	float distance;

	// Recorrer cada columna y fila de la pantalla
	for (column = 0; column < mlx->win_width; column++)
	{
		for (row = 0; row < mlx->win_height; row++)
		{
			// Calcular la distancia del rayo correspondiente
			distance = mlx->frame->fov_distances[column];

			// Obtener el color actual del píxel
			color = *(unsigned int *)(mlx->bit_map_address + (row * mlx->line_length) + (column * (mlx->bits_per_pixel / 8)));

			// Aplicar el fog al color
			color = apply_fog_pixel(color, fog_color, distance, max_distance);

			// Escribir el color modificado de vuelta en el buffer
			*(unsigned int *)(mlx->bit_map_address + (row * mlx->line_length) + (column * (mlx->bits_per_pixel / 8))) = color;
		}
	}
}