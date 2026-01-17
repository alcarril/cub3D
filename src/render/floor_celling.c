/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor_celling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 16:45:21 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/16 23:59:19 by alejandro        ###   ########.fr       */
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
	Hace lo mismo que render_floor_and_ceiling pero de forma más rápida
	- En lugar de pintar píxel a píxel, pinta línea por línea usando
	  la función buffering_line
	- Con esto se reduce la cantidad de llamadas a la función de biferiado
	  y se mejora el rendimiento.
	- Solo se puede usar cuando el renderizado del suelo y el techo no estan texturizados
	Mejoras de microprocesador:
	- Se reduce la sobrecarga de llamadas a funciones al pintar líneas completas
	  en lugar de píxeles
	- Se minimiza las indirecciones dentro del bucle para que puedan acceder a las variables
	  de forma más eficiente (mediante registros en lugar de registro -> cache -> RAM)
	- Se usan referencias locales para evitar accesos repetidos a estructuras anidadas
*/
void render_floor_and_ceiling_speed(t_mlx *mlx)
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
	while (y < mlx->win_height)
	{
		buffering_line(y, floor_color, mlx, refs[WIDTH]);
		y++;
	}
}

/*
	Función para renderizar el suelo y el techo con efectos de ambiente
	- Similar a render_floor_and_ceiling_speed pero aplica efectos de
	  sombreado y niebla basados en la distancia desde la cámara
	- Calcula un factor de distancia para cada línea que determina
	  cuánto se deben aplicar los efectos de ambiente
	- Usa funciones específicas para aplicar sombreado y niebla al color
	  del suelo y el techo antes de dibujarlos
	- Esto crea una sensación de profundidad y atmósfera en la escena 3D
	- Más costoso computacionalmente debido a los cálculos adicionales por píxel
	  pero mejora significativamente la calidad visual
	Mejoras de microprocesador:
	- Se reduce la sobrecarga de llamadas a funciones al pintar líneas completas
	  en lugar de píxeles
	- Se minimiza las indirecciones dentro del bucle para que puedan acceder a las variables
	  de forma más eficiente (mediante registros en lugar de registro -> cache -> RAM)
	- Se usan referencias locales para evitar accesos repetidos a estructuras anidadas dentro de
	las restrciiones de la norma de 42
	- se omite la opcion de meter las opciones de ambiente en un if dentro de un bucle
	  para evitar saltos de instruccion que penalizan el rendimiento del procesador
*/
void render_floor_and_ceiling_amb(t_mlx *mlx)
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
		colors[0] =	apllyamb_ceiling(&(mlx->amb), df, colors[0]);
		buffering_line(y, colors[0], mlx, refs[WIDT]);
		y++;
	}
	while (y < refs[HEIG])
	{
		df = dist_factor_floor(refs[HEIG], y, refs[HOR], mlx->amb.ambiance);
		colors[1] =	apllyamb_ceiling(&(mlx->amb), df, colors[1]);
		buffering_line(y, colors[1], mlx, refs[WIDT]);
		y++;
	}
}


//cersion profundidad PORTOTIPO DE CREACION
void render_floor_and_ceiling_speed2(t_mlx *mlx)
{
	int y;
	int horizon;
	float dist_factor;
	unsigned int color_ceiling;
	unsigned int color_floor;
	unsigned int fog_color = FOG_MEDIO_CLARO; // niebla más oscura


	color_ceiling = mlx->map->ceiling_color_hex;
	color_floor = mlx->map->floor_color_hex;
	horizon = (mlx->win_height >> 1) + mlx->player->pitch_pix;
	y = 0;
	while (y < horizon)
	{
		// dist_factor = 1.0f - (float)(y - horizon) / horizon; //NIEBLA TOTAL
		dist_factor = (float)(horizon - y) / horizon; //AMBIENTE ABIERTO SOL DETRAS PAREDES
		// dist_factor = 1.0f - (float)(horizon - y) / horizon; //-> NIEBLA CON SOL ARRIBA DESCARTADA
		
		if (dist_factor < 0.0f)
			dist_factor = 0.0f;	
		if (dist_factor > 1.0f)
			dist_factor = 1.0f;
		float k = 4.0f;//->ESTE VALOR ES AJUSTABLE
		color_ceiling =	shade_inverse(mlx->map->ceiling_color_hex, k, dist_factor * 0.5f);// valores pequeñas en configuracions de ambiente queda bien y el configuracions de niebla grandes
		// color_ceiling = apply_desaturation(color_ceiling, dist_factor * 0.6f);
		color_ceiling = apply_fog_pixel(color_ceiling, fog_color, dist_factor * 0.2f);// valores pequeñas en configuracions de ambiente queda bien y el configuracions de niebla grandes
		buffering_line(y, color_ceiling, mlx, mlx->win_width);
		y++;
	}
	while (y < mlx->win_height)
	{
		//siemrep en eld enominador  hwight - horizon para que no crashee
		//si poner horizon - y es mas grados pero con y - horizon es mas fucnoinal difuminado se funde mejor
		
		//al haber cambiado los de horizon - y si quito el 1.0f - rompe y se pone negro
		// dist_factor = 1.0f - (float)( horizon - y) / (mlx->win_height - horizon); // NIEBLA TOTAL PERFECTO
		dist_factor = 1.0f - (float)( y - horizon) / (mlx->win_height - horizon); // AMBIENTE ABIERTO SOL DETRAS PARAREDES (cerca claro lejos oscuro)
		// dist_factor = (float)( y - horizon) / (mlx->win_height - horizon); // AMBIENTE ABOERTO SOL DERTAS JUGADOR SIEMPRE DESCARTADO
		
		if (dist_factor < 0.0f)
			dist_factor = 0.0f;	
		if (dist_factor > 1.0f)
			dist_factor = 1.0f;
		
		// Inverse shading fuerte para suelo
		
		float k = 1.0f;
		
		color_floor = shade_inverse(mlx->map->floor_color_hex, k, dist_factor * 0.7f);
		color_floor = apply_desaturation(color_floor, dist_factor * 0.6f);
		// Aplicar fog usando tu función
		color_floor = apply_fog_pixel(color_floor, FOG_MEDIO_CLARO, dist_factor * 0.3f);

		buffering_line(y, color_floor, mlx, mlx->win_width);
		y++;
	}
}

