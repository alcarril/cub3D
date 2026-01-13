/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders_fog_blur.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 15:40:14 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/13 18:25:21 by alejandro        ###   ########.fr       */
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


////////////////Minimaping 

int select_mip_level(float dist)
{
	if (dist < 3.0f)  
		return 0;
	if (dist < 6.0f)  
		return 1;
	if (dist < 12.0f) 
		return 2;
	return 3;
}

#define MAX_MIP 3
//logaritimica mas realisata
int select_mip_level_log(float dist)
{
	int mip;
	float scale = 0.5f;

	mip = (int)(log2f(dist * scale));
	if (mip < 0) mip = 0;
	if (mip > MAX_MIP) mip = MAX_MIP;
	return mip;
}


//bordes mas difuminados
int select_mip_level_lat(float dist, float cameraX)
{
	float edge = fabsf(cameraX); // 0 centro, ~1 bordes
	dist *= (1.0f + edge * 0.4f);

	if (dist < 3) 
		return 0;
	if (dist < 6) 
		return 1;
	if (dist < 12) 
		return 2;
	return 3;
}

//continuo 
float get_mip_float(float dist)
{
	float scale = 0.5f;
	float mip = log2f(dist * scale);
	if (mip < 0) mip = 0;
	if (mip > MAX_MIP) mip = MAX_MIP;
	return mip; // valor float entre 0 y MAX_MIP
}



// Funcion base aplicasr shde a  un color
unsigned int apply_shade(unsigned int color, float shade)
{
	int r = (int)(((color >> 16) & 255) * shade);
	int g = (int)(((color >> 8) & 255) * shade);
	int b = (int)((color & 255) * shade);

	if (r > 255) 
		r = 255; 
	if (r < 0) 
		r = 0;
	if (g > 255) 
		g = 255; 
	if (g < 0) 
		g = 0;
	if (b > 255) 
		b = 255; 
	if (b < 0) 
		b = 0;
	return (r << 16) | (g << 8) | b;
}

//Fucnion para plizar el shade lineal
unsigned int shade_linear(unsigned int color, float dist, float max_dist)
{
	float shade = 1.0f - (dist / max_dist);
	if (shade < 0.0f) 
		shade = 0.0f;
	if (shade > 1.0f) 
		shade = 1.0f;

	return apply_shade(color, shade);
}

//metodo inverso de shadin (recomendado) -> k =(0, 1)
unsigned int shade_inverse(unsigned int color, float dist, float k, float max_dist)
{
	float k_reg;
	
	k_reg = k / max_dist; // normalizar k en funcion de la distancia maxima
	float shade = 1.0f / (1.0f + dist * k_reg);
	return apply_shade(color, shade);
}

//medodo exponencial niebla densa density (0, 1)
unsigned int shade_exponential(unsigned int color, float dist, float density, float max_dist)
{
	float density_reg;
	density_reg = density / max_dist; // normalizar density en funcion de
	
	float shade = expf(-dist * density_reg);
	return apply_shade(color, shade);
}