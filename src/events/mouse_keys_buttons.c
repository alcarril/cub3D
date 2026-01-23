/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_keys_buttons.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 22:59:20 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/22 22:19:23 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Funcion para inicializar el mouse cuando mlx_hookk detecte que ha entrado
	en la ventana por primera vez. 
	- Si el mouse ya ha entrado en la ventana, no hace nada
	- Si es la primera vez que entra, esconde el cursor, activa el mouse
	  y ajusta la variable has_been_mouse_in_window a true para no volver
	  a inicializar el mouse.
	 - Pone la vairbale player->mouse.onoff a ON para indicar que el mouse
	  esta activado. yel bucle principal de renderizado pueda entrar s analizar
	  la posicion del mouse solo cuando sea necesario (mas eficiencia de codigo)
	- El resto de veces que el mouse entre en la ventana cunado se ha 
	  desactivado se saca y se vueleve a activar lo gestiona la funcion 
	  is_mouse_in_windowdentro del bucle de renderizado principal.
	- Avisamos que ya se puede usar el boost mode sin que crashee el kernell 
	  pixmap al activar el mouse correctamente. Porque el boost mode usa un 
	  renderizado rapido y escribe usando alineacion de memeria y sube tanto 
	  la tasasd defps que al kernell no le da tiempo a crear el pixmap del 
	  mouse y crashea.
*/
int	mouse_init_manager(t_mlx *mlx)
{
	if (mlx->has_been_mouse_in_window == true)
		return (0);
	if (mlx->has_been_mouse_in_window == false)
		mlx->has_been_mouse_in_window = true;
	printf("Now you can use boost mode without crash kernell pixmap, ;)\n");
	mlx_mouse_hide(mlx->mlx_var, mlx->mlx_window);
	mlx->player->mouse.onoff = ON;
	return (0);
}

/*
	Función para activar o desactivar el mouse.
	- Cuando el mouse está activado, se esconde el cursor y se desactiva
	  la variable que indica al bicle de renderizado que debe analizar
	  la posición del mouse. (mejora de eficincia)
	- Cuando el mouse está desactivado, se muestra el cursor y se activa.
	- Al activarlo, se evalúa si el mouse está dentro o fuera de la ventana:
	  - Si el mouse está dentro de la ventana, se mueve a la posición de 
		referencia (axis_x, axis_y) y se ajusta la variable out_and_on a false.
	  - Si el mouse está fuera de la ventana, se ajusta la variable out_and_on
		a true, indicando que el mouse no debe ser considerado para mover al 
		jugador hasta que vuelva a entrar en la ventana.
	La variable out_and_on se usa en el bucle principal de renderizado para que
	cunado antivamso el mouse fuera de la ventana no se mueva al jugador hasta
	que vuelva a entrar en la ventana. Sino pega tirones raros.
*/
void	toogle_mouse(t_mlx *ml)
{
	t_mouse	*m;

	m = &(ml->player->mouse);
	if (m->onoff == ON)
	{
		m->onoff = OFF;
		mlx_mouse_show(ml->mlx_var, ml->mlx_window);
		printf("MOUSE OFF\n");
	}
	else
	{
		m->onoff = ON;
		mlx_mouse_get_pos(ml->mlx_var, ml->mlx_window, &(m->pos_x),
			&(m->pos_y));
		if (is_mouse_in_window(ml, m->pos_x, m->pos_y) == IN)
		{
			mlx_mouse_move(ml->mlx_var, ml->mlx_window, m->axis_x, m->axis_y);
			m->out_and_on = false;
		}
		else
			m->out_and_on = true;
		m->out_and_on = true;
		mlx_mouse_hide(ml->mlx_var, ml->mlx_window);
		printf("MOUSE ON\n");
	}
}

/*
	Funcion que maneja los botones del mouse para ajustar la sensibilidad
	del mouse en el eje x (rotacion) y el factor de pitch (eje y). Tanto la 
	sensibilidad como el factor de pitch tienen limites maximos y minimos
	para evitar que el jugador pueda poner valores extremos que hagan
	inutilizable el mouse.
	- Rueda del mouse hacia arriba (boton 4): Aumenta la
	  sensibilidad y el factor de pitch.
	- Rueda del mouse hacia abajo (boton 5): Disminuye la
	  sensibilidad y el factor de pitch.
	- Se informa al usuario del cambio por terminal (stdout).
	- Se aplican los limites maximos y minimos a la sensibilidad
	  y al factor de pitch.
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
