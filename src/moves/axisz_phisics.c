/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   axisz_phisics.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 09:02:37 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/21 18:52:33 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Funcion para calcular el valor de camz del jugador en funcion del del tiempo
	Gracias esta variable vamos a poder elevar al jugador hacer que se agache. Y con
	esto podemos saltar de diferentes maneras y alturas (segun gravesdad y configuracions
	del salto), volar, flotar etc.. NOr sirve para calcular despues el vertical offse
	en funcion del camz que nos da esa senciaon de altura al trasladar los puntos del
	frame en funcion de la distancia es decir no de manera lineal como el pitch.
	Funncio que nos permite calcula la posicion de la camara en z, que simula los ojos
	del jugador y permite crear sensacion de moviento vertical (saltos, caidas, vuelos etc..)
	Para ellos si el jugador esta en modo salto aplicamos la fuerza de salto menos la fuerza
	de gravedad del mapa. De la luvha de estas dos fuerzas en base al itmeposale la celeracion
	total que multiplicada por el diferencial de tiempo da luagar la velocidad el jugador en el ejez. 
	Si la velocidad es positiva el jugador asciende si es negativa
	el jugador desciende. Finalmente usamos multiplicamos la velocidad obtenida  por el diferen
	cial de tiempo normalizado para obtener el desplazamiento en z diferencial de la posicion)
	del jugador y lo sumamos a su posicion actual en z (camz). 
	 Por ultimo comprobamos que camz no se salga de los limites maximos y minimos
	 se comprueba en cada frame
*/
void	jump(t_mlx *mlx, long long dt_ms)
{
	t_player	*pl;
	t_phisics	*ph;
	float		dt;
	float		tm_aceleration;
	
	pl = mlx->player;
	if (pl->is_dragging || pl->is_hided || pl->is_flying || pl->is_groundpound)
		return ;
	dt = (float)dt_ms * 0.001f;
	ph = &mlx->phisics;
	if (pl->is_jumping)
		tm_aceleration = pl->aceleration_z - ph->gravity;
	else
		tm_aceleration = - ph->gravity;
	pl->speed_z += tm_aceleration * dt;
	pl->camz += pl->speed_z * dt;
	check_altitude(pl);
}

/*
	Funcion que comprueba que la posicion en z de la camara del jugador
	no se salga de los limites maximos y minimos permitidos en el mapa.
	Si se sale de los limites se corrige la posicion y se resetea la velocidad
	en z a 0 para evitar que siga moviendose en esa direccion. Tambien se desactiva
	el estado de salto y aire del jugador. Se comprueab en cada frame
*/
void	check_altitude(t_player	*pl)
{
	if (pl->camz > MAP_MAX_Z)
	{
		pl->camz = MAP_MAX_Z;
		pl->speed_z = 0.0f;
		pl->is_jumping = false;
	}
	if (pl->camz < MIDDLE_Z)
	{
		pl->camz = MIDDLE_Z;
		pl->speed_z = 0.0f;
		pl->is_jumping = false;
		pl->is_onair = false;
	}
}