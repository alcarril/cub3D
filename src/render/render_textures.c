/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_textures.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 15:44:58 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/10 16:25:45 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

unsigned int apply_fog_pixel(unsigned int color, unsigned int fog_color, float distance, float max_distance);
unsigned int apply_blur(t_mlx *mlx, int column, int row);
void apply_fog(t_mlx *mlx, unsigned int fog_color, float max_distance);

/*
	Dibuja una columna de pared texturizada en la pantalla.
	Parámetros:
	- mlx: Puntero a la estructura principal que contiene toda la información del juego.
	- column: Índice de la columna en la pantalla donde se dibujará la pared.
	- wall: Puntero a la estructura de la pared que contiene información sobre la pared a dibujar.
	- ray: Puntero a la estructura del rayo que contiene información sobre el rayo que golpeó la pared.
*/
// void	draw_wall_column_tex(t_mlx *mlx, int column, t_wall *wall, t_ray *ray)
// {
// 	int	i;
// 	unsigned int	color;

// 	wall->texture = select_texture(mlx, ray);
// 	wall->wall_x = calculate_wall_x(mlx, ray);
// 	calculate_tex(wall, wall->texture, mlx->win_height);
// 	i = wall->wall_start;
// 	while (i <= wall->wall_end)
// 	{
// 		wall->tex_y = (int)wall->tex_pos;
// 		color = extract_color(wall->texture, wall->tex_x, wall->tex_y);
// 		buffering_pixel(column, i, mlx, color);
// 		wall->tex_pos += wall->text_v_step;
// 		i++;
// 	}
// }

void draw_wall_column_tex(t_mlx *mlx, int column, t_wall *wall, t_ray *ray)
{
    int i;
    unsigned int color;
    // unsigned int fog_color = 0x000000; // Color del fog (negro)
    // float max_distance = 20.0f; // Distancia máxima para aplicar el fog

    wall->texture = select_texture(mlx, ray);
    wall->wall_x = calculate_wall_x(mlx, ray);
    calculate_tex(wall, wall->texture, mlx->win_height);
    i = wall->wall_start;
    while (i <= wall->wall_end)
    {
        wall->tex_y = (int)wall->tex_pos;
        color = extract_color(wall->texture, wall->tex_x, wall->tex_y);

        // Aplicar el fog al color basado en la distancia
        // color = apply_fog_pixel(color, fog_color, ray->wall_dist, max_distance);
		// color = apply_blur(mlx, column, i);
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
	  cuánto se debe avanzar en la textura por cada píxel dibujado en la pantalla.
	- tex_pos: Posición inicial en la textura.

	Parámetros:
	- wall: Estructura que contiene información sobre la pared.
	- texture: Puntero a la textura.
	- win_height: Altura de la ventana.

	No devuelve nada, actualiza directamente los valores en la estructura `wall`.
*/
void	calculate_tex(t_wall *wall, t_texture *texture, int win_height)
{
	wall->tex_x = (int)(wall->wall_x * (double)texture->width);
	if (wall->tex_x < 0) 
		wall->tex_x = 0;
	if (wall->tex_x >= texture->width) 
		wall->tex_x = texture->width - 1;
	wall->text_v_step = (float)texture->height / (float)wall->wall_height;
	wall->tex_pos = (wall->wall_start - win_height / 2 + wall->wall_height / 2) * wall->text_v_step;
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
