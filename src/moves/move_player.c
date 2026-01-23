/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move_player.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/06 12:30:42 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 08:57:41 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Con esta funcion rotamos la direccion hacia la que mira el jugador. 
	Dado que los ejes estan invertidos en la ventana tenemos que hacer un ajust
	para que cuando rote en sentido horario el angulo disminuya y cuando rote e
	sentido antiohorario sea alreves. Ademas se ajustan los limites de los 
	angulos para que nunca sean mayores de 360 grados o menos aunque a la hora 
	de hacer los calculos de los senos y los cosenos esto no sea relevante 
	porque no acambian pero evitamos que el float llegue a los limites y 
	se haga overflow.
	OPtimiacion de procesador:
	- Se evita el uso de la función fmod para limitar el ángulo entre 0 y 360
	  grados, ya que fmod implica una operación de división que es costosa en 
	  términos de ciclos de CPU. En su lugar, se utilizan simples restas y suma
	  condicionales.
	- Se podria cambiar ela oprrcion de onversion a radianes por una 
	  multiplicacion por una constante precalculada.
	- Se hace aqui la operacion para no tener que usar los radianes y hacer el 
	  calculo en el resto de funcione de engine.
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
	Funcion para controlar el pitch de la camara del jugador en el eje Y
	segun las teclas de mirar arriba y abajo.
	- Que es el pitch:
		El pich se mide en pixeles para facilitar el calculo de la
		posicion vertical de la camara en la pantalla. Esta variable la usa-
		remos luego en el renderizado para hacer una traslacion de los puntos
		de la pantalla (pixeles de la matriz de pixeles) hacia arriba o 
		hacia abajo simulando el efecto de mirar hacia en el eje Y.
		Esta traslacion se puede hacer trasladoando los punto o moviendo
		la linea del horizonte.
	- POrque se usa el pitch en pixeles:
		Como el jugador realmente esta en un mapa 2D y no tiene una posicion
		en el eje Y (altura) real, no podemos usar una posicion en el eje Y
		para calcular el pitch. En su lugar usamos una variable en pixeles
	-Como se calcula el pitch:
		Se calcula multiplicando un pitch_factor (porcentaje pixeles de 
		desplazamiento) por el total de pixeles que se puede llegar a mover
		la camara en el eje Y (max_pitch_pix). Normalemte este maximo sera
		una fraccion de la altura total de la ventana (por ejemplo la mitad
		de la altura de la ventana, (win_height / 2)
	Mejoras de rendimiento:
	 - Se utiliza una variable estática para almacenar el valor de 
	   max_pitch_pix, evitando así la necesidad de acceder a la estructura del 
	   jugador en cada llamada a la función. Esto reduce el número de accesos a
	   memoria y facilita que el procesador pueda colocar este valor en un
	   regsitro y evita cache misses. Como esta fucnion se llama continuamente
	   en el hoot loop, esta optimizacion puede tener un impacto en el
	   rendimiento general.
*/
void	axis_y_pitch(t_player *player)
{
	t_player	*pl;
	static int	max_pitch;

	if (max_pitch == 0)
		max_pitch = player->max_pitch_pix;
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
	Función para calcular las componentes del vector velocidad del jugador 
	según la configuración de físicas del motor gráfico esté activada o no o 
	en qué modo lo esté.

	Modos de funcionamiento:
	- Modo normal: Se calcula el vector de velocidad según una velocidad fija 
	  y un delta time fijo (estilo retro para videojuegos clásicos). Si la 
	  tasa de fps varía, la velocidad varía, no es portable a distintos
	  sistemas porque el moviento del jugador deja de ser determinista.
	  a nivel de jugabilidad es menos preciso y no permite incluir inercias.
	- Modo físicas: Se calcula el vector de velocidad según una velocidad 
	  diferencial y un delta time. Esto incluye aceleración e inercias y 
	  permite que el juego tenga un movimiento determinista independientemente 
	  de la tasa de fps (estilo motores modernos). Además, permite incluir 
	  inercias, tipos de suelo, distintas aceleraciones, etc.
	- Modo doom/quake: Se calcula el vector de velocidad según una velocidad 
	  diferencial en cada uno de los ejes y un delta time. Esto permite 
	  simular todo lo anterior y, además, permite incluir aceleraciones 
	  diferenciales que puedan verse afectadas de manera realista por efectos 
	  externos. También solucionamos el strafe.
	En modos de fisicas ademas se puede mover al jugador dentro del aire y
	no solo en el suelo. (jumping y flying)
	Limitacion de delta time:
	- Se limita el delta time máximo a un valor predefinido (MAX_DT_FRAME) 
	  para evitar saltos bruscos en la física del jugador en caso de caídas 
	  significativas en la tasa de fps. Esto asegura que el movimiento del 
	  jugador siga siendo suave y controlable, incluso en situaciones de 
	  rendimiento variable.
	Variables:
		- pl: puntero a la estructura del jugador.
		- delta_time: tiempo transcurrido desde la ultima actualizacion en
		  milisegundos.
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
	Función auxiliar para verificar colisiones en un área alrededor del jugador
	Se pasa la poscion del jugador en float a int para saber el grid del mapa
	en el que esta, se compurbea que este dentro de los limites del mapa y lueg
	se hace la comprobacion de si es una pared o no.
	Cada una de las comprobaciones de WALL es para cada una de las 4 direccione
	de movimiento porque al combinarse con epsilon para que el jugador no entre 
	dentro de las paredes en el minimapa (epsilon sería el volumen del jugador)
*/
bool	is_collision(float x, float y, t_mlx *mlx, float e)
{
	int	mx;
	int	my;

	mx = (int)x;
	my = (int)y;
	if (mx < 0 || my < 0 || mx >= (int)mlx->map->max_columns
		|| my >= (int)mlx->map->max_rows)
	{
		return (true);
	}
	return (
		is_wall_tile(mlx->map->map_grids[(int)(y + e)][(int)(x + e)]) ||
		is_wall_tile(mlx->map->map_grids[(int)(y - e)][(int)(x + e)]) ||
		is_wall_tile(mlx->map->map_grids[(int)(y + e)][(int)(x - e)]) ||
		is_wall_tile(mlx->map->map_grids[(int)(y - e)][(int)(x - e)])
	);
}

/*
	Con esta funcion actualizamos la posiscion del juahador dentro del plano
	segun el vector de movimitento en el que se mueva.
	
	Pasos:
	1. Comprobamso hacia que direccion esta mirando el jugador dentro del
	   mapa 2D y rotamos su angulo segun las teclas de rotacion.
	2. Calculamos el pitch de la camara del jugador en el eje Y segun las
	   teclas de mirar arriba y abajo.
	3. Calculamos el vector de velocidad del jugador segun el modo de
	   fisicas activado o no.
	4. Calculamos la nueva posicion del jugador sumando su vector de
	   moviento a su posicion actual.
	5. Comprobamos si hay colision en cada uno de los ejes por separado.
	6. Si no hay colision en ningun eje, actualizamos la posicion del
	   jugador a la nueva posicion.
	7. Si hay colision en alguno de los ejes, actualizamos la posicion
	   del jugador solo en el eje que no tiene colision y aplicamos un
	   coeficiente de friccion en el eje libre para simular la fuerza
	   de rozamiento.
	   
	Calculo de colisiones: Antes de moverlo calculamos la nueva posicion 
	en cada eje (movimeinto descompuesto) asi podemos calcular si hay colision
	cada uno de los vectores que componen el moviento en la direccion hacia
	donde se mueve y en ela caso de encontrar una parade que no se quede 
	bloqueado. Adenas metemos un coeficionete de friccion que disminuye la 
	velocidad de moviento en el eje que quede libre para simular la fuerzxa 
	de rozamiento que diminuiriza la velocidad.
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
