/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 01:48:42 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/12 21:50:42 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Funcion que obtiene la posicion actual del mouse en la ventana
	y la compara con la posicion de referencia (axis_x, axis_y).
	Si hay diferencia en algun eje, se acota el movimiento y se
	actualiza el angulo de vision (eje x) o el pitch (eje y) del jugador
	segun la sensibilidad o factor de pitch por pixel.
	Finalmente resetea la posicion del mouse a la posicion de referencia.
	Esta funcion se  ejecuta si el mouse esta activo y cada frame del juego
	es mas eficiente que renderizar por eventos puesto que asi noas aseguramos
	un moviento fluido a la velocidad del frame rate del juego. Si usamos eventos
	estaoms limitados por la frecuencia de eventos del sistema operativo (suele ser
	mas baja que la del frame rate del juego)
	
	NOTA: Como es un funcion que se llama en cada frame se han optimizado el numero
	de variables locales a un minimo necesario intentando hacer el motor lo mas eficiente
	posible al igual que con en resto de funciones qu se ejutan dentro del loop de renderi
	zado.
*/
void	get_mouse_pos_and_move(t_mlx *mlx)
{
	t_mouse	*m;
	int		pix_dif[2];
	bool	is_move[2];
	
	m = &(mlx->player->mouse);
	mlx_mouse_get_pos(mlx->mlx_var, mlx->mlx_window, &(m->pos_x), &(m->pos_y));
	if (is_mouse_in_window(mlx, m->pos_x, m->pos_y) == false)
		return ;
	ft_bzero((void*)is_move, sizeof(is_move));
	pix_dif[X] =  m->pos_x - m->axis_x;
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
		mlx->player->pitch_pix -= pix_dif[Y] * (m->pitch_factor * mlx->player->max_pitch_pix);
	}
	reset_mouse_position(mlx, is_move);
}


/*
	Comprueba si la posicion del mouse (mouse_x, mouse_y) esta dentro
	de los limites de la ventana (0 a win_width, 0 a win_height). Nos sirve
	una vex que el mouse este activado comprobemos que esta dentro de la ventana.
	Ya que podemos activarlo estando con le cuersor fuera de la ventana (multitarea)
	y no quemros que tenga encuenta los movimientos del mouse hasta que entre en la ventana
	Retornos:
	- true: si el mouse esta dentro de la ventana
	- false: si el mouse esta fuera de la ventana
*/
bool	is_mouse_in_window(t_mlx *mlx, int mouse_x, int mouse_y)
{
	if (mouse_x >= 0 && mouse_x < mlx->win_width &&
		mouse_y >= 0 && mouse_y < mlx->win_height)
		return (true);
	return (false);
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


/*
	Funcion que resetea la posicion del mouse a la posicion
	de referencia (axis_x, axis_y) dependiendo de en que ejes
	se haya movido el mouse (is_move). La posicion de referencia 
	es la posicion a la que vuelve en mouse en cada iteracion y la
	posicion a la que se mueve al activar el mouse. Ademas nor permite
	que el usuario pueda mover la muñeca en un rango pequeño, si se actulizase
	siempre a la ultima posicion se haga el moviento o no del mouse el cursor
	el usuaraio se quedaria sin escritoio para mover el mouse y ademas se saldria
	de la ventana facilmente.
	Tiene 4 casos:
	- No se ha movido en ningun eje: no hace nada (evitamos llamadas innecesarias a la API
	  de minilibx ya que esto tiene un coste grande en el rendimiento del motor grafico)
	- Se ha movido en ambos ejes: resetea en ambos ejes
	- Se ha movido solo en el eje x: resetea solo en el eje x y el eje y
	  queda en la posicion actual para evitar que un pequelo moviento
	  del mouse en le eje y en el siguiente frame se tenga en cuenta
	  con esto mantenemos la logica de la deadzone
	- Se ha movido solo en el eje y: resetea solo en el eje y y el eje x
	  queda en la posicion actual para evitar que un pequelo moviento
	  del mouse en le eje x en el siguiente frame se tenga en cuenta
	  con esto mantenemos la logica de la deadzone
	Ademas si da la casualidad de que se han cancelado mucho movientos en un eje
	y se han ido sumando a la posicon del mouse hasta salirse de la ventana, se fuerza
	la posicion del mouse a la posicion de referencia en ambos ejes.
	Con esta funccion conseguimos que por mucho el usario mueva la muñeca una vez que el 
	mouse este en la venta y este activo nunca se va a salir a no ser que lo desactivemos
*/
void	reset_mouse_position(t_mlx *mlx, bool *is_move)
{
	t_mouse	*m;

	m = &(mlx->player->mouse);
	if (is_move[X] == 0 && is_move[Y] == 0)
		return ;
	else if (is_move[X] == 1 && is_move[Y] == 1)
		mlx_mouse_move(mlx->mlx_var, mlx->mlx_window, m->axis_x, m->axis_y);
	else if (is_move[X] == 1 && is_move[Y] == 0)
		mlx_mouse_move(mlx->mlx_var, mlx->mlx_window, m->axis_x, m->pos_y);
	else if (is_move[X] == 0 && is_move[Y] == 1)
		mlx_mouse_move(mlx->mlx_var, mlx->mlx_window, m->pos_x, m->axis_y);
	if (is_mouse_in_window(mlx, m->axis_x, m->axis_y) == false)
		mlx_mouse_move(mlx->mlx_var, mlx->mlx_window, m->axis_x, m->axis_y);

}