/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   axisz_phisics.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 09:02:37 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 07:00:44 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Funcion que nos permite aplicar al fisicas del satltao y caida del jugador.
	Para ello modificamos el valor de la variable camz del jugador en funcion 
	de los distintos momentos del tiempo. Camz representa los ojos del jugador 
	en el eje z.
	
	Para modicar camz aplicamos las ecuaciones de movimiento rectilineo 
	uniformemente acelerado (MRUA) en el eje z. Dinde la aceleracion que impulsa 
	al jugadorhacia aribba (elevar camz) es la fuerza de salto y la que lo 
	impulsa hacia abajo (es decir que disminuye camz) es la fuerza de gravedad 
	del mapa. De la luchade las dos fuerzas obtenemos la aceleracion total en el 
	eje z (ficticio).
	Calculamos la velocidad en z del jugador sumando la aceleracion total por el 
	diferencialde tiempo (dt) al valor de la velocidad en z del frame anterior. 
	Con esto obetenemosla el valor del diferencial de al velocidad en ese frame. 
	De la multiplicacion de la velocidad en z por el diferencial de tiempo 
	obtenemos el desplazamiento del jugador en z en ese frame. Sumamos este 
	desplazamiento a la posicion actual.
	
	Usamos el tiempo para que el movimiento sea independiente de la tasa de fps. 
	(forma de hacelo en moto res graficos realistas).
	
	¿Como hacemos que el jugador salte y caiga?
	El MURUA es un tipo de movimiento en el que un objeto se desplaza en
	una linea recta con una aceleracion constante. En este caso la aceleracion
	es variable en funcion de las fuerzas que actuan sobre el jugador. No son
	fisicas realista en el eje z, la aceleracion tambien es un vector con
	un diferencial de tiempo que hace que se disipe en funcion de la energia
	potencial y cinetica. Pero para este juego es suficiente con este modelo
	simplificado. Para poder simular saltos y caidas de manera sencilla hemos
	establecido una variable booleana que marca cunado el jugador esta pulsando
	el boton de salto (is_jumping). Cuando esta variable es true aplicamos la
	fuerza de salto menos la fuerza de gravedad para calcular la aceleracion
	total en z. Cunado la variable es false porque el jugador suelta las teclas
	de salto o ya no puede saltar mas alto (ha llegado al maximo de altura del
	salto) solo aplicamos la fuerza de gravedad para que el jugador caiga.

	Porque usamos dos fuerzas opuestas si la ascenssion se controla con
	la variable is jumping?
	La razon es que de esta manera podemos ajustar la fuerza de salto y la
	fuerza de gravedad de manera independiente. Dandole diferentes configuracio
	nes a estas dos fuerzas podemos conseguir saltos mas altos o mas bajos,

	Variables:
		- pl: puntero a la estructura del jugador
		- ph: puntero a la estructura de las fisicas del juego
		- dt: diferencial de tiempo en segundos
		- tm_aceleration: aceleracion total en z del jugador en ese frame
		  compuesta por la fuerza de salto menos la fuerza de gravedad
		- dt_ms: diferencial de tiempo en milisegundos
		
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
		tm_aceleration = -ph->gravity;
	pl->speed_z += tm_aceleration * dt;
	pl->camz += pl->speed_z * dt;
	check_altitude(pl);
}

/*
	Funcion que comprueba que la altura de la camara del jugador (camz)
	este dentro de los limites permitidos por el mapa:
	- Si camz es mayor que el maximo permitido (MAP_MAX_Z) lo igualamos
	  a MAP_MAX_Z y ponemos la velocidad en z a 0 (el jugador deja de subir).
	- Si camz es menor que el minimo permitido (MIDDLE_Z) lo igualamos
	  a MIDDLE_Z y ponemos la velocidad en z a 0 (el jugador deja de bajar).
	  Ademas ponemos los booleanos is_jumping e is_onair a false porque
	  el jugador ha tocado el suelo.
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
