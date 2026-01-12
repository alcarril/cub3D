/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_keys_buttons.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 22:59:20 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/12 21:28:16 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Funcion para inicializar el mouse cuando mlx_hookk detecte que ha entrado
	en la ventana por primera vez. Usamos la estatica para que solo se ejecute una vez
	Si se desconecta el mouse y volvemos a entrar por la ventana tendremos que conectarlo
	nuevamente
	Esconde el cursor y activa el mouse en on
*/
int	mouse_init_manager(t_mlx *mlx)
{
	static bool	initialized;
	if (initialized == true)
		return (0);
	initialized = true;
	mlx_mouse_hide(mlx->mlx_var, mlx->mlx_window);
	mlx->player->mouse.onoff = ON;
	return (0);
}

/*
	Funcion para activar o desactivar el mouse
	- Cuando el mouse esta activado se esconde el cursor y lo desactiva
	- Cuando el mouse esta desactivado se muestra el cursor y lo activa
*/
void	toogle_mouse(t_mlx *mlx)
{
	if (mlx->player->mouse.onoff == ON)
	{
		mlx->player->mouse.onoff = OFF;
		mlx_mouse_show(mlx->mlx_var, mlx->mlx_window);
		printf("MOUSE OFF\n");
	}
	else
	{
		mlx->player->mouse.onoff = ON;
		mlx_mouse_hide(mlx->mlx_var, mlx->mlx_window);
		printf("MOUSE ON\n");
	}
}

/*
	Funcion que maneja los botones del mouse para ajustar la sensibilidad
	del mouse en el eje x (rotacion) y el factor de pitch (eje y). Tanto la 
	sensibilidad como el factor de pitch tienen limites maximos y minimos
*/
int	mouse_button_manager(int mouse_button, int x, int y, t_mlx *mlx)
{
	(void)x;
	(void)y;
	if (mouse_button == 4)
	{
		mlx->player->mouse.sens_x += 0.02f;
		mlx->player->mouse.pitch_factor += 0.002f;
		printf("MOUSE: Sensibility increasing\n");
	}
	if (mouse_button == 5)
	{
		mlx->player->mouse.sens_x -= 0.02f;
		mlx->player->mouse.pitch_factor -= 0.002f;
		printf("MOUSE: Sensibility decreasing\n");
	}
	if (mlx->player->mouse.sens_x > MOUSE_MAX_SENSX)
		mlx->player->mouse.sens_x = MOUSE_MAX_SENSX;
	if (mlx->player->mouse.pitch_factor > MAX_MOUSE_PITCH_FACTOR)
		mlx->player->mouse.pitch_factor = MAX_MOUSE_PITCH_FACTOR;
	if (mlx->player->mouse.sens_x < MOUSE_MIN_SENSX)
		mlx->player->mouse.sens_x = MOUSE_MIN_SENSX;
	if (mlx->player->mouse.pitch_factor < MIN_MOUSE_PITCH_FACTOR)
		mlx->player->mouse.pitch_factor = MIN_MOUSE_PITCH_FACTOR;
	return (0);
}


//Es es la que va en el hook
//el ajuste de grados se hace en el render del frame dentro de move (rotate)
//no hace falta hacerlo aqui seria redundante
//lso datos del mouse pueden ir dentro de una estrcutura para no declrar
//tanta meria local
int	mouse_move_manager(int x, int y, t_mlx *mlx)
{
	t_mouse	*m;
	int		pix_dif[2];
	bool	is_move[2];
	
	m = &(mlx->player->mouse);
	m->pos_x = x;
	m->pos_y = y;
	if (is_mouse_in_window(mlx, m->pos_x, m->pos_y) == false)
		return (0);
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
	return (reset_mouse_position(mlx, is_move), 0);
}



