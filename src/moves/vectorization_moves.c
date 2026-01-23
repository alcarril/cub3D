/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectorization_moves.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:25:18 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 20:22:16 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Explicacion de la funcion:
	
	Calculo del movimiento del jugador en cada eje del plano. Descomponemos el 
	movimiento y lo almacenamos en el array diferencial. Para ello, sabiendo 
	que estamos trabajando con un vector unitario, usamos los principios de 
	trigonometría de la circunferencia goniométrica (visión geométrica) o la 
	matriz de traslación (visión algebraica). Calculamos el diferencial usando 
	las ecuaciones paramétricas que definen cómo se mueve un punto, teniendo un 
	vector de movimiento, una posición inicial en un espacio euclidiano y un 
	escalar (velocidad * dt = distancia). Según el ángulo de rotación del 
	jugador respecto a los ejes del plano y la dirección relativa en la que 
	queremos movernos, se calcula el diferencial. Descomponemos el movimiento 
	en cada uno de los ejes para determinar la posición final del jugador en 
	cada uno de los ejes del plano según el vector de movimiento. Primero 
	guardamos el vector de movimiento en función de las teclas pulsadas y luego 
	calculamos el diferencial. Aplicamos la velocidad y el diferencial de 
	tiempo para obtener el desplazamiento final en cada eje.

	- Cosas a tener en cuenta:
		- En la ventana, el NORTE (90 grados) y el SUR (270 grados) están 
		  invertidos.
		- Hay una vectorización diferente para cada uno de los movimientos 
		  dentro de la circunferencia (cada uno de los cuadrantes del plano).
		- En lugar de sumar 90 grados para los movimientos laterales, se podría 
		  usar la relación de signos de la matriz de rotación.
		- Ponemos el differencial del moviento y el vector de moviento a 0 al 
		  principio de la funcion para evitar errores de acumulacion de 
		  movimientos residuales.
	- Parametros:
		pl: puntero a la estructura del jugador.
		dt: delta time, tiempo transcurrido desde la ultima actualizacion.
		speed: velocidad del jugador en el plano.
		angle: angulo de rotacion del jugador en radianes.

	Optimizaciones de procesador:
	- Se eliminan operaciones aritméticas de división porque consumen muchos 
	  ciclos de CPU.
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
	Función que calcula el diferencial del movimiento del jugador en el modo 
	DukeDoom. Funciona de manera similar a la vectorización normal, pero permite 
	usar una velocidad diferencial en cada uno de los ejes, que proviene de una 
	aceleración diferencial. Esto da como resultado un movimiento mucho más 
	realista y permite simular efectos externos como viento, explosiones, 
	superficies deslizantes, entre otros.

	Detalles de implementación:
	- **Velocidad diferencial:** Se calcula una velocidad específica para cada
	  eje (x, y) en función de las teclas de movimiento presionadas y de las 
	  aceleraciones diferenciales aplicadas al jugador.
	- **Efectos externos:** Este método permite simular fuerzas externas que 
	  afectan de manera desigual a los ejes, como vientos laterales o 
	  explosiones.
	- **Diferencial de movimiento:** El desplazamiento final del jugador en 
	  cada eje se calcula multiplicando el vector de movimiento por la 
	  velocidad diferencial y el delta time.

	Parámetros:
	- pl: Puntero a la estructura del jugador.
	- dt: Delta time, tiempo transcurrido desde la última actualización.
	- angle: Ángulo de rotación del jugador en radianes.

	Este método es ideal para simulaciones avanzadas y realistas, donde se 
	requiere un control más preciso sobre las fuerzas que afectan al movimiento
	del jugador.
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
	Función que sirve para conservar la trayectoria en el plano 2D cuando el 
	jugador está saltando mientras tiene velocidad en el plano 2D. Calculamos 
	la siguiente posición del jugador en base al vector de movimiento que 
	conservamos (principio de conservación del movimiento). Esto nos permite 
	hacer que, cuando el jugador salte, mantenga su trayectoria en el plano 2D.

	Detalles de implementación:
	- **Conservación del movimiento:** Se utiliza el vector de movimiento 
	  calculado previamente para determinar el desplazamiento del jugador en 
	  el aire.
	- **Modo normal:** Si el modo DukeDoom no está activado, el cálculo del 
	  diferencial de movimiento utiliza una velocidad constante (speed_dt) 
	  multiplicada por el delta time.
	- **Modo DukeDoom:** Si el modo DukeDoom está activado, el cálculo del 
	  diferencial de movimiento utiliza una velocidad diferencial en cada eje 
	  (speed_a[X] y speed_a[Y]) multiplicada por el delta time.

	Parámetros:
	- pl: Puntero a la estructura del jugador.
	- dt: Delta time, tiempo transcurrido desde la última actualización.
	- is_dukedoom: Booleano que indica si el modo DukeDoom está activado.

	Este método asegura que el jugador mantenga un movimiento fluido y realista 
	durante los saltos, respetando las físicas del juego y el modo de 
	vectorización seleccionado.
*/
void	airborne_vectorization(t_player *pl, long long dt, bool is_dukedoom)
{
	if (is_dukedoom == false)
	{
		pl->diff[X] = pl->v_move[X] * pl->speed_dt * dt;
		pl->diff[Y] = pl->v_move[Y] * pl->speed_dt * dt;
	}
	else
	{
		pl->diff[X] = pl->v_move[X] * pl->speed_a[X] * dt;
		pl->diff[Y] = pl->v_move[Y] * pl->speed_a[Y] * dt;
	}
}
