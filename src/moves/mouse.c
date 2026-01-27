/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 01:48:42 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 20:42:14 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función que obtiene la posición actual del mouse en la ventana y la compara
	con la posición de referencia (axis_x, axis_y). Si hay diferencia en algún 
	eje, se acota el movimiento y se actualiza el ángulo de visión (eje x) o el
	pitch (eje y) del jugador según la sensibilidad o factor de pitch por 
	píxel. Finalmente, resetea la posición del mouse a la posición de 
	referencia.

	Esta función se ejecuta si el mouse está activo y en cada frame del juego.
	Es más eficiente que renderizar por eventos, ya que así nos aseguramos un 
	movimiento fluido a la velocidad del frame rate del juego. Si usamos 
	eventos, estamos limitados por la frecuencia de eventos del sistema 
	operativo (que suele ser más baja que la del frame rate del juego).

	Además, si da la casualidad de que se han cancelado muchos movimientos en 
	un eje y se han ido sumando a la posición del mouse hasta salirse de la 
	ventana, se fuerza la posición del mouse a la posición de referencia en 
	ambos ejes. Si se hace un movimiento de píxeles en pantalla muy brusco, 
	como esta función se ejecuta en cada frame, la única manera de hacer que el
	mouse esté siempre en la ventana es resetearlo en cada frame. Sin embargo, 
	esto tiene un coste debido a cómo gestiona la cola de eventos la mini libx 
	en conjunto con el kernel. Por lo tanto, es mejor llamarla solo cuando se 
	hace un movimiento efectivo (140 frames por segundo serían 140 llamadas, 
	independientemente de que el mouse se haya movido más o menos veces en ese 
	intervalo de tiempo). Siempre es mejor detectar en cada frame si se ha 
	movido o no y resetear solo si se ha movido en algún eje o se ha salido de 
	la ventana.

	Con esta mejora, si apagamos el mouse, movemos el ratón y lo volvemos a 
	encender fuera de la pantalla, automáticamente se centra. Para controlar 
	que esto no pase, se introduce una variable extra que sirve para saber si 
	el mouse está fuera de la ventana y encendido. Esto nos permite que, si 
	esto ocurre, simplemente se salga de la función a menos que detecte que el 
	mouse está dentro de la ventana, en cuyo caso esta variable se apaga 
	(false). Con esta variable, podríamos eliminar el uso de Motion Notify 
	para saber si el mouse ha entrado en la ventana en la primera iteración, 
	pero usando Motion Notify nos aseguramos de que en cada frame no se 
	compruebe la posición del mouse si no es necesario. Esto evita dos llamadas
	a funciones de la API de mlx que ralentizan un poco el funcionamiento del 
	programa, y si se puede evitar, mejor. Además, gracias al branch predictor,
	el `if` del render no consume excesivos ciclos de procesador cuando el 
	contador de programa ha pasado por ahí varias veces.

	Con este ajuste, cuando el mouse se activa con el puntero en la ventana, lo
	interpreta directamente y lo mueve. Como no queremos que esto pase, en el 
	manejador reseteamos su posición al eje o no, según donde se encuentre 
	cuando se activa. Si se activa dentro de la ventana, lo centramos para que 
	no haga un movimiento brusco.

	Mejora de microprocesador:
	- Como es una función que se llama en cada frame, se han optimizado el 
	  número de variables locales al mínimo necesario, intentando hacer el 
	  motor lo más eficiente posible, al igual que con el resto de funciones 
	  que se ejecutan dentro del loop de renderizado.
*/
void	get_mouse_pos_and_move(t_mlx *mlx)
{
	t_mouse	*m;
	int		pix_dif[2];
	bool	is_move[2];

	m = &(mlx->player->mouse);
	mlx_mouse_get_pos(mlx->mlx_var, mlx->mlx_window, &(m->pos_x), &(m->pos_y));
	if (is_mouse_in_window(mlx, m->pos_x, m->pos_y) == OUT)
	{
		if (m->out_and_on == true)
			return ;
		mlx_mouse_move(mlx->mlx_var, mlx->mlx_window, m->axis_x, m->axis_y);
	}
	else
		m->out_and_on = false;
	ft_bzero((void *)is_move, sizeof(is_move));
	move_player_with_mouse(mlx, pix_dif, is_move);
	reset_mouse_position(mlx, is_move);
}

/*
	Funcion que mueve al jugador segun la diferencia de pixeles
	del mouse en cada eje respecto a la posicion de referencia
	(axis_x, axis_y). Si hay moviento en el eje x se actualiza
	el angulo de vision del jugador segun la sensibilidad por pixel
	del mouse. Si hay moviento en el eje y se actualiza el pitch
	del jugador segun el factor de pitch por pixel del mouse.
	Si hay moviento en algun eje se marca en is_move para que luego
	se resete la posicion del mouse en ese eje.
	Parametros:
	- mlx: estructura principal del motor grafico
	- pix_dif: array de 2 enteros donde se guarda la diferencia
	  de pixeles del mouse en cada eje respecto a la posicion
	  de referencia
	- is_move: array de 2 booleanos donde se marca si ha habido
	  moviento en cada eje para luego resetear la posicion del
	  mouse en ese eje
*/
void	move_player_with_mouse(t_mlx *mlx, int *pix_dif, bool *is_move)
{
	t_mouse	*m;

	m = &(mlx->player->mouse);
	pix_dif[X] = m->pos_x - m->axis_x;
	if (clamp_mouse_deltax(&pix_dif[X]) == true)
	{
		is_move[X] = 1;
		mlx->player->angle -= pix_dif[X] * m->sens_x;
		mlx->player->rad_angle = mlx->player->angle * (PI / 180.0f);
	}
	pix_dif[Y] = m->pos_y - m->axis_y;
	if (clamp_mouse_deltay(&pix_dif[Y]) == true)
	{
		is_move[Y] = 1;
		mlx->player->pitch_pix -= pix_dif[Y]
			* (m->pitch_factor * mlx->player->max_pitch_pix);
	}
}

/*
	Comprueba si la posición del mouse (mouse_x, mouse_y) está dentro de los 
	limites de la ventana (0 a win_width, 0 a win_height). Esto es útil para 
	asegurarnos de que, una vez que el mouse esté activado, se encuentre dentro
	de la ventana. Esto es importante porque el mouse puede activarse mientras 
	el cursor está fuera de la ventana (por ejemplo, al usar multitarea), y no 
	queremos que se tengan en cuenta los movimientos del mouse hasta que entre 
	en la ventana.

	Retornos:
	- true: Si el mouse está dentro de la ventana.
	- false: Si el mouse está fuera de la ventana.
*/
bool	is_mouse_in_window(t_mlx *mlx, int mouse_x, int mouse_y)
{
	if (mouse_x >= 0 && mouse_x < mlx->win_width
		&& mouse_y >= 0 && mouse_y < mlx->win_height)
	{
		return (IN);
	}
	return (OUT);
}

/*
	Acota el movimiento del mouse (pixeles de diferencia) en el eje X
	de la matriz de pixeles de la ventana, para que cuando se vaya a
	tener en cuenta para ser multiplicado por la sensibilidad por pixel
	no se tengan movimientos muy bruscos. Ademas establece una deadzone
	en pixeles para que lo movuientos muy pequeños de la muñeca del usuario
	del juego en el eje x no se tengane en cuenta
	Valores de retorno:
	- true: si el movimiento es mayor a la deadzone se acota
	- false: si el movimiento esta dentro de la deadzone se cancela
*/
bool	clamp_mouse_deltax(int *pix_dif)
{
	if (*pix_dif > -MOUSE_DEADZONEX && *pix_dif < MOUSE_DEADZONEX)
		return (false);
	if (*pix_dif > MOUSE_MAX_MOV)
		*pix_dif = MOUSE_MAX_MOV;
	if (*pix_dif < -MOUSE_MAX_MOV)
		*pix_dif = -MOUSE_MAX_MOV;
	return (true);
}

/*
	Acota el movimiento del mouse (pixeles de diferencia) en el eje Y
	de la matriz de pixeles de la ventana, para que cuando se vaya a
	tener en cuenta para ser multiplicado por el factor de pitch por pixel
	no se tengan movimientos muy bruscos. Ademas establece una deadzone
	en pixeles para que lo movuientos muy pequeños de la muñeca del usuario
	del juego en el eje y no se tengan en cuenta
	Valores de retorno:
	- true: si el movimiento es mayor a la deadzone se acota
	- false: si el movimiento esta dentro de la deadzone se cancela
*/
bool	clamp_mouse_deltay(int *pix_dif)
{
	if (*pix_dif > -MOUSE_DEADZONEY && *pix_dif < MOUSE_DEADZONEY)
		return (false);
	if (*pix_dif > MOUSE_MAX_MOV)
		*pix_dif = MOUSE_MAX_MOV;
	if (*pix_dif < -MOUSE_MAX_MOV)
		*pix_dif = -MOUSE_MAX_MOV;
	return (true);
}
