/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 20:28:56 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 20:40:20 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función que resetea la posición del mouse a la posición de referencia 
	(axis_x, axis_y) dependiendo de en qué ejes se haya movido el mouse 
	(is_move). La posición de referencia es la posición a la que vuelve el 
	mouse en cada iteración y la posición a la que se mueve al activar el mouse. 
	Además, nos permite que el usuario pueda mover la muñeca en un rango 
	pequeño. Si se actualizase siempre a la última posición, se haga el 
	movimiento o no del mouse, el cursor se quedaría sin escritorio para mover 
	el mouse y, además, se saldría de la ventana fácilmente.

	Tiene 4 casos:
	- **No se ha movido en ningún eje:** No hace nada (evitamos llamadas 
	  innecesarias a la API de minilibx, ya que esto tiene un coste grande en 
	  el rendimiento del motor gráfico).
	- **Se ha movido en ambos ejes:** Resetea en ambos ejes.
	- **Se ha movido solo en el eje x:** Resetea solo en el eje x y el eje y 
	  queda en la posición actual para evitar que un pequeño movimiento del 
	  mouse en el eje y en el siguiente frame se tenga en cuenta. Con esto 
	  mantenemos la lógica de la deadzone.
	- **Se ha movido solo en el eje y:** Resetea solo en el eje y y el eje x 
	  queda en la posición actual para evitar que un pequeño movimiento del 
	  mouse en el eje x en el siguiente frame se tenga en cuenta. Con esto 
	  mantenemos la lógica de la deadzone.
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
}
