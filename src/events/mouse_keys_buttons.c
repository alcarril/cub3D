/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_keys_buttons.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 22:59:20 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/13 15:33:26 by alejandro        ###   ########.fr       */
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




