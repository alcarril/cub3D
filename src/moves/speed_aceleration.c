/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   speed_aceleration.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:14:31 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/21 17:07:11 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Estta funcoin acelera al jugador de manera cunado no esta en el aire.
	Con esta funcion calculamos la derivada de la velocidad del jugador haciendo que cuando
	se mueva el jugador dentro de un eje se acelere poco a poco, y cundo de je de moerse
	se desacelere poco a poco simulando una inercia:
	 - Parametros amtematicos y fisicos:
	  Aceleracion (constane) = delta_speed / delta_time (derivada de la velocidad).
	  Delta_time = tiempo trnascuttido entre fram y frame (en ms).
	  Delta_speed = incremento de velocidad por frame (constante ajustable).
	  air_friction = coeficiente de friccion del aire que hace que el jugador se desacelere
	  Velocidad:  La usamos para poder saber el valor de speed que es la variable que 
	  se va usar para calcular el diferencial de la posicion del jugador 
	  en caja eje (x, y) del mapa y asi moerlo segun una trayectoriade manera "continua"
	 - Alternativas de calcuclo de velocidad:
	  A la hora de calcular la velocidad del jugador tenemos varias alternativas:
	   - Velocidad instantanea: La velocidad del jugador cambia de manera instantanea
	   segun se presionen las teclas de moiento. Esto hace que el jugador se mueva
	   de manera brusca y poco realista.
	   - Velocidad con aceleracion y desaceleracion: La velocidad del jugador cambia
	   de manera gradual segun se presionen las teclas de moiento. Esto hace que el
	   jugador se mueva de manera mas suave y realista.
	   - Velocidad con inercia: La velocidad del jugador cambia de manera gradual segun
	   se presionen las teclas de moiento, pero ademas cuando se deja de presionar
	   una tecla el jugador sigue moiendose un poco mas antes de detenerse por completo.
	   Esto simula la inercia del moimiento y hace que el jugador se mueva de manera
	   mas realista.
	 - Alternativas de calculo del diferencial de la velocidad en el vector de moimientos
	 segun la variable delta que multiplica a la aceleracion (delta)
	  - Delta fijo: Usar un valor fijo para delta (por ejemplo 0.1f) hace que la aceleracion
	  sea constante y predecible. Esto es facil de implementar pero puede no ser realista
	  en situaciones donde el tiempo entre frames varía mucho. Esto es la implmetacio mas sencilla
	  hace que el jugadoir se mueva de manera continua , pero tiene dos porblemas. Cunado el frame rate
	  bajja el moiento del jugador se relentiza y cunado el frame rate sube el moiento del jugador se acelera
	  demasiado. Lo bueno que es que n caso de lags el jugador no se teleporta a otra posicion.
	   Uso típico: motores clásicos, demos, retro FPS. (cunado el softare del juego iba controlaba el nivel de fps)
	  - Delta basado en delta_time: Usar delta_time para calcular delta hace que la aceleracion
	  sea proporcional al tiempo transcurrido entre frames. Esto es mas realista pero puede
	  ser mas complejo de implementar. Esto tiene el problema de que cunado hay lags el jugador se teleporta
	  a otra posicion. Como ventaja se sonsigue un moiento determinista del jugador independientemente del frame rate.
	  Esto es util cunado se quiere tener un control preciso sobre el moiento del para que sea mas 
	  predecible. Otra desventaj es que cuando el frame rate baja tambien se pueden porducir pequeños saltos en el moiento del jugador.
	  Esto se puede clipearr limitando el valor maximo de delta_time para evitar saltos grandes en el moiento.
	  Los calculos del timestamp ya se estaba unsado para el contador de fps por lo que los reutilizamos aqui.
	 Uso típico: motores modernos, juegos con FPS variable, físicas “jugables” más puristas. (es util para que un juego pueda correrse en diferentes sistemas con diferentes capacidades de hardware
	  y mantener una experiencia de juego consistente).
	  - Delta basado en en el tiempo con substeps: Esta es una variacion del delta basado en delta_time donde se divide el tiempo transcurrido
	  entre frames en substeps mas pequeños para mejorar la precision del calculo de la aceleracion. 
	  Uso típico: motores profesionales, Unity, Unreal, Source, Doom 3.
	  Conclusion: No usamo un delta basado en fps porque ees menos predecible y porque lmitar la tasasde fps de 
	  nuestra juego tiene sentido por cuidar los ciclos de cpu pero queremos que el motor se alo mas optimo posible
	  segun lo que lo pueda ejecutar exprimir el hardware del usuario. Demas no tenemos la posibilidad de usar uslee
	  para hacer que el tiempo que el motor este renderizando a un tasa mayor de fps de que queremaso limitar en suc ota 
	  superior por lo que solo podriamos hacer un bucle infiniy esperar a que el tiempo transcurrido entre frames sea mayor al limite
	  lo que consumiria recursos de meeria igualemnte. Por eso tenemos que usar la opcion 2 o 3. Metemos la 3 que es la mas precisa.

	  //meterle el headboobing
*/


/*
	La aceleracion se calcula en el inicion del juego como un factor sobre la friccion
	Simpre mayor que que un oapra que se mayor que la friccion. Ademas se mete un parametro 
	k (0,1) que es la traacion, normalemte es el porcenate de la fricion que calcelamos cunado
	el suelo se esta moiendo. Este factor realista a anivel fisco porque simula la fuerza de agarre
	que tiene el calzado o la rueda si fuese un coche con el suelo sobre el que aplica la aceleracion
	Si la fuerza de aggare es mo¡pca por mucha aceleracio (potencia) que tenga no las appica bien y la
	frccion con el medio (suelo fuerza contraria) hace qu eno avance tan rapido es decir pierde aceleracion 
	efectiva.
	Desde elpunto de vista practino no s sirve para poder cancelar la friccion y que no nos ocurra el probelamd eantes 
	con la calcelacion de aceleracion con la fuerza de frccicion y bloqeuso con la fuerza minima
	Subiendo la aceleracion se haceq que arranque mas rapido
	La ocmbiancion de los valores de aceleracionm traccion y friccion hace que podamos conseguir distinitos efectos 
	de moiento simulano sistinito tipos de suelo.
*/
void	acelerate_player(t_mlx *mlx, unsigned int vi, long long dt, bool *mo)
{
	float	accel;
	float	speed;
	float	max_speed;
	float	traction;

	speed = mlx->player->speed_dt;
	max_speed = mlx->player->max_speed_dt;	
	traction = mlx->phisics.floor_friction * mlx->player->traccion_k;
	accel = mlx->player->aceleration[vi] * traction;
	speed += accel * (float)dt;
	if (speed > max_speed)
		speed = max_speed;
	mlx->player->speed_dt = speed;
	*mo = true;
}


/*
	Sea plica la fuerza contraria al moiento que es la fuerza que frena al jugador simulndo la fisicas de un
	entrono realista. Esta fuerza de friccion seria la fuerza que frena al jugador. Cunado se frena el jugador para
	Controlar el tema de la dicion exponenecial del la velociadad por un factor menor que uno se usa:
	Para el frenado cunad on ose esta moiendo no basta con la calcelacion 
	que hace la traccion asi que hay que meterle un multiplicado si no queremos
	que este haciend ocalculos muy grandes a la friccion y asi no tocar la friccion 
	de base para las configuraciones. Es decir no hacer lo contrario que es meter un divisior
	cuando se esta moiendo y pone run friccion mas alta porque de la maner auq el o tengo ya fuciona
	esto es estilo doom/quake
	Otra poccion es mete run afuersxa de frernado del jugador que disminuya la speed despues de 
	aplicarle la friccion pero no es lo mas realista a nivel de fiscas del mapa auqnuq ei que es muy manipublae
	esto es el estilo doom/quake
	Ademas metemos el spano to zero que no es fisica tal cual pero es puero estilo gameplay para poder
	controlar el tema de las matematicas de la divisiones ( se ua mucho en shooters) se rompe la fisica a porpo
	sitop
*/
void	decelerate_player(t_mlx *mlx, long long dt, bool mo)
{
	float	speed;
	float	friction;

	speed = mlx->player->speed_dt;
	if (speed <= 0.0f)
		return;
	friction = mlx->phisics.floor_friction;
	if (mo == false)
		friction *= 10;
	speed -= speed * friction * (float)dt;
	if (speed < SPEEDMIN_MS)
		speed = 0.0f;
	mlx->player->speed_dt = speed;
}


/*
	Aceleracion del jugador en base a su sus velocidades en cada uno de los ejes. Esto permite
	implemtar e un futuro una aceleracion siferente por cada uno de los ejes o factores externos
	que afecten a la aceleracion en cada uno de los ejes. (como veintos laterales, golpes de explosiones etc)
	Amdemas se corrige el stracing para que la velocidad maxima sea la misma en todas las direcciones (no conste computa
	mente pero es un detalle menor que mejora la experiencia de juego y no se nota en la tasa de fps).
*/
void	acelerate_dukedoom(t_mlx *mlx, unsigned int vi, long long dt, bool *mo)
{
	float 		aceleration;
	float 		speed[2];
	float		max_speed[2];
	float		traccion;

	speed[X] = mlx->player->speed_a[X];
	speed[Y] = mlx->player->speed_a[Y];
	max_speed[X] = mlx->player->max_speed_a[X];
	max_speed[Y] = mlx->player->max_speed_a[Y];
	traccion = mlx->phisics.floor_friction * mlx->player->traccion_k;
	aceleration = mlx->player->aceleration[vi] * traccion;
	if(speed[X] < max_speed[X])
		speed[X] += dt * aceleration;
	if (speed[Y] < max_speed[Y])
		speed[Y] += dt * aceleration;
	if(speed[X] >= max_speed[X])
		speed[X] = max_speed[X];
	if (speed[Y] >= max_speed[Y])
		speed[Y] = max_speed[Y];
	normalize_vector(speed, max_speed);
	mlx->player->speed_a[X] = speed[X];
	mlx->player->speed_a[Y] = speed[Y];
	*mo = true;
}


/*
	Decelaracion en cada un de las componeentes de vectore veloicidad del jugador. Que se va a usar para escalar
	la posicion del jugador en cada uno de los ejes. segun su vector de direccion. Esto se hace asi porque se puede
	incluir un aaceleracion diferencial que afecte a la velocidad diferencial del jugador.
*/
void	decelerate_dukedoom(t_mlx *mlx, long long dt, bool mo)
{
	float 		speed[2];
	float 		friction;

	speed[X] = mlx->player->speed_a[X];
	speed[Y] = mlx->player->speed_a[Y];
	friction = mlx->phisics.floor_friction;
	if (mo == false)
		friction *= 10;
	if(speed[X] > SPEEDMIN_MS)
		speed[X] -= speed[X] * friction * (float)dt;
	if (speed[X] <= SPEEDMIN_MS)
		speed[X] = 0.0f;
	if (speed[Y] > SPEEDMIN_MS)
		speed[Y] -= speed[Y] * friction * (float)dt;
	if (speed[Y] <= SPEEDMIN_MS)
		speed[Y] = 0.0f;
	mlx->player->speed_a[X] = speed[X];
	mlx->player->speed_a[Y] = speed[Y];
}
