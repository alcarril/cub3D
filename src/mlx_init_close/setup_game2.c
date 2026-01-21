/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_game2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:54:24 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/21 18:41:12 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Configuracion por defecto de la ambiancia del motor:
	- Color de la niebla en las paredes
	- Color de la niebla en el suelo y techo
	- Distancia maxima del mapa para el calculo de la niebla (factor de normalizacion)
	- Factor de multiplicacion de la niebla en las paredes
	- Factor de multiplicacion de la niebla en el suelo
	- Factor de multiplicacion de la niebla en el techo
	- Factor k de la niebla en las paredes 
	- Factor k de la niebla en el suelo 
	- Factor k de la niebla en el techo 
	- Factor de multiplicacion del shader en las paredes
	- Factor de multiplicacion del shader en el suelo
	- Factor de multiplicacion del shader en el techo
	- Tipo de ambiancia (abierto, cerrado, nocturno etc..)
*/
void	setup_default_ambiance(t_map *map, t_ambiance *amb)
{
	amb->fog_color_walls = FOG_MEDIO_OSCURO;
	amb->fog_color_fc = FOG_MEDIO_CLARO;
	amb->v_max_distance_map = map->max_distance * 0.9f;
	amb->vinv_max_diatance = 1.0f / amb->v_max_distance_map;
	amb->mult_fog_walls = 0.2f;
	amb->mult_fog_floor = 0.3f;
	amb->mult_fog_ceiling = 0.2f;
	amb->k_factor_walls = 8.0f;
	amb->k_factor_floor = 1.0f;
	amb->k_factor_ceiling = 4.0f;
	amb->mult_shader_walls = 1.0f;
	amb->mult_shader_floor = 0.7f;
	amb->mult_shader_ceiling = 0.5f;
	amb->ambiance = OPEN;
}

/*
	Configuracion por defecto de las fisicas del motor:
	- Gravedad por defecto del motor (fuerza de atraccion del mapa): Aceleracion negativa en z que se usa en los movientos verticales del jugador para calcual la
	veliocidad en z al ser la aceletarion total del jugador en z una resta de la aceleracion de la fuerza de salto del jugador menos
	la gravedad del mapa en funcion del timepo. Se desprecia la masa del jugador para simplificar los calculos.
	- Friccion del aire por defecto del motor (fuerza que hace que el jugador se desacelere cunado esta en el aire dx dy bajando speed):
	Se usa para simular la resistencia del aire al movimiento del jugador en el aire desacelerando su velocidad en x e y en funcion del tiempo.
	- Friccion del suelo por defecto del motor (Fuerza de frenado del jugador): Se usa para simular la resistencia del suelo al movimiento del jugador desacelerando su velocidad en x e y en funcion del tiempo.
*/
void	setup_default_phisics(t_phisics *phisics)
{
	phisics->gravity = GRAVITY_EARTH;
	phisics->air_friction = AIR_FRICTION_MS;
	phisics->floor_friction = FLOOR_FRICTION_MS;
}

/*
	Inicilizamos los datos de la fisica del jugador:
	- Aceleracion en funcion de la friccion del suelo: simepre se usa una
	  friccion de suelo  y un factor de aceleracion en funcion del tipo de moviento
	  del jugador (lateral, frontal, trasero). Relacionar la aceleracion con la friccion
	  co un factor es mas manetnible que poner valores fijos, se usa mucho en fisica de juegos
	- Traccion del jugador: Sirve para simular ela agarre del jugador al suelo.
	- Velocidad de desplazamiento de jugador en dt (distancia / tiempo). Para modo con fisicas en 
	el que el diferencial de la velocidad (derivada) no se calcula en funcion de una aceleracion diferencial
	ni una friccion diferencial sino que se calcula directamente la velocidad en funcion del tiempo. Crea sensacion
	de aceleracion e inercia pero menos realista que cunado la aceleracion y la friccion son diferenciales.
	- Velocidad maxima de desplazamiento del jugador en dt (distancia / tiempo).
	- Velocidad de desplazamiento calulada en cada uno de de sus componetes: Aqui a velocidad ya es un vector
	con componenetes independientes en x e y (dx dy). Esto permite aplicarle aceleraciones y fricciones diferenciales
	y vectorizadas. Se usa para el modo fisicas dukedoom y crea sensacion de inercia y a celeracion mas realista que con 
	la velocidad diferencial sin descomponer el vector en sus componentes. Ademas permite interacciones con otros vectores
	como el viento, superficies en movimiento, explosiones etc..
	- Velocidad maxima de desplazamiento en cada uno de sus componentes (dx dy).
	- Posicion en z de la camara del jugador (ojos): Simula la posicion en z del jugador en el mapa 3D desde un motor 2D
	- Offset vertical de la camara para saltos y caidas: Se usa en el renderizado para simular saltos y caidas al mover
	los puntos del la imagen renderizada en y en funcion de este offset vertical. traslaciones con perspectiva de la imagen	renderizada
	- Aceleracion en z del jugador: Aceleracion vertical del jugador para saltos y caidas (simula la fuerza de salto despreciando masa del jugador)
	Se calcula en funcion de la gravedad del mapa y un factor de aceleracion respecto a la gravedad. MAs mantenible se usa mucho en fisica de juegos.
	- Copia de la aceleracion en z del jugador para saltos y caidas: Esta variable se usa para guardar la aceleracion en caso de cambios de configura
	cion de las fisicas del motor
	- Velocidad en z del jugador: Velocidad vertical del jugador para saltos y caidas. Se calcula en funcion de la aceleracion en z, la gravedad y el tiempo.
	Sirve para que el jugador desplace su camza en z simulando saltos y caidas a lo largo del tiempo.
	- Boleanos de estado del jugador. Sirven para controlar en que estado se encuentra el jugador y aplicar las fisicas correspondientes:
		is_jumping: Indica si el jugador esta saltando
		is_onair: Indica si el jugador esta en el aire (saltando o cayendo)
		is_flying: Indica si el jugador esta volando (modo vuelo)
		is_hided: Indica si el jugador esta agachado
		is_dragging: Indica si el jugador esta arrastrandose
		is_sprinting: Indica si el jugador esta esprintando
		is_groundpound: Indica si el jugador esta haciendo un golpe terrestre (caida rapida)
	
*/
void	init_player_phisics(t_player *pl, t_phisics *ph)
{
	pl->aceleration[FRONT] = ACCELERATION_FRONT_FACTOR * ph->floor_friction;
	pl->aceleration[BACK] = ACCELERATION_BACK_FACTOR * ph->floor_friction;
	pl->aceleration[SIDES] = ACCELERATION_SIDES_FACTOR * ph->floor_friction;
	pl->traccion_k = TRACCION_K;
	pl->speed_dt = 0.0f;
	pl->max_speed_dt = MAX_PLAYER_SPEED;
	pl->speed_a[X] = 0.0f;
	pl->speed_a[Y] = 0.0f;
	pl->max_speed_a[X] = MAX_PLAYER_SPEED;
	pl->max_speed_a[Y] = MAX_PLAYER_SPEED;
	pl->camz = MIDDLE_Z;
	pl->vertical_offset = 0.0f;
	pl->aceleration_z = 0.0f;
	pl->aceleration_zcp = ph->gravity * ACCELERATION_K_EARTH;
	pl->speed_z = 0.0f;
	pl->is_jumping = false;
	pl->is_onair = false;
	pl->is_flying = false;
	pl->is_hided = false;
	pl->is_dragging = false;
	pl->is_sprinting = false;
	pl->is_groundpound = false;
}

/*
	Inicializar los valores del mouse:
	- La posicion x e y del mouse en la ventana (pixeles en la matrix depixeles)
	- El eje de la ventana en x e y
	- El pitch factor del mouse se multiplica por el diferecial de la posion del mouse para saber el pitch
	de inclinacion del jugador cunado se usa el mouse (variable) sensibilidad el mouse en el eje y
	(pitch_pix / pixely)
	- La sensibilidad por moviento de pixeles del mouse en el eje x para rotar al jugador (grados / pixelx)
	- Mouse on off para apagar y encender el mouse
*/
void	init_player_mouse(t_player *pl, t_mlx *mlx)
{
	pl->mouse.pos_x = mlx->win_width / 2;
	pl->mouse.pos_y = mlx->win_height / 2;
	pl->mouse.axis_x = mlx->win_width / 2;
	pl->mouse.axis_y = mlx->win_height / 2;
	pl->mouse.sens_x = MOUSE_INIT_SENSX;
	pl->mouse.pitch_factor = MOUSE_PITCH_FACTOR;
	pl->mouse.onoff = OFF;
	pl->mouse.out_and_on = true;
}