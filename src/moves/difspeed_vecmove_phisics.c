/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   difspeed_vecmove_phisics.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:03:22 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 07:52:09 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Funcion directora de la actulizacion de la posicion del jugador en el mapa
	cuando las fisicas estan activadas en cada frame de hoot loop de
	renderizado.
	Tiene dos modos de funcionamiento:
	 - Cunado el jugador esta en en el suelo aplica las aceleraciones del
	 jugador en funion de si el moviento es fornta (FRONT), lateral (SIDES)
	 o hacia atras (BACK). Despues aplica la desaceleracion de la velocidad
	 del jugador. Usamos la variable is_moving para saber si el jugador esta
	 moviendose o no y asi aplicar una desaceleracion mayor o menor. Depues
	 se usa la velocidad calculada para actualizar la posicion del jugador
	 en el mapa mediante moviento vectorial.
	 - Cunado el jugador esta en el aire se desactivan las aceleraciones
	 en el plano 2D para que el jugador nopueda controlar su moviento aereo.
	 Solo se aplica la desaceleracion de la velocidad en el aire y se actualiza
	 la posicion del jugador en el mapa mediante moviento vectorial en el aire
	 conservando la direccion del vector de moviento.
	
	Esta funcion permite que el jugadro cunete con una sensacion de moviento
	mas realista y fluida, ya que las aceleraciones y desaceleraciones sen
	tan mas naturales. Ademas podemos ocnfigurarar estas para generar distintos
	sensaciones de moviento segun el tipo de juego. (inercias, deslizamientos,
	fricciones, etc).
	Variables:
		pl: puntero a la estructura del jugador.
		is_moving: variable booleana que indica si el jugador esta moviendose
		o no.
		delta_time: tiempo transcurrido desde la ultima actualizacion en
		milisegundos.
*/
void	difspeed_and_vecmove(t_mlx *mlx, long long delta_time)
{
	t_player	*pl;
	bool		is_moving;

	pl = mlx->player;
	is_moving = 0;
	if (mlx->player->is_onair == false)
	{
		if (pl->keys.move_up == true)
			acelerate_player(mlx, FRONT, delta_time, &is_moving);
		if (pl->keys.move_down == true)
			acelerate_player(mlx, BACK, delta_time, &is_moving);
		if (pl->keys.move_left == true || pl->keys.move_right == true)
			acelerate_player(mlx, SIDES, delta_time, &is_moving);
		decelerate_player(mlx, delta_time, is_moving);
		vectorization(mlx->player, delta_time, pl->speed_dt, pl->rad_angle);
	}
	else
	{
		decelerate_player_air(mlx, delta_time);
		airborne_vectorization(pl, delta_time, false);
	}
}

/*
	Funcion directora de la actulizacion de la posicion del jugador en el mapa
	cuando las fisicas estan activadas en cada frame de hoot loop de
	renderizado.
	Funcion igual que la funcion con fisicas difspeed_and_vecmove pero pero
	adaptada al sistema de moviento tipo Duke Nukem 3D. Es decir aceleraciones
	del vector de moviento diferenciales. Mediante la decomposicion del vector
	de velocidad en sus dos componenetes permitinedosnos asi un control mas
	fino sobre el moviento del jugador y crear sensaciones mas realistas y la 
	interaccion del movientos del jugador con el entorno (expliasions, golpes
	, vientos, terrenos fanganosos, etc).

	Variables:
		pl: puntero a la estructura del jugador.
		is_moving: variable booleana que indica si el jugador esta moviendose
		o no.
		delta_time: tiempo transcurrido desde la ultima actualizacion en
		milisegundos.
*/
void	difvspeed_and_vecmove_nukedoom(t_mlx *mlx, long long delta_time)
{
	t_player	*pl;
	bool		is_moving;

	pl = mlx->player;
	is_moving = 0;
	if (mlx->player->is_onair == false)
	{
		if (pl->keys.move_up == true)
			acelerate_dukedoom(mlx, FRONT, delta_time, &is_moving);
		if (pl->keys.move_down == true)
			acelerate_dukedoom(mlx, BACK, delta_time, &is_moving);
		if (pl->keys.move_left == true || pl->keys.move_right == true)
			acelerate_dukedoom(mlx, SIDES, delta_time, &is_moving);
		decelerate_dukedoom(mlx, delta_time, is_moving);
		vectorization_dukedoom(mlx->player, delta_time, pl->rad_angle);
	}
	else
	{
		decelerate_dukedoom_air(mlx, delta_time);
		airborne_vectorization(pl, delta_time, true);
	}
}
