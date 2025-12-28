/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/22 22:31:15 by alejandro         #+#    #+#             */
/*   Updated: 2025/12/28 12:03:09 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"
void	rotate_player(t_mlx *mlx);

int	close_handler(t_mlx *mlx)
{
	mlx_destroy_image(mlx->mlx_var,
		mlx->mlx_img);
	mlx_destroy_window(mlx->mlx_var,
		mlx->mlx_window);
	mlx_destroy_display(mlx->mlx_var);
	free(mlx->mlx_var);
	exit(0);
}

int	key_press(int keysym, t_mlx *mlx)
{
	if (keysym == XK_Escape)
	{
		close_handler(mlx);
		return (0);
	}
	if (keysym == XK_w)
		mlx->player->move_up = true;
	if (keysym == XK_s)
		mlx->player->move_down = true;
	if (keysym == XK_a)
		mlx->player->move_left = true;
	if (keysym == XK_d)
		mlx->player->move_right = true;
	if (keysym == XK_Left)
		mlx->player->r_counterclockwise = true;
	if (keysym == XK_Right)
		mlx->player->r_clockwise = true;
	return (0);
}

int	key_release(int keysym, t_mlx *mlx)
{
	if (keysym == XK_w)
		mlx->player->move_up = false;
	if (keysym == XK_s)
		mlx->player->move_down = false;
	if (keysym == XK_a)
		mlx->player->move_left = false;
	if (keysym == XK_d)
		mlx->player->move_right = false;
	if (keysym == XK_Left)
		mlx->player->r_counterclockwise = false;
	if (keysym == XK_Right)
		mlx->player->r_clockwise = false;
	return (0);
}


// Función uxiliar para verificar colisiones en un área alrededor del jugador
//cada un de las comporbaciones de WALL es para cada uno de las 4 direcciones de 
//moviemintos
int is_collision(float x, float y, t_mlx *mlx, float epsilon)
{
	return (
		mlx->map->map_grids[(int)(y + epsilon)][(int)(x + epsilon)] == WALL ||
		mlx->map->map_grids[(int)(y - epsilon)][(int)(x + epsilon)] == WALL ||
		mlx->map->map_grids[(int)(y + epsilon)][(int)(x - epsilon)] == WALL ||
		mlx->map->map_grids[(int)(y - epsilon)][(int)(x - epsilon)] == WALL
	);
}


//caso de moverse en x
//caso de moverse en y
//caso de dar con pared hacia donde se mueve o se queda quieto
//ajustar el moviemnto para qu sea poco a poco
void	move_player(t_mlx *mlx)
{
	double speed;
	float angle_rad;
	float new_pos_x;
	float new_pos_y;
	float epsilon; // Margen para evitar que el jugador entre en las celdas de tipo WALL

	rotate_player(mlx);
	speed = mlx->player->speed;
	angle_rad = mlx->player->angle * (PI / 180);
	epsilon = EPSILON;
	if (mlx->player->move_up == true)
	{
		new_pos_x = mlx->player->pos_x + (speed * cos(angle_rad));
		new_pos_y = mlx->player->pos_y - (speed * sin(angle_rad));//ajuste para las coordenadas invertidas del sistema grafico
		if (!is_collision(new_pos_x, new_pos_y, mlx, epsilon))
		{
			mlx->player->pos_x = new_pos_x;
			mlx->player->pos_y = new_pos_y;
		}
	}
	if (mlx->player->move_down == true)
	{
		new_pos_x = mlx->player->pos_x - (speed * cos(angle_rad));
		new_pos_y = mlx->player->pos_y + (speed * sin(angle_rad));
		if (!is_collision(new_pos_x, new_pos_y, mlx, epsilon))
		{
			mlx->player->pos_x = new_pos_x;
			mlx->player->pos_y = new_pos_y;
		}
	}
	if (mlx->player->move_left == true)
	{
		new_pos_x = mlx->player->pos_x - (speed * sin(angle_rad));
		new_pos_y = mlx->player->pos_y - (speed * cos(angle_rad));
		if (!is_collision(new_pos_x, new_pos_y, mlx, epsilon))
		{
			mlx->player->pos_x = new_pos_x;
			mlx->player->pos_y = new_pos_y;
		}
	}
	if (mlx->player->move_right == true)
	{
		new_pos_x = mlx->player->pos_x + (speed * sin(angle_rad));
		new_pos_y = mlx->player->pos_y + (speed * cos(angle_rad));
		if (!is_collision(new_pos_x, new_pos_y, mlx, epsilon))
		{
			mlx->player->pos_x = new_pos_x;
			mlx->player->pos_y = new_pos_y;
		}
	}
}


//rotar en sentido del reloj
//rotar en sentido antihoraio
//controlar los grados y los radianes
//controlar los maximos y minimos 
void	rotate_player(t_mlx *mlx)
{
	//el clockwise depende de donde este el norte y el sur del mapa segun si rotamos el ejeo no lo rotamos
	if (mlx->player->r_clockwise == true)
		mlx->player->angle -= 1.95f;//5 grados es mucho 
	else if (mlx->player->r_counterclockwise == true)
		mlx->player->angle += 1.95f;//5 grados es mucho
	//el ajuste de lso grados se hace despues de hacer el cambio de grados
	if (mlx->player->angle >= 360.0f)
		mlx->player->angle -= 360.0f;
	else if (mlx->player->angle < 0.0f)
		mlx->player->angle += 360.0f;
	return ;
}

//controlar que pasa cunado hay colision para que no se quede atascasdo o se pmueva hacia eleje de la colision
//Documentar porque los angulos cambian y como cambia 
//Ducomentar