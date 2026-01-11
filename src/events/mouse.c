/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/11 01:48:42 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/11 13:56:54 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

//Es es la que va en el hook
//el ajuste de grados se hace en el render del frame dentro de move (rotate)
//no hace falta hacerlo aqui seria redundante
//lso datos del mouse pueden ir dentro de una estrcutura para no declrar
//tanta meria local
int	mouse_move_manager(int x, int y, t_mlx *mlx)
{
	t_mouse	*mouse;
	int		pix_dif;
	
	if (is_mouse_in_window(mlx, x, y) == false)
		return (0);
	mouse = &(mlx->player->mouse);
	mouse->pos_x = x;
	pix_dif = x - mouse->axis_x;
	spin_by_pixels(mlx->player, pix_dif);
	mlx_mouse_move(mlx->mlx_var, mlx->mlx_window, mouse->axis_x, mouse->axis_y);
	return (0);
}


//el ajuste de grados se hace en el render del frame dentro de move (rotate)
//no hace falta hacerlo aqui seria redundante
//las variables del mouse pueden ir dentro de una estrcuura para no declara
//tanta memeria local
void	get_mouse_pos_and_rotate(t_mlx *mlx)
{
	t_mouse	*mouse;
	int		pix_dif;
	
	mouse = &(mlx->player->mouse);
	mlx_mouse_get_pos(mlx->mlx_var, mlx->mlx_window, &(mouse->pos_x), &(mouse->pos_y));
	if (is_mouse_in_window(mlx, mouse->pos_x, mouse->pos_y) == false)
		return ;
	pix_dif =  mouse->pos_x - mouse->axis_x;
	if (pix_dif > -MOUSE_DEADZONE && pix_dif < MOUSE_DEADZONE)
		return ;
	if (pix_dif > MOUSE_MAX_MOV)
		pix_dif = MOUSE_MAX_MOV;
	if (pix_dif < -MOUSE_MAX_MOV)
		pix_dif = -MOUSE_MAX_MOV;
	mlx->player->angle -= pix_dif * MOUSE_SENS;
	mlx->player->rad_angle = mlx->player->angle * (PI / 180.0f);
	// spin_by_pixels(mlx->player, pix_dif);
	mlx_mouse_move(mlx->mlx_var, mlx->mlx_window, mouse->axis_x, mouse->axis_y);
}

//se pude sustituir por enter notify para no comporbar en cada evento
//o en cada frame de la renderizacion, aunque por control de limites
//esta bien que este pos si mlx_mouse_move se bugease
//Se usa para que cunado empieza el programa si no tenemos el mouse
//en la ventana sale de la funcion y no cambia la posisoin porque
//sino tendria en cuaenta los cambios de posicion del mouse cunado no esta en
//la ventana
bool	is_mouse_in_window(t_mlx *mlx, int mouse_x, int mouse_y)
{
	if (mouse_x >= 0 && mouse_x < mlx->win_width &&
		mouse_y >= 0 && mouse_y < mlx->win_height)
		return (true); // El mouse está dentro de la ventana
	return (false); // El mouse está fuera de la ventana
}

void	spin_by_pixels(t_player *player, int pix_dif)
{
	if (pix_dif > -MOUSE_DEADZONE && pix_dif < MOUSE_DEADZONE)
		return ;
	if (pix_dif > MOUSE_MAX_MOV)
		pix_dif = MOUSE_MAX_MOV;
	if (pix_dif < -MOUSE_MAX_MOV)
		pix_dif = -MOUSE_MAX_MOV;
	player->angle -= pix_dif * MOUSE_SENS;
	player->rad_angle = player->angle * (PI / 180.0f);
}