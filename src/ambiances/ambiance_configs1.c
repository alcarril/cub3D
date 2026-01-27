/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ambiance_configs1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 20:24:57 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/22 20:58:38 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Calcla el factor de dissipacion que se va a usar en los shader y fogs
	para el suelo en funcion de la posicion del pixel de la matriz de pixeles
	respecto al eje de la pantalla con la trasformacion de los puntos segun el 
	pitch del jugador, es decir el horizonte real simulado por el pitch del 
	jugador. Cada ambiente usa un tipo de calculo diferente.
	Despues se acota el factor entre 0.0 y 1.0
*/
float	dist_factor_floor(int win_height, int win_y, int horizon, int ambient)
{
	float	dist_fac;

	if (ambient == CEMENTERY)
		dist_fac = 1.0f - (float)(horizon - win_y) / (win_height - horizon);
	else if (ambient == ASTURIAS)
		dist_fac = (float)(win_y - horizon) / (win_height - horizon);
	else if (ambient == OPEN)
		dist_fac = 1.0f - (float)(win_y - horizon) / (win_height - horizon);
	if (dist_fac < 0.0f)
		dist_fac = 0.0f;
	if (dist_fac > 1.0f)
		dist_fac = 1.0f;
	return (dist_fac);
}

/*
	Calcla el factor de dissipacion que se va a usar en los shader y fogs
	para el techo en funcion de la posicion del pixel de la matriz de pixeles
	respecto al eje de la pantalla con la trasformacion de los puntos segun el 
	pitch del jugador, es decir el horizonte real simulado por el pitch del 
	jugador. Cada ambiente usa un tipo de calculo diferente.
	Despues se acota el factor entre 0.0 y 1.0
*/
float	dist_factor_ceiling(int win_y, int horizon, int ambient)
{
	float	dist_factor;

	if (ambient == CEMENTERY)
		dist_factor = 1.0f - (float)(win_y - horizon) / horizon;
	else if (ambient == ASTURIAS)
		dist_factor = 1.0f - (float)(horizon - win_y) / horizon;
	else if (ambient == OPEN)
		dist_factor = (float)(horizon - win_y) / horizon;
	if (dist_factor < 0.0f)
		dist_factor = 0.0f;
	if (dist_factor > 1.0f)
		dist_factor = 1.0f;
	return (dist_factor);
}

/*
	Aplica efectos de ambiente al color del techo
	- Usa los parámetros de ambiente para calcular el sombreado y la niebla
	  basados en el factor de distancia
	- Aplica sombreado inverso para oscurecer el color con la distancia
	- Aplica desaturación para simular pérdida de color con la distancia
	- Aplica niebla para mezclar el color con el color de niebla basado en 
	la distancia
	- Devuelve el color modificado listo para renderizar
	- Segun las configuraciones de ambiente del juego conseguimos esfectos 
	 esteticos diferentes
*/
unsigned int	apllyamb_ceiling(t_ambiance *a, float df, unsigned int rcol)
{
	unsigned int	col;
	float			mult_shader;
	float			mult_fog;
	float			k_factor;
	int				fog_color;

	mult_shader = a->mult_shader_ceiling;
	mult_fog = a->mult_fog_ceiling;
	k_factor = a->k_factor_ceiling;
	fog_color = a->fog_color_fc;
	col = shade_inverse(rcol, k_factor, df * mult_shader);
	col = apply_desaturation(col, df * 0.6f);
	col = apply_fog_pixel(col, fog_color, df * mult_fog);
	return (col);
}

/*
	Aplica efectos de ambiente al color del suelo
	- Usa los parámetros de ambiente para calcular el sombreado y la niebla
	  basados en el factor de distancia
	- Aplica sombreado inverso para oscurecer el color con la distancia
	- Aplica desaturación para simular pérdida de color con la distancia
	- Aplica niebla para mezclar el color con el color de niebla basado en la 
	 distancia
	- Devuelve el color modificado listo para renderizar
	- Segun las configuraciones de ambiente del juego conseguimos esfectos 
	esteticos diferentes
*/
unsigned int	apllyamb_floor(t_ambiance *a, float df, unsigned int rcol)
{
	unsigned int	col;
	float			mult_shader;
	float			mult_fog;
	float			k_factor;
	int				fog_color;

	mult_shader = a->mult_shader_floor;
	mult_fog = a->mult_fog_floor;
	k_factor = a->k_factor_floor;
	fog_color = a->fog_color_fc;
	col = shade_inverse(rcol, k_factor, df * mult_shader);
	col = apply_desaturation(col, df * 0.6f);
	col = apply_fog_pixel(col, fog_color, df * mult_fog);
	return (col);
}
