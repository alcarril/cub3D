/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectorization_moves.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:25:18 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/21 18:54:17 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Calculo del movimiento del jugador en cada eje del plano. Descomponemos el 
	moviento y lo bufferizamos dentro del array diferencial. Para ello, sabiendo que estamos
	tratando con un vector unitario usamos los principios de trigonometria de la circunferencia 
	gonimetrica (cision geometrica) o matriz de traslacion (vision algebraica) y calculamos el
	diferencial  usando las ecucciones paramtricas que definene como se mueve un punto teniendo
	un vetor de movimiento y una posisicon inicial en un espacio euclideo y el escalar (velocidad * dt = distancia)
	.Segun el angulo de rotacion 
	del jugador respeto a los ejes del plano y la direcion relativa en la que queramos
	movernos el difrecial se calcula. Descomponemos el moviento en cada uno de us ejes para saber
	cual es laposicion fianl del jugador en cada uno de los ejes del plano segun el vector de moviento
	Primero guardamos el vector de moviento en funcion de las teclas pulsadas y despues calculamos el diferencial
	Aplicamos la velocidad y el diferencial de tiempo para obtener el desplazamiento final en cada eje.
	- Cosas a tener en cuenta:
		- En la ventana el NORTE (90 grados) y el SUR (270) estan invertidos.
		- Hay un vectorixacion diferente para cada uno de los movientos dentro
		de la circunferencia (cada uno de los cuadrantes del plano)
		- En ver de sumar 90 grados para los movimientos laterales se podria usar
		la relacion de signos de la matriz de rotacion
	Optimizacione sde procesasdor:
	- Elimino funoperacinoes aritemetizza de division porque consument muxhocilos de CPU
		
*/
void	vectorization(t_player *pl, long long dt, float speed, float angle)
{
	ft_bzero(pl->diff, sizeof(pl->diff));
	ft_bzero(pl->v_move, sizeof(pl->v_move));
	if (pl->keys.move_up)
	{
		pl->v_move[X] += cos(angle);
		pl->v_move[Y] -= sin(angle);
	}
	if (pl->keys.move_down)
	{
		pl->v_move[X] -= cos(angle);
		pl->v_move[Y] += sin(angle);
	}
	if (pl->keys.move_left)
	{
		pl->v_move[X] += cos(angle + (PI * 0.5f));
		pl->v_move[Y] += sin(angle - (PI * 0.5f));
	}
	if (pl->keys.move_right)
	{
		pl->v_move[X] += cos(angle - (PI * 0.5f));
		pl->v_move[Y] += sin(angle + (PI * 0.5f));
	}
	pl->diff[X] = pl->v_move[X] * speed * dt;
	pl->diff[Y] = pl->v_move[Y] * speed * dt;
}


/*
   Funcion que calcula el diferencial del moviento del jugador en el modo dukedoom
   Funciona igualq ue la vectorizacion normal pero se puede meter un velocidad diferencial+
   en cada uno de los ejes viene de una aceleracion diferencial. Da un efecto mucho mas realista y permite
   simular efectos externos como viento, explosiones etc.
*/
void	vectorization_dukedoom(t_player *pl, long long dt, float angle)
{
	ft_bzero(pl->diff, sizeof(pl->diff));
	ft_bzero(pl->v_move, sizeof(pl->v_move));
	if (pl->keys.move_up)
	{
		pl->v_move[X] += cos(angle);
		pl->v_move[Y] -= sin(angle);
	}
	if (pl->keys.move_down)
	{
		pl->v_move[X] -= cos(angle);
		pl->v_move[Y] += sin(angle);
	}
	if (pl->keys.move_left)
	{
		pl->v_move[X] += cos(angle + (PI * 0.5f));
		pl->v_move[Y] += sin(angle - (PI * 0.5f));
	}
	if (pl->keys.move_right)
	{
		pl->v_move[X] += cos(angle - (PI * 0.5f));
		pl->v_move[Y] += sin(angle + (PI * 0.5f));
	}
	pl->diff[X] = pl->v_move[X] * pl->speed_a[X] * dt;
	pl->diff[Y] = pl->v_move[Y] * pl->speed_a[Y] * dt;
}


/*
	Funcion que sirve para conservar la trayectoria en el palno 2D cunado el jugador esta
	saltando mintras tiene velocidad. Calculamos la sisguiente posiscion del jugador en base
	al cevtor de moviento que conservamos (principio de conservacion del moviento). Esto nos permite
	hacer que cuando el jugador salte mantenga su trayectoria.
*/
void	airborne_vectorization(t_player *pl, long long dt, bool is_dukedoom)
{
	if(is_dukedoom == false)
	{
		pl->diff[X] =  pl->v_move[X] * pl->speed_dt * dt;
		pl->diff[Y] =  pl->v_move[Y] * pl->speed_dt * dt;
	}
	else
	{
		pl->diff[X] = pl->v_move[X] * pl->speed_a[X] * dt;
		pl->diff[Y] = pl->v_move[Y] * pl->speed_a[Y] * dt;
	}
}


