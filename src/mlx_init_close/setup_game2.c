/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_game2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 16:54:24 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 23:00:09 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Configuracion por defecto de la ambiancia del motor:
	- Color de la niebla en las paredes
	- Color de la niebla en el suelo y techo
	- Distancia maxima del mapa para el calculo de la niebla 
	(factor de normalizacion)
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
	Configuración por defecto de las físicas del motor:

	Detalles:
	- **Gravedad por defecto del motor:** Representa la fuerza de atracción en 
	  el eje Z que afecta al jugador. Se utiliza para calcular la aceleración 
	  total del jugador en el eje Z, que es la diferencia entre la fuerza de 
	  salto del jugador y la gravedad del mapa en función del tiempo. La masa 
	  del jugador se desprecia para simplificar los cálculos.
	- **Fricción del aire:** Simula la resistencia del aire al movimiento del 
	  jugador cuando está en el aire. Esto desacelera la velocidad del jugador 
	  en los ejes X e Y en función del tiempo.
	- **Fricción del suelo:** Representa la resistencia del suelo al movimiento 
	  del jugador. Se utiliza para desacelerar la velocidad del jugador en los 
	  ejes X e Y en función del tiempo.

	Parámetros:
	- phisics: Puntero a la estructura de físicas que se va a inicializar.

	NOTA:
	- Los valores de gravedad y fricción están definidos como constantes en el 
	  archivo de configuración del motor gráfico.
	- Estas configuraciones son esenciales para simular un movimiento realista 
	  del jugador en el entorno del juego.
*/
void	setup_default_phisics(t_phisics *phisics)
{
	phisics->gravity = GRAVITY_EARTH;
	phisics->air_friction = AIR_FRICTION_MS;
	phisics->floor_friction = FLOOR_FRICTION_MS;
}

/*
	Inicilizamos los datos de la física del jugador:

	- **Aceleración en función de la fricción del suelo:** Siempre se usa una 
	  fricción de suelo y un factor de aceleración en función del tipo de 
	  movimiento del jugador (lateral, frontal, trasero). Relacionar la 
	  aceleración con la fricción con un factor es más mantenible que usar 
	  valores fijos. Este enfoque es común en la física de juegos.
	- **Tracción del jugador:** Sirve para simular el agarre del jugador al 
	  suelo.
	- **Velocidad de desplazamiento del jugador en dt (distancia/tiempo):** 
	  Para el modo con físicas en el que el diferencial de la velocidad 
	  (derivada) no se calcula en función de una aceleración diferencial ni 
	  una fricción diferencial, sino que se calcula directamente la velocidad 
	  en función del tiempo. Esto crea una sensación de aceleración e inercia, 
	  pero es menos realista que cuando la aceleración y la fricción son 
	  diferenciales.
	- **Velocidad máxima de desplazamiento del jugador en dt (distancia/tiempo)
	- **Velocidad de desplazamiento calculada en cada uno de sus componentes:** 
	  Aquí la velocidad ya es un vector con componentes independientes en X e Y 
	  (dx, dy). Esto permite aplicarle aceleraciones y fricciones diferenciales 
	  y vectorizadas. Se usa para el modo físicas DukeDoom y crea una sensación 
	  de inercia y aceleración más realista que con la velocidad diferencial 
	  sin descomponer el vector en sus componentes. Además, permite 
	  interacciones con otros vectores como el viento, superficies en 
	  movimiento, explosiones, etc.
	- **Velocidad máxima de desplazamiento en cada uno de sus componentes 
	  (dx, dy).**
	- **Posición en Z de la cámara del jugador (ojos):** Simula la posición en 
	  Z del jugador en el mapa 3D desde un motor 2D.
	- **Offset vertical de la cámara para saltos y caídas:** Se usa en el 
	  renderizado para simular saltos y caídas al mover los puntos de la imagen 
	  renderizada en Y en función de este offset vertical. Esto permite 
	  realizar traslaciones con perspectiva de la imagen renderizada.
	- **Aceleración en Z del jugador:** Aceleración vertical del jugador para 
	  saltos y caídas (simula la fuerza de salto despreciando la masa del 
	  jugador). Se calcula en función de la gravedad del mapa y un factor de 
	  aceleración respecto a la gravedad. Este enfoque es más mantenible y se 
	  usa mucho en la física de juegos.
	- **Copia de la aceleración en Z del jugador para saltos y caídas:** Esta 
	  variable se usa para guardar la aceleración en caso de cambios en la 
	  configuración de las físicas del motor.
	- **Velocidad en Z del jugador:** Velocidad vertical del jugador para 
	  saltos y caídas. Se calcula en función de la aceleración en Z, la 
	  gravedad y el tiempo. Sirve para que el jugador desplace su cámara en Z, 
	  simulando saltos y caídas a lo largo del tiempo.
	- **Booleanos de estado del jugador:** Sirven para controlar en qué estado 
	  se encuentra el jugador y aplicar las físicas correspondientes:
	  - `is_jumping`: Indica si el jugador está saltando.
	  - `is_onair`: Indica si el jugador está en el aire (saltando o cayendo).
	  - `is_flying`: Indica si el jugador está volando (modo vuelo).
	  - `is_hided`: Indica si el jugador está agachado.
	  - `is_dragging`: Indica si el jugador está arrastrándose.
	  - `is_sprinting`: Indica si el jugador está esprintando.
	  - `is_groundpound`: Indica si el jugador está haciendo un golpe terrestre
		(caída rápida).
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
	Inicializa los valores del ratón para el jugador:

	Detalles:
	- Configura la posición inicial del ratón en la ventana (`pos_x` y 
	  `pos_y`) como el centro de la ventana, calculado dividiendo el ancho y 
	  alto de la ventana entre 2.
	- Establece los ejes de referencia del ratón (`axis_x` y `axis_y`) en el 
	  centro de la ventana. Estos ejes se utilizan como punto de referencia 
	  para calcular el movimiento relativo del ratón.
	- Configura el factor de inclinación del ratón (`pitch_factor`), que se 
	  multiplica por el diferencial de la posición del ratón en el eje Y para 
	  calcular el `pitch` (inclinación vertical) del jugador. Este valor 
	  representa la sensibilidad del ratón en el eje Y.
	- Configura la sensibilidad del ratón en el eje X (`sens_x`), que se 
	  utiliza para calcular la rotación del jugador en grados por cada píxel 
	  de movimiento del ratón en el eje X.
	- Inicializa el estado del ratón (`onoff`) como apagado (`OFF`), lo que 
	  significa que el ratón no está activo al inicio del juego.
	- Configura la variable `out_and_on` como `true`, lo que indica que el 
	  ratón está fuera de la ventana o que se ha activado recientemente.

	Parámetros:
	- pl: Puntero a la estructura del jugador que contiene los datos del ratón
	  a inicializar.
	- mlx: Puntero a la estructura principal del motor gráfico que contiene 
	  información sobre la ventana del juego.

	NOTA:
	- La sensibilidad del ratón en los ejes X e Y puede ajustarse dinámicamente
	  durante el juego para adaptarse a las preferencias del jugador.
	- La posición inicial del ratón en el centro de la ventana asegura que el 
	  movimiento relativo del ratón sea consistente desde el inicio del juego.
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
