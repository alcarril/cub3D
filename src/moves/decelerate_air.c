/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decelerate_air.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:28:39 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/21 17:53:57 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"


/*
	Funcion que sirve para poder decelerar al jugador en su trayectoria en el plano
	del mapa cunado esta en el aire. Funciona igual que la funcionde desaceleracion de 
	la velocidad plicando la fuerza contraria a la aceleracion en el plano "2D" mediante 
	la friccion del aire (normalemte mas baja que la friccion del suelo). Esto nor permite que
	el jugador cuando salte renga un trayectoria parabolica.
*/
void	decelerate_player_air(t_mlx *mlx, long long dt)
{
	float	speed;
	float	friction;

	speed = mlx->player->speed_dt;
	if (speed <= 0.0f)
		return;
	friction = mlx->phisics.air_friction;
	speed -= speed * friction * (float)dt;
	if (speed < SPEEDMIN_MS)
		speed = 0.0f;
	mlx->player->speed_dt = speed;
}

/*
	Hace lo mismo que la funcion de desaceleracion del jugador en el aire pero tenniedndo en cuenta
	que el vector de la veloidad esta descompuesto en sus dos componenetes (x e y).
*/
void	decelerate_dukedoom_air(t_mlx *mlx, long long dt)
{
	float 		speed[2];
	float 		friction;

	speed[X] = mlx->player->speed_a[X];
	speed[Y] = mlx->player->speed_a[Y];
	friction = mlx->phisics.air_friction;
	if(speed[X] > SPEEDMIN_MS)
		speed[X] -= speed[X] * friction * (float)dt;
	if (speed[X] <= SPEEDMIN_MS)
		speed[X] = 0.0f;
	if (speed[Y] > SPEEDMIN_MS)
		speed[Y] -= speed[Y] * friction * (float)dt;
	if (speed[Y] <= SPEEDMIN_MS)
		speed[Y] = 0.0f;
	mlx->player->speed_a[X] = speed[X];
	mlx->player->speed_a[Y] = speed[Y];
}