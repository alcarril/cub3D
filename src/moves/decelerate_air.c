/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decelerate_air.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:28:39 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 07:08:50 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Funcion que sirve para poder decelerar al jugador en su trayectoria en el 
	plano del mapa cunado esta en el aire. Funciona igual que la funcionde 
	desaceleracion de la velocidad plicando la fuerza contraria  la aceleracion
	en el plano "2D" mediante la friccion del aire (normalemte mas baja que 
	la friccion del suelo).
	Como el jugador no tiene control sobre su movimiento en el aire, la unica 
	fuerza que actua sobre el es la friccion del aire, que reduce su velocidad
	gradualmente hasta detenererlo.
	La direccion que describe el cambio de posicion del juggador (diferencial 
	del vector de moviento del jugadir se conserva, pero la magnitud de este 
	vector disminuye con el tiempo.) Esto nor permite que el jugador cuando 
	salte renga un trayectoria parabolica.
	Cunado la velocidad del jugador baja de un umbral minimo (SPEEDMIN_MS)
	esta se pone a 0 para evitar movimientos calculos innecesarios. ya que 
	la friccion eesun porcentaje de la velocidad actual y nunca llegaria a 0.
	asi damos menos trabajo a la CPU.
	Variables:
		speed: velocidad actual del jugador en el aire.
		friction: coeficiente de friccion del aire que afecta al jugador.
		dt: delta time, tiempo transcurrido desde la ultima actualizacion.
*/
void	decelerate_player_air(t_mlx *mlx, long long dt)
{
	float	speed;
	float	friction;

	speed = mlx->player->speed_dt;
	if (speed <= 0.0f)
		return ;
	friction = mlx->phisics.air_friction;
	speed -= speed * friction * (float)dt;
	if (speed < SPEEDMIN_MS)
		speed = 0.0f;
	mlx->player->speed_dt = speed;
}

/*
	Hace lo mismo que la funcion decelerate_player_air pero en el
	plano 2D, afectando a las componentes X e Y de la velocidad del jugador.
*/
void	decelerate_dukedoom_air(t_mlx *mlx, long long dt)
{
	float	speed[2];
	float	friction;

	speed[X] = mlx->player->speed_a[X];
	speed[Y] = mlx->player->speed_a[Y];
	friction = mlx->phisics.air_friction;
	if (speed[X] > SPEEDMIN_MS)
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
