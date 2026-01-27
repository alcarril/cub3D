/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 22:31:15 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/22 21:58:04 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

/*
	Funcion que maneja lso evetos de teclado de X11  de KeyPress. Cunado
	el hook de x11 detecta una presion de tecla, las transforma en un evecto
	 y esta funcion (handler) es triggered.
	- Si entra en alguna de las teclas retorna 0
	- Si no imprieme los controles y las configuraciones del juego
	Funciones llamadas:
	- close_game_manager: Cierra el juego y libera la memoria cuando se
	 la tecla ESC es presionada.
	- player_keypress: Maneja las teclas relacionadas con el movimiento
	 del jugador. Retorna true si la tecla fue manejada.
	- graphics_engine_keypress: Maneja las teclas relacionadas con
	 los ajustes del motor grafico. Retorna true si la tecla fue manejada.
	- phisics_engine_keypress: Maneja las teclas relacionadas con
	 los ajustes del motor fisico. Retorna true si la tecla fue manejada.
	- toggle_minimap: Activa o desactiva el minimapa cuando se presiona
	 la tecla 'm'.
	- minimap_zoom: Aumenta o disminuye el zoom del minimapa cuando
	 se presionan las teclas '+' o '-'.
	- toggle_rays: Activa o desactiva la visualizacion de rayos en
	 el minimapa cuando se presiona la tecla 'r'.
	- print_controls: Imprime los controles del juego en la consola
*/
int	key_press(int keysym, t_mlx *mlx)
{
	if (keysym == XK_Escape)
		return (close_game_manager(mlx), 0);
	if (player_keypress(mlx, keysym))
		return (0);
	if (graphics_engine_keypress(mlx, keysym))
		return (0);
	if (phisics_engine_keypress(mlx, keysym))
		return (0);
	else if (keysym == XK_m)
		toggle_minimap(mlx);
	else if (keysym == XK_plus)
		minimap_zoom(mlx, true);
	else if (keysym == XK_minus)
		minimap_zoom(mlx, false);
	else if (keysym == XK_r)
		toggle_rays(mlx);
	else if (keysym == XK_z)
		print_controls();
	else
		printf(CONTROLS_INFO);
	return (0);
}

/*
	Funcion que maneja los eventos de teclado de X11 de KeyRelease. Cunado
	el hook de x11 detecta una liberacion de tecla, las transforma en un evecto
	 y esta funcion (handler) es triggered.
	- Si entra en alguna de las teclas retorna 0
	- Si no retorna 0
	Teclas gestionadas y funciones llamadas:
	- Teclas de movimiento (W, A, S, D, Flechas): Actualizan el estado de las
	 teclas en la estructura del jugador para detener el movimiento o
	 rotacion correspondiente.
	- Shift Izquierdo: Llama a player_sprint_keyrelease para detener
	 el sprint del jugador.
	- Barra espaciadora: Llama a player_space_keyrelease para detener
	 el salto del jugador.
*/
int	key_release(int keysym, t_mlx *mlx)
{
	if (mlx == NULL)
		return (0);
	if (keysym == XK_w)
		mlx->player->keys.move_up = false;
	if (keysym == XK_s)
		mlx->player->keys.move_down = false;
	if (keysym == XK_a)
		mlx->player->keys.move_left = false;
	if (keysym == XK_d)
		mlx->player->keys.move_right = false;
	if (keysym == XK_Left)
		mlx->player->keys.r_counterclockwise = false;
	if (keysym == XK_Right)
		mlx->player->keys.r_clockwise = false;
	if (keysym == XK_Up)
		mlx->player->keys.look_up = false;
	if (keysym == XK_Down)
		mlx->player->keys.look_down = false;
	if (keysym == XK_Shift_L && mlx->player->keys.sprint == true)
		player_sprint_keyrelease(mlx);
	if (keysym == XK_space && mlx->player->is_jumping == true)
		player_space_keyrelease(mlx);
	return (0);
}

/*
	Funcion que maneja las teclas relacionadas con
	los ajustes del motor grafico.
	- Si entra en alguna de las teclas retorna true
	- Si no retorna false
	Teclas gestionadas y funciones llamadas:
	- O: Llama a toogle_raycasting para activar o desactivar el raycasting.
	- F: Llama a toggle_fish_eye para activar o desactivar el efecto fish eye.
	- E: Llama a toogle_dist_calc para cambiar el metodo de calculo de 
	  distancia.
	- T: Llama a toggle_textures para activar o desactivar las texturas.
	- C: Llama a toogle_floor_celling para cambiar el metodo de renderizado
	  de piso y techo.
	- U: Llama a toogle_ambiance para activar o desactivar la ambiancia.
	- 1, 2, 3, 4: Llama a ambiance_keypress para seleccionar diferentes 
	  ambientes.
*/
bool	graphics_engine_keypress(t_mlx *mlx, int keysym)
{
	if (keysym == XK_o)
		toogle_raycasting(mlx);
	else if (keysym == XK_f)
		toggle_fish_eye(mlx);
	else if (keysym == XK_e)
		toogle_dist_calc(mlx);
	else if (keysym == XK_t)
		toggle_textures(mlx);
	else if (keysym == XK_c)
		toogle_floor_celling(mlx);
	else if (keysym == XK_u)
		toogle_ambiance(mlx);
	else if (ambiance_keypress(mlx, keysym))
		return (true);
	else
		return (false);
	return (true);
}

/*
	Funcion que imprime los controles y configuraciones del motor grafico
	en la consola (stdout).
	- Organiza los controles en secciones para facilitar la lectura.
	- Llama a print_controls2 para imprimir controles adicionales.
*/
void	print_controls(void)
{
	printf("Controls and configs:\n");
	printf("\n[General]\n");
	printf("ESC: Exit Game\n");
	printf("Z: Print Controls\n");
	printf("\n[Graphics Engine Settings]\n");
	printf("F: Toggle Fish Eye Effect\n");
	printf("O: Toggle Raycasting On/Off\n");
	printf("E: Toggle Distance Calculation (Euclidean/Perpendicular)\n");
	printf("T: Toggle Textures On/Off\n");
	printf("C: Toggle Floor and Ceiling Rendering Method\n");
	printf("\n[Screen Display Settings]\n");
	printf("M: Toggle Minimap\n");
	printf("R: Toggle Rays on Minimap\n");
	printf("+/-: Minimap Zoom In/Out\n");
	printf("\n[Player Movement]\n");
	printf("WASD: Move Up, Down, Left, Right\n");
	printf("Left Arrow: Rotate Counterclockwise\n");
	printf("Right Arrow: Rotate Clockwise\n");
	printf("Up Arrow: Look Up\n");
	printf("Down Arrow: Look Down\n");
	printf("\n[Player Movement EXTRA]\n");
	printf("Up Arrow: Look Up\n");
	printf("Down Arrow: Look Down\n");
	printf("Left Shift: Sprint\n");
	print_controls2();
}

void	print_controls2(void)
{
	printf("Space: Jump\n");
	printf("Left Control: Crouch\n");
	printf("Caps Lock: Ground Pound\n");
	printf("Q: Toggle Fly Mode\n");
	printf("Up Arrow: Look Up\n");
	printf("Down Arrow: Look Down\n");
	printf("\n[Mouse Settings]\n");
	printf("J: Toggle Mouse On/Off\n");
	printf("Mouse Scroll Up: Increase Mouse Sensitivity\n");
	printf("Mouse Scroll Down: Decrease Mouse Sensitivity\n");
	printf("\n[Physics Settings]\n");
	printf("P: Toggle Physics On/Off\n");
	printf("K: Toggle DukeDoom Physics Mode\n");
	printf("6: Select Earth Gravity Mode\n");
	printf("7: Select Moon Gravity Mode\n");
	printf("8: Select Hulkpiter Gravity Mode\n");
	printf("9: Select Spectro Gravity Mode\n");
	printf("0: Select Jetpack Gravity Mode\n");
	printf("\n[Ambiance Settings]\n");
	printf("U: Toggle Ambiance On/Off\n");
	printf("1: Select Asturias Ambiance\n");
	printf("2: Select Cemetery Ambiance\n");
	printf("3: Select Open Ambiance\n");
	printf("4: Select Matrix Ambiance\n");
}
