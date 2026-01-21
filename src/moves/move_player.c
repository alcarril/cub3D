/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 12:30:42 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/21 17:44:36 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Con esta funcion rotamos la direccion hacia la que mira el jugador. Dado que los
	ejes estan invertidos en la ventana tenemos que hacer un ajuste para que cuando
	rote en sentido horario el angulo disminuya y cuando rote en sentido antiohorario
	sea alreves. Ademas se ajustan los limites de los angulos para que nunca sean mayores
	de 360 grados o menos aunque a la hora de hacer los calculos de los senos y los cosenos
	esto no sea relevante porque no acambian pero evitamos que el float llegue a los limites y 
	se haga overflow.
	OPtimiacion de procesador:
	- Se evita el uso de la función fmod para limitar el ángulo entre 0 y 360 grados,
	  ya que fmod implica una operación de división que es costosa en términos de
	  ciclos de CPU. En su lugar, se utilizan simples restas y sumas condicionales.
	- Se podria cambiar ela oprrcion de onversion a radianes por una multiplicacion por una constante precalculada
	- Se hace aqui la operacion para no tener que usar los radianes y hacer el calculo en el resto de funciones
	de engine
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
	Falta comnetario
*/
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

/*
	Funcion para calcular la componetes del vector velocidad del jugador segun en el la configuracion
	de fisicas del motor grafico este activada o no o qen que modo lo este. 
	MOdo normal: Se calcula el vector de velocidad segun una velocidad fija y un delta time
	fijo (estilo retro para videojuegos clasicos) si tasa fps varia velocidad varia no portable-
	Modo fisicas: Se calcula el vector de velocidad segun una velocidad diferencial y un delta time
	Esto incluye aceleracion e inercias y permite que el juego tenga un moviento determinista
	independientemente de la tasa de fps. (estilo motores modernos) y permite meter inercias
	tipos de suelo, distintas aceleraciones etc..
	Modo doom/quake: Se calcula el vector de velocidad segun una velocidad diferencial en cada uno de los ejes
	y un delta time. Esto permite simluar todo lo anterior y ademas permite meter aceleraciones diferenciales
	que puedan verse afectadas de manera realista por efectos externos. Tambien solucionamos el strafe
*/
void	jump_speed_vecmove(t_mlx *mlx)
{
	t_player	*pl;
	long long	delta_time;

	pl = mlx->player;
	if (mlx->frame->phisics_onoff == ON)
	{
		delta_time = mlx->del_timestamp;
		if (delta_time > MAX_DT_FRAME)
			delta_time = MAX_DT_FRAME;
		jump(mlx, delta_time);
		if (mlx->frame->dukedoom_mode == OFF)
			difspeed_and_vecmove(mlx, delta_time);
		else
			difvspeed_and_vecmove_nukedoom(mlx, delta_time);
	}
	else
	{
		delta_time = 1;
		vectorization(mlx->player, delta_time, pl->speed, pl->rad_angle);
	}
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
		is_wall_tile(mlx->map->map_grids[(int)(y + e)][(int)(x + e)]) ||
		is_wall_tile(mlx->map->map_grids[(int)(y - e)][(int)(x + e)]) ||
		is_wall_tile(mlx->map->map_grids[(int)(y + e)][(int)(x - e)]) ||
		is_wall_tile(mlx->map->map_grids[(int)(y - e)][(int)(x - e)])
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
	jump_speed_vecmove(mlx);
	new_pos[X] = player->pos_x + player->diff[X];
	new_pos[Y] = player->pos_y + player->diff[Y];
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
			player->pos_x += player->diff[X] * WALL_FRICTION;
		if (!colision[Y])
			player->pos_y += player->diff[Y] * WALL_FRICTION;
	}
}

