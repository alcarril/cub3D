/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   phisics_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/18 17:57:44 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 08:59:10 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Funcion que normaliza un vector de velocidad en 2D (x e y) en funcion
	del valor maximo de velocidad permitido. Si la magnitud del vector
	supera el valor maximo se escala el vector para que su magnitud sea
	igual al valor maximo.
	El modulo del vector se calcula usando el terorema de pitagoras.
	(hipotenusa^2 = cateto1^2 + cateto2^2)
	Con esto evitamos el strafeing speed o velocidad de deslizamiento que 
	courre en mucho videojuegos. Muy sensacion doomlike and nukelike
*/
void	normalize_vector(float *v_speed, float *max_speed)
{
	float	total_speed;

	total_speed = sqrtf((v_speed[X] * v_speed[X]) + (v_speed[Y] * v_speed[Y]));
	if (total_speed > max_speed[X])
	{
		v_speed[X] *= max_speed[X] / total_speed;
		v_speed[Y] *= max_speed[X] / total_speed;
	}
}
