/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_celling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:45:21 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/24 17:18:41 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función para renderizar el suelo y el techo píxel a píxel.

	Detalles:
	- Calcula la línea del horizonte (origen o centro) basada en la altura de 
	  la ventana y la desviación vertical del jugador (`pitch_pix`), que 
	  representa la traslación vertical de los puntos del conjunto.
	- Itera sobre cada píxel desde la parte superior hasta la línea del 
	  horizonte y pinta el techo con el color definido en el mapa.
	- La linea del horizonte se calcula como la mitad de la altura de la ventan
	  más el pitch del jugador en píxeles, es una tralacion del origen vertical
	  de la camara para simular mirar hacia arriba o hacia abajo en vez de hace
	  una traslacion de los puntos del conjunti.
	- Luego, itera desde la línea del horizonte hasta la parte inferior de la 
	  ventana y pinta el suelo con el color definido en el mapa.
	- Divide la ventana en dos partes: techo y suelo, y las pinta con los 
	  colores especificados.

	Detalles adicionales:
	- Se utiliza en el renderizado antes de sobrescribir la imagen con las 
	  paredes, otros elementos 3D y el minimapa 2D.
	- Es más lento que `render_floor_and_ceiling_speed`, pero es más modular.
	- Es menos eficiente que renderizar el suelo y el techo directamente en el 
	  proceso de raycasting, pero mejora la modularidad del código.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Esta función es útil para dividir el renderizado en pasos más claros y 
	modulares, aunque con un mayor coste computacional.
*/
void	render_floor_and_ceiling(t_mlx *mlx)
{
	int	x;
	int	y;
	int	horizon;

	horizon = (mlx->win_height >> 1) + mlx->player->pitch_pix;
	y = -1;
	while (++y < horizon && y < mlx->win_height)
	{
		x = -1;
		while (++x < mlx->win_width)
			buffering_pixel(x, y, mlx, mlx->map->ceiling_color_hex);
	}
	while (y < mlx->win_height)
	{
		x = -1;
		while (++x < mlx->win_width)
			buffering_pixel(x, y, mlx, mlx->map->floor_color_hex);
		y++;
	}
}

/*
	Función para renderizar el suelo y el techo de manera más rápida.

	Detalles:
	- En lugar de pintar píxel a píxel, esta función pinta línea por línea 
	  utilizando la función `buffering_line`. Esto reduce la cantidad de 
	  llamadas a la función de bufferización y mejora el rendimiento.
	- Divide la ventana en dos partes: techo y suelo. Calcula la línea del 
	  horizonte (centro de la ventana) basada en la altura de la ventana y el 
	  desplazamiento vertical del jugador (`pitch_pix`).
	- Pinta todas las líneas por encima del horizonte con el color del techo y 
	  todas las líneas por debajo del horizonte con el color del suelo.

	Limitaciones:
	- Esta función solo puede usarse cuando el renderizado del suelo y el techo
	  no están texturizados, ya que no se realizan cálculos por píxel.

	Mejoras de rendimiento:
	- Reduce la sobrecarga de llamadas a funciones al pintar líneas completas 
	  en lugar de píxeles individuales.
	- Minimiza las indirecciones dentro del bucle, permitiendo un acceso más 
	  eficiente a las variables (usando registros en lugar de registro -> 
	  caché -> RAM).
	- Utiliza referencias locales para evitar accesos repetidos a estructuras 
	  anidadas, mejorando el rendimiento dentro de las restricciones de la 
	  norma de 42.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Esta función es ideal para mejorar el rendimiento en escenas donde el 
	renderizado del suelo y el techo no requieren texturas ni efectos avanzados
*/
void	render_floor_and_ceiling_speed(t_mlx *mlx)
{
	int	y;
	int	ceiling_color;
	int	floor_color;
	int	refs[3];

	refs[HEIG] = mlx->win_height;
	refs[WIDTH] = mlx->win_width;
	refs[HOR] = (refs[0] >> 1) + mlx->player->pitch_pix;
	ceiling_color = mlx->map->ceiling_color_hex;
	floor_color = mlx->map->floor_color_hex;
	y = 0;
	while (y < refs[HOR] && y < refs[HEIG])
	{
		buffering_line(y, ceiling_color, mlx, refs[WIDTH]);
		y++;
	}
	while (y < refs[HEIG])
	{
		buffering_line(y, floor_color, mlx, refs[WIDTH]);
		y++;
	}
}

/*
	Función para renderizar el suelo y el techo con efectos de ambiente.

	Detalles:
	- Similar a `render_floor_and_ceiling_speed`, pero aplica efectos de 
	  sombreado y niebla basados en la distancia desde la cámara.
	- Calcula un factor de distancia (`dist_factor`) para cada línea, que 
	  determina la intensidad de los efectos de ambiente.
	- Aplica sombreado y niebla al color del suelo y el techo antes de 
	  dibujarlos, creando una sensación de profundidad y atmósfera en la escena 
	  3D.
	- El sombreado y la niebla se aplican utilizando funciones específicas como 
	  `applyamb_ceiling` y `applyamb_floor`, que ajustan los colores según el 
	  factor de distancia.

	Mejoras de rendimiento:
	- Reduce la sobrecarga de llamadas a funciones al pintar líneas completas 
	  en lugar de píxeles individuales.
	- Minimiza las indirecciones dentro del bucle, permitiendo un acceso más 
	  eficiente a las variables (usando registros en lugar de registro -> 
	  caché -> RAM).
	- Utiliza referencias locales para evitar accesos repetidos a estructuras 
	  anidadas, mejorando el rendimiento dentro de las restricciones de la 
	  norma de 42.
	- Evita el uso de condiciones dentro del bucle para aplicar los efectos de 
	  ambiente, eliminando saltos de instrucción que penalizan el rendimiento 
	  del procesador en hoot loops.

	Parámetros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  toda la información del juego.

	Esta función mejora significativamente la calidad visual al añadir efectos 
	de profundidad y atmósfera, aunque con un mayor coste computacional debido 
	a los cálculos adicionales por píxel.
*/
void	render_floor_and_ceiling_amb(t_mlx *mlx)
{
	int				y;
	int				refs[3];
	float			df;
	unsigned int	colors[2];

	colors[0] = mlx->map->ceiling_color_hex;
	colors[1] = mlx->map->floor_color_hex;
	refs[HEIG] = mlx->win_height;
	refs[WIDT] = mlx->win_width;
	refs[HOR] = (refs[0] >> 1) + mlx->player->pitch_pix;
	y = 0;
	while (y < refs[HOR] && y < refs[HEIG])
	{
		df = dist_factor_ceiling(y, refs[HOR], mlx->amb.ambiance);
		colors[0] = apllyamb_ceiling(&(mlx->amb), df, colors[0]);
		buffering_line(y, colors[0], mlx, refs[WIDT]);
		y++;
	}
	while (y < refs[HEIG])
	{
		df = dist_factor_floor(refs[HEIG], y, refs[HOR], mlx->amb.ambiance);
		colors[1] = apllyamb_ceiling(&(mlx->amb), df, colors[1]);
		buffering_line(y, colors[1], mlx, refs[WIDT]);
		y++;
	}
}
