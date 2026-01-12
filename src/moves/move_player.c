/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 12:30:42 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/12 23:08:12 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Calculo del movimiento del jugador en cada eje del plano. Descomponemos el 
	moviento y lo bufferizamos dentro del array diferencial. Para ellos usamos 
	los principios de trigonometria de la circunferencia gonimetrica, vectorizacion y el
	diferencial de un punto en un espacio euclideo. Segun el angulo de rotacion 
	del jugador respeto a los ejes del plano y la direcion relativa en la que queramos
	movernos el difrecial se calcula de un maera u otra
	- Cosas a tener en cuenta:
		- En la ventana el NORTE (90 grados) y el SUR (270) estan invertidos.
		- Hay un vectorixacion diferente para cada uno de los movientos dentro
		de la circunferencia (cada uno de los cuadrantes del plano)
		- En ver de sumar 90 grados para los movimientos laterales se podria usar
		la relacion de signos de la matriz de rotacion
		
*/
void	vectorization(t_player *player)
{
	float	angle_rad;
	
	bzero(player->differencial, sizeof(player->differencial));
	angle_rad = player->rad_angle;
	if (player->keys.move_up)
	{
		player->differencial[X] += cos(angle_rad) * player->speed;
		player->differencial[Y] -= sin(angle_rad) * player->speed;
	}
	if (player->keys.move_down)
	{
		player->differencial[X] -= cos(angle_rad) * player->speed;
		player->differencial[Y] += sin(angle_rad) * player->speed;
	}
	if (player->keys.move_left)
	{
		player->differencial[X] += cos(angle_rad + (PI / 2)) * player->speed;
		player->differencial[Y] += sin(angle_rad - (PI / 2)) * player->speed;
	}
	if (player->keys.move_right)
	{
		player->differencial[X] += cos(angle_rad - (PI / 2)) * player->speed;
		player->differencial[Y] += sin(angle_rad + (PI / 2)) * player->speed;
	}
}

/*
	Con esta funcion rotamos la direccion hacia la que mira el jugador. Dado que los
	ejes estan invertidos en la ventana tenemos que hacer un ajuste para que cuando
	rote en sentido horario el angulo disminuya y cuando rote en sentido antiohorario
	sea alreves. Ademas se ajustan los limites de los angulos para que nunca sean mayores
	de 360 grados o menos aunque a la hora de hacer los calculos de los senos y los cosenos
	esto no sea relevante porque no acambian pero evitamos que el float llegue a los limites y 
	se haga overflow.
*/
void	rotate_player(t_player *player, float delta_grades)
{
	if (player->keys.r_clockwise == true)
		player->angle -= delta_grades;
	else if (player->keys.r_counterclockwise == true)
		player->angle += delta_grades;
	if (player->angle >= 360.0f)
		player->angle -= 360.0f;
	else if (player->angle < 0.0f)
		player->angle += 360.0f;
	player->rad_angle = player->angle * (PI / 180.0f);
}

/*
	Función uxiliar para verificar colisiones en un área alrededor del jugador
	cada un de las comporbaciones de WALL es para cada uno de las 4 direcciones de 
	moviemintos y se combina con epsilon para que el jugador no entre dentro de las 
	paredes en el minimapa (epsilon seria el volumen del jugador)
*/
bool	is_collision(float x, float y, t_mlx *mlx, float e)
{
	int mx = (int)x;
	int my = (int)y;

	if (mx < 0 || my < 0 || mx >= (int)mlx->map->max_columns || my >= (int)mlx->map->max_rows)
		return (true);
	return (
		mlx->map->map_grids[(int)(y + e)][(int)(x + e)] == WALL ||
		mlx->map->map_grids[(int)(y - e)][(int)(x + e)] == WALL ||
		mlx->map->map_grids[(int)(y + e)][(int)(x - e)] == WALL ||
		mlx->map->map_grids[(int)(y - e)][(int)(x - e)] == WALL
	);
}

/*
	Con esta funcion actualizamos la posiscion del juahador dentro del plano
	segun el vector de movimitento en el que se mueva. Antes de moverlo calculamos
	la nueva posicion en cada eje (movimeinto descompuesto) asi podemos calcular si hay
	colision en cada uno de los vectores que componen el moviento en la direccion hacia
	donde se mueve y en ela caso de encontrar una parade que no se quede bloqueado. Adenas
	metemos un coeficionete de friccion que disminuye la velocidad de moviento en el eje que
	quede libre para simular la fuerzxa de rozamiento que diminuiriza la velocidad.
*/
void	move_player(t_mlx *mlx)
{
	t_player	*player;
	float		new_pos[2];
	bool		colision[2];
	
	player = mlx->player;
	rotate_player(player, 1.1f);
	axis_y_pitch(player);
	vectorization(player);
	new_pos[X] = player->pos_x + player->differencial[X];
	new_pos[Y] = player->pos_y + player->differencial[Y];
	colision[X] = is_collision(new_pos[X], player->pos_y, mlx, player->volume);
	colision[Y] = is_collision(player->pos_x, new_pos[Y], mlx, player->volume);
	if (!colision[X] && !colision[Y])
	{
		player->pos_x = new_pos[X];
		player->pos_y = new_pos[Y];
	}
	else
	{
		if (!colision[X])
			player->pos_x += player->differencial[X] * FRICTION;
		if (!colision[Y])
			player->pos_y += player->differencial[Y] * FRICTION;
	}
}

void	axis_y_pitch(t_player *player)
{
	t_player	*pl;
	
	pl = player;
	if (pl->keys.look_up == true)
		pl->pitch_pix += pl->pitch_factor * pl->max_pitch_pix;
	if (pl->keys.look_down == true)
		pl->pitch_pix -= pl->pitch_factor * pl->max_pitch_pix;
	if (pl->pitch_pix > pl->max_pitch_pix)
		pl->pitch_pix = pl->max_pitch_pix;
	if (pl->pitch_pix < -pl->max_pitch_pix)
		pl->pitch_pix = -pl->max_pitch_pix;
	return ;
}