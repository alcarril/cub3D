/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing_textures.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 20:55:10 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/27 03:53:46 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Dibuja una columna de pared texturizada en la pantalla.

	Parámetros:
	- mlx: Puntero a la estructura principal que contiene toda la información 
	  del juego.
	- column: Índice de la columna en la pantalla donde se dibujará la pared.
	- wall: Puntero a la estructura de la pared que contiene información sobre 
	  la pared a dibujar.
	- ray: Puntero a la estructura del rayo que contiene información sobre el 
	  rayo que golpeó la pared.

	Funcionalidad:
	- Selecciona la textura adecuada según la dirección del rayo y el lado de 
	  la pared golpeada.
	- Calcula la posición exacta en la pared donde el rayo impactó para mapear 
	  correctamente la textura.
	- Calcula las coordenadas de la textura y la posición inicial para el 
	  mapeo vertical.
	- Itera sobre cada píxel de la columna de la pared desde el inicio hasta el 
	  final, extrayendo el color correspondiente de la textura y dibujándolo en 
	  el búfer de la imagen.

	Mejoras de microprocesador:
	- Se calcula la textura y la posición de `wall_x` una sola vez fuera del 
	  bucle de cada columna.
	- Se evita acceder a un valor de puntero dentro del bucle crítico utilizando 
	  una variable local `t_texture *texture` en vez de `wall->texture`. Esto 
	  permite un acceso más rápido a registros de memoria y mejora la 
	  vectorización y el uso de SIMD.
	- Se evita acceder a un valor de la estructura `wall` usando una variable 
	  local `wall_end` en vez de `wall->wall_end`. Esto mejora el acceso a 
	  registros de memoria y permite un mejor desenrollado de bucles por el 
	  compilador.
	- Se evita acceder a un valor de la estructura `wall` usando una variable 
	  local `wall_tex_y` en vez de `wall->tex_y`. Esto también mejora el acceso 
	  a registros de memoria y permite un mejor uso de SIMD.
*/
void	draw_wall_column_tex(t_mlx *mlx, int column, t_wall *wall, t_ray *ray)
{
	t_texture		*texture;
	unsigned int	color;
	int				i;
	int				wall_end;
	int				wall_tex_y;

	wall_end = wall->wall_end;
	texture = select_texture(mlx, ray);
	wall->wall_x = calculate_wall_x(mlx, ray);
	calculate_tex(wall, texture, mlx->win_height, mlx->player);
	i = wall->wall_start;
	while (i <= wall_end)
	{
		wall_tex_y = (int)wall->tex_pos;
		color = extract_color(texture, wall->tex_x, wall_tex_y);
		buffering_pixel(column, i, mlx, color);
		wall->tex_pos += wall->text_v_step;
		i++;
	}
}

/*
	Dibuja una columna de pared texturizada en la pantalla utilizando 
	punteros para optimizar el acceso a la memoria.

	Parámetros:
	- mlx: Puntero a la estructura principal que contiene toda la información 
	  del juego.
	- column: Índice de la columna en la pantalla donde se dibujará la pared.
	- wall: Puntero a la estructura de la pared que contiene información sobre 
	  la pared a dibujar.
	- ray: Puntero a la estructura del rayo que contiene información sobre el 
	  rayo que golpeó la pared.

	Funcionalidad:
	- Selecciona la textura adecuada según la dirección del rayo y el lado de 
	  la pared golpeada.
	- Calcula la posición exacta en la pared donde el rayo impactó para mapear 
	  correctamente la textura.
	- Calcula las coordenadas de la textura y la posición inicial para el 
	  mapeo vertical.
	- Utiliza punteros para acceder directamente a los datos de la textura y 
	  del búfer de imagen, mejorando el rendimiento al reducir el overhead de 
	  múltiples accesos a estructuras.
	- Itera sobre cada píxel de la columna de la pared desde el inicio hasta el 
	  final, extrayendo el color correspondiente de la textura y dibujándolo en 
	  el búfer de la imagen.

	Mejoras de microprocesador:
	- **Prefetch automático:** Al usar punteros alineados a memoria contigua, 
	  la CPU puede anticipar lecturas de memoria y cargar bloques en caché ante
	  de que sean usados, reduciendo stalls de memoria (memory-bound).
	- **Variables locales / struct local:** Debido a la limitación de solo 5 
	  variables locales por función, se usan structs locales para agrupar los 
	  valores de bucle. Esto mantiene los datos en registros de CPU y mejora la 
	  eficiencia, aunque lo ideal para prefetch absoluto sería usar variables 
	  individuales locales. 
	- **Evitar cálculos repetitivos:** Se precalculan offsets de textura y 
	  framebuffer fuera del bucle crítico, evitando multiplicaciones y sumas 
	  en cada iteración.
	- **Alineación de punteros:** Los punteros al framebuffer y a la textura se 
	  calculan de forma lineal y se accede a memoria contigua. Esto evita que l
	  CPU haga accesos desalineados que penalizan el throughput de la caché y 
	  mejora el uso de SIMD.
	- **Reducción de overhead de estructuras:** Se evita acceder directamente 
	  a `wall->texture`, `wall->wall_end` o `wall->tex_y` dentro del bucle. 
	  En vez de eso, se copian a variables locales, minimizando el número de 
	  lecturas a memoria y acelerando la iteración.
	- **Optimización de bucle crítico:** Todo el trabajo dentro del bucle se 
	  hace con punteros y operaciones simples (suma de punteros), lo que permit
	  a la CPU pipelinear las instrucciones y reducir latencia.
*/
void	drawwallcoltexspeed(t_mlx *m, int col, t_wall *w, t_ray *ray)
{
	t_locals	l;
	t_texture	*texture;

	texture = select_texture(m, ray);
	w->wall_x = calculate_wall_x(m, ray);
	calculate_tex(w, texture, m->win_height, m->player);
	l.win_width = m->win_width;
	l.wall_end = w->wall_end;
	l.i = w->wall_start - 1;
	l.tex_pos = w->tex_pos;
	l.text_v_step = w->text_v_step;
	l.tex_stride = texture->line_length >> 2;
	l.tex_ptr = (t_u *)texture->addr + w->tex_x;
	l.fb_ptr = (t_u *)m->bit_map_address + col + w->wall_start * m->win_width;
	while (++l.i <= l.wall_end)
	{
		l.tex_y = (int)l.tex_pos;
		if (l.tex_y < 0)
			l.tex_y = 0;
		else if (l.tex_y >= texture->height)
			l.tex_y = texture->height - 1;
		*l.fb_ptr = l.tex_ptr[l.tex_y * l.tex_stride];
		l.tex_pos += l.text_v_step;
		l.fb_ptr += l.win_width;
	}
}

/*
	Dibuja una columna de pared texturizada en la pantalla con efectos de 
	ambiente, como sombreado, desaturación y niebla.

	Parámetros:
	- mlx: Puntero a la estructura principal que contiene toda la información 
	  del juego.
	- column: Índice de la columna en la pantalla donde se dibujará la pared.
	- wall: Puntero a la estructura de la pared que contiene información sobre 
	  la pared a dibujar.
	- ray: Puntero a la estructura del rayo que contiene información sobre el 
	  rayo que golpeó la pared.

	Funcionalidad:
	- Selecciona la textura adecuada según la dirección del rayo y el lado de 
	  la pared golpeada.
	- Calcula la posición exacta en la pared donde el rayo impactó para mapear 
	  correctamente la textura.
	- Calcula las coordenadas de la textura y la posición inicial para el 
	  mapeo vertical.
	- Aplica efectos de ambiente como sombreado, desaturación y niebla a cada 
	  píxel de la textura antes de dibujarlo en el búfer de la imagen.

	Optimizaciones de microprocesador:
	- Se evita acceder a valores de la estructura `wall` dentro del bucle 
	  crítico utilizando variables locales (`wall_end`, `wall_tex_y`) para 
	  mejorar el acceso a registros de memoria y permitir un mejor desenrollado 
	  de bucles por el compilador, mejorando la vectorización y el uso de SIMD.
	- Se calcula la proporción de distancia (`proporcion_dist`) una sola vez 
	  fuera del bucle de cada columna, utilizando la multiplicación por el 
	  inverso de la distancia máxima (`inv_max_distance`) en lugar de realizar 
	  divisiones dentro del bucle crítico.
	- La elección de aplicar efectos de ambiente o no se realiza antes de 
	  llamar a la función, evitando saltos condicionales dentro del bucle 
	  crítico, lo que mejora el rendimiento al eliminar ramas condicionales.
	- El color de la niebla (`fog_color`) se almacena en una variable local 
	  para evitar accesos repetidos a la estructura `mlx` en cada iteración del 
	  bucle, permitiendo el uso directo de registros de CPU o caché.

	Esta función mejora la calidad visual al añadir efectos de ambiente, como 
	sombreado, desaturación y niebla, mientras optimiza el rendimiento en 
	bucles críticos.
*/
void	drawinglopp_tex_amb(t_mlx *mlx, int column, t_wall *wall, t_ray *ray)
{
	unsigned int	color;
	float			proporcion_dist;
	int				i;
	int				wall_end;
	int				wall_tex_y;

	proporcion_dist = ray->wall_dist * mlx->amb.vinv_max_diatance;
	wall->texture = select_texture(mlx, ray);
	wall->wall_x = calculate_wall_x(mlx, ray);
	calculate_tex(wall, wall->texture, mlx->win_height, mlx->player);
	wall_end = wall->wall_end;
	i = wall->wall_start - 1;
	while (++i <= wall_end)
	{
		wall_tex_y = (int)wall->tex_pos;
		color = extract_color(wall->texture, wall->tex_x, wall_tex_y);
		color = shade_inverse(color, mlx->amb.k_factor_walls, proporcion_dist
				* mlx->amb.mult_shader_walls);
		color = apply_desaturation(color, proporcion_dist * 0.6f);
		color = apply_fog_pixel(color, mlx->amb.fog_color_walls,
				proporcion_dist * mlx->amb.mult_fog_walls);
		buffering_pixel(column, i, mlx, color);
		wall->tex_pos += wall->text_v_step;
	}
}
