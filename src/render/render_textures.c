/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:44:58 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/13 19:13:15 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Dibuja una columna de pared texturizada en la pantalla.
	Parámetros:
	- mlx: Puntero a la estructura principal que contiene toda la información del juego.
	- column: Índice de la columna en la pantalla donde se dibujará la pared.
	- wall: Puntero a la estructura de la pared que contiene información sobre la pared a dibujar.
	- ray: Puntero a la estructura del rayo que contiene información sobre el rayo que golpeó la pared.
*/
void draw_wall_column_tex(t_mlx *mlx, int column, t_wall *wall, t_ray *ray)
{
	int i;
	unsigned int color;
	unsigned int fog_color = FOG_MEDIO_OSCURO; // Color del fog (negro)
	float max_distance = mlx->map->max_distance * 0.8f; // Distancia máxima para aplicar el fog
	// int mip;

	wall->texture = select_texture(mlx, ray);
	wall->wall_x = calculate_wall_x(mlx, ray);
	calculate_tex(wall, wall->texture, mlx->win_height, mlx->player->pitch_pix);
	i = wall->wall_start;
	while (i <= wall->wall_end)
	{			
		wall->tex_y = (int)wall->tex_pos;
		color = extract_color(wall->texture, wall->tex_x, wall->tex_y);
		// color = apply_blur(mlx, column, i);
		//aplicar el shade:
			// color = shade_linear(color, ray->wall_dist, max_distance);
			color = shade_inverse(color, ray->wall_dist, 4.0f, max_distance);
			// color = shade_exponential(color, ray->wall_dist, 4.0f, max_distance);
		// Aplicar el fog al color basado en la distancia -> se puede modificar
			color = apply_fog_pixel(color, fog_color, ray->wall_dist, max_distance);
		buffering_pixel(column, i, mlx, color);
		wall->tex_pos += wall->text_v_step;
		i++;
	}
}

/*
	Selecciona la textura adecuada en función de la dirección del rayo
	y del lado de la pared que fue golpeada.
	Parámetros:
	- mlx: Puntero a la estructura principal que contiene toda la información del juego.
	- ray: Puntero a la estructura del rayo que contiene información sobre la dirección del rayo y el lado golpeado.
	Devuelve:
	- Puntero a la textura seleccionada.
*/
t_texture *select_texture(t_mlx *mlx, t_ray *ray)
{
	t_texture *texture;

	if (ray->side_hit == VERTICAL)
	{
		if (ray->raydir[X] > 0)
			texture = &mlx->map->textures[E];
		else
			texture = &mlx->map->textures[W];
	}
	else
	{
		if (ray->raydir[Y] > 0)
			texture = &mlx->map->textures[N];
		else
			texture = &mlx->map->textures[S];
	}
	return texture;
}

/*
	Viene de las ecuaciones parametricas de la linea (x = x0 + t * dx):
	- Donde X0 e Y0 son las coordenadas del jugador
	- DX y DY son las direcciones del rayo
	- t es la distancia a la pared (hipotenusa aprox)
	Ademas hay un ajuste para obtener solo la parte decimal (wall_x - floor(wall_x))
	Se hace un ajuste para que no se inviertan las texturas en ciertas caras
	wall x es un porcentaje (0-1) de donde el rayo golpea la pared en la baseo
	eje x de la textura.
	Nota: se osa ala distancia de la hipotenisa (wall_dist) en ligar de la proyección
	para evitar desplazamientos en las texturas debido al efecto de ojo de pez.
*/
double	calculate_wall_x(t_mlx *mlx, t_ray *ray)
{
	double	wall_x;

	if (ray->side_hit == VERTICAL)
		wall_x = mlx->player->pos_y + ray->wall_dist * ray->raydir[Y];
	else
		wall_x = mlx->player->pos_x + ray->wall_dist * ray->raydir[X];
	wall_x -= floor(wall_x);
	if ((ray->side_hit == VERTICAL && ray->raydir[X] < 0) || 
		(ray->side_hit == HORIZONTAL && ray->raydir[Y] > 0))
	{
		wall_x = 1.0 - wall_x;
	}
	return (wall_x);
}

/*
	Calcula los parámetros necesarios para renderizar la textura:
	- tex_x: Coordenada X en la la matriz de pixeels de la textura
	es decir el pixel horizontal de la textura que se va a usar.
	- text_v_step: text height to screen height ratio, es decir
	  cuánto se debe avanzar en la textura por cada píxel dibujado en la pantalla. para que no se deforme
	- tex_pos: Posición inicial en la textura.

	Parámetros:
	- wall: Estructura que contiene información sobre la pared.
	- texture: Puntero a la textura.
	- win_height: Altura de la ventana.

	No devuelve nada, actualiza directamente los valores en la estructura `wall`.
*/
void	calculate_tex(t_wall *wall, t_texture *texture, int win_height, int pitch)
{
	wall->tex_x = (int)(wall->wall_x * (double)texture->width);
	if (wall->tex_x < 0) 
		wall->tex_x = 0;
	if (wall->tex_x >= texture->width) 
		wall->tex_x = texture->width - 1;
	wall->text_v_step = (float)texture->height / (float)wall->wall_height;
	wall->tex_pos = (wall->wall_start - (win_height / 2 + pitch) + wall->wall_height / 2) * wall->text_v_step;
}

/*
	Extrae el color de un píxel de una textura en base a las coordenadas tex_x y tex_y.
	Pasos:
	1. Validar las coordenadas tex_x y tex_y para asegurarse de que están dentro de los límites de la textura.
	2. Calcular la dirección del píxel en la memoria de la textura utilizando tex_x, tex_y, line_length y bits_per_pixel.
	3. Leer el color desde la dirección calculada en la memoria.
	4. Devolver el color como un entero (unsigned int).
	Parámetros:
	- texture: Puntero a la textura de la que se extraerá el color.
	- tex_x, tex_y: Coordenadas del píxel en la textura.
	Devuelve:
	- El color como un entero (unsigned int).
*/
unsigned int	extract_color(t_texture *texture, int tex_x, int tex_y)
{
	unsigned int	color;
	char			*pixel_address;

	if (tex_y < 0)
		tex_y = 0;
	if (tex_y >= texture->height)
		tex_y = texture->height - 1;
	pixel_address = texture->addr + (tex_y * texture->line_length) + 
		(tex_x * (texture->bits_per_pixel / 8));
	color = *(unsigned int *)pixel_address;
	return (color);
}

