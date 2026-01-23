/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   speed_aceleration.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 14:14:31 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 20:13:14 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Esta función acelera al jugador de manera gradual cuando no está en el aire
	Con esta función calculamos la derivada de la velocidad del jugador haciend
	que cuando se mueva el jugador dentro de un eje se acelere poco a poco, y 
	cuando deje de moverse se desacelere poco a poco simulando una inercia:
	 - Parámetros matemáticos y físicos:
	   Aceleración (constante) = delta_speed / delta_time (derivada de la 
	   velocidad).
	   Delta_time = tiempo transcurrido entre frame y frame (en ms).
	   Delta_speed = incremento de velocidad por frame (constante ajustable).
	   air_friction = coeficiente de fricción del aire que hace que el jugador 
	   se desacelere.
	   Velocidad: La usamos para poder saber el valor de speed que es la 
	   variable que se va a usar para calcular el diferencial de la posición 
	   del jugador en cada eje (x, y) del mapa y así moverlo según una 
	   trayectoria de manera "continua".
	 - Alternativas de cálculo de velocidad:
	   A la hora de calcular la velocidad del jugador tenemos varias 
	   alternativas:
	   - Velocidad instantánea: La velocidad del jugador cambia de manera 
		 instantánea según se presionen las teclas de movimiento. Esto hace que
		 el jugador se mueva de manera brusca y poco realista.
	   - Velocidad con aceleración y desaceleración: La velocidad del jugador 
		 cambia de manera gradual según se presionen las teclas de movimiento. 
		 Esto hace que el jugador se mueva de manera más suave y realista.
	   - Velocidad con inercia: La velocidad del jugador cambia de manera 
		 gradual según se presionen las teclas de movimiento, pero además 
		 cuando se deja de presionar una tecla el jugador sigue moviéndose un 
		 poco más antes de detenerse por completo. Esto simula la inercia del 
		 movimiento y hace que el jugador se mueva de manera más realista.
	 - Alternativas de cálculo del diferencial de la velocidad en el vector de 
	   movimientos según la variable delta que multiplica a la aceleración 
	   (delta):
	   - Delta fijo: Usar un valor fijo para delta (por ejemplo 0.1f) hace que 
		 la aceleración sea constante y predecible. Esto es fácil de implementa
		 pero puede no ser realista en situaciones donde el tiempo entre frames 
		 varía mucho. Esto es la implementación más sencilla, hace que el 
		 jugador se mueva de manera continua, pero tiene dos problemas. Cuando 
		 el frame rate baja el movimiento del jugador se ralentiza y cuando el 
		 frame rate sube el movimiento del jugador se acelera demasiado. Lo 
		 bueno es que en caso de lags el jugador no se teletransporta a otra 
		 posición. Uso típico: motores clásicos, demos, retro FPS (cuando el 
		 software del juego controlaba el nivel de fps).
	   - Delta basado en delta_time: Usar delta_time para calcular delta hace 
		 que la aceleración sea proporcional al tiempo transcurrido entre 
		 frames. Esto es más realista pero puede ser más complejo de 
		 implementar. Esto tiene el problema de que cuando hay lags el jugador 
		 se teletransporta a otra posición. Como ventaja se consigue un 
		 movimiento determinista del jugador independientemente del frame rate. 
		 Esto es útil cuando se quiere tener un control preciso sobre el 
		 movimiento para que sea más predecible. Otra desventaja es que cuando 
		 el frame rate baja también se pueden producir pequeños saltos en el 
		 movimiento del jugador. Esto se puede limitar restringiendo el valor 
		 máximo de delta_time para evitar saltos grandes en el movimiento. Los 
		 cálculos del timestamp ya se estaban usando para el contador de fps, 
		 por lo que los reutilizamos aquí. Uso típico: motores modernos, juegos 
		 con FPS variable, físicas “jugables” más puristas (es útil para que un 
		 juego pueda correrse en diferentes sistemas con diferentes capacidades 
		 de hardware y mantener una experiencia de juego consistente).
	   - Delta basado en el tiempo con substeps: Esta es una variación del delt
		 basado en delta_time donde se divide el tiempo transcurrido entre 
		 frames en substeps más pequeños para mejorar la precisión del cálculo 
		 de la aceleración. Uso típico: motores profesionales, Unity, Unreal, 
		 Source, Doom 3.
	   Conclusión: No usamos un delta basado en fps porque es menos predecible y 
	   porque limitar la tasa de fps de nuestro juego tiene sentido por cuidar 
	   los ciclos de CPU, pero queremos que el motor sea lo más óptimo posible 
	   según lo que lo pueda ejecutar y exprimir el hardware del usuario. Ademá
	   no tenemos la posibilidad de usar usleep para hacer que el tiempo que el 
	   motor esté renderizando a una tasa mayor de fps de que queramos limitar 
	   su cota superior, por lo que solo podríamos hacer un bucle infinito y 
	   esperar a que el tiempo transcurrido entre frames sea mayor al límite, l
	   que consumiría recursos de manera igualmente ineficiente. Por eso tenemo
	   que usar la opción 2 o 3. Metemos la 3 que es la más precisa.
*/

/*

	Explicacion de parametros fisicos:
	- La aceleración:
	  Es el diferencial de la velocidad a lo largo del tiempo (delta time).
	  Al ser un valor ajustable podemos hacer que el jugador acelere más o
	  menos rápido.
	  Se calcula en el inicio del juego como un factor sobre la  fricción, 
	  siempre mayor que 1 para que sea mayor que la fricción. 
	- La traccion:  
	  Se mete un parámetro k (0,1) que es la tracción, normalmente es el 
	  porcentaje de la fricción que cancelamos cuando el suelo se está moviendo. 
	  Este factor es realista a nivel físico porque simula la fuerza de agarre 
	  que tiene el calzado o la rueda (si fuese un coche) con el suelo sobre el 
	  que aplica la aceleración. Si la fuerza de agarre es poca, por mucha 
	  aceleración (potencia) que tenga, no las aplica bien y la fricción con el 
	  medio (suelo, fuerza contraria) hace que no avance tan rápido, es decir, 
	  pierde aceleración efectiva por deslizamiento.
	  Desde el punto de vista práctico, nos sirve para poder cancelar la fricci
	  y que no nos ocurra el problema de antes con la cancelación de aceleración 
	  con la fuerza de fricción y bloqueo con la fuerza mínima. Subiendo la 
	  aceleración se hace que arranque más rápido.
	  - La frciccion del suelo:
	  Es la fuerza contraria a la aceleración que frena al jugador cuando se 
	  está moviendo. Esta fuerza de fricción sería la fuerza que frena al 
	  jugador. Cuanto mayor sea la fricción, más rápido se frenará el jugador 
	  cuando deje de moverse. Esta tambien se manifiesta como una aceleracion
	  negativa que reduce la velocidad del jugador.
	  Se puede ajustar para simular distintos tipos de suelo (hielo, tierra, 
	  asfalto, etc.) y hacer que el jugador se sienta diferente al moverse 
	  sobre ellos.
	- velocidad y velocidad máxima:
	  La velocidad es la magnitud del vector de movimiento del jugador. Se usa 
	  para calcular el diferencial de la posición del jugador en cada frame.
	  La velocidad máxima es el límite superior que puede alcanzar la velocidad 
	  del jugador. Esto evita que el jugador se mueva demasiado rápido y pierda 
	  el control, el valor del diferencial de la velocidad en cada momento, de
	  pende de la compbinacion de la aceleracion total multiplicada por el 
	  delta time.
	Se pueden ajustar para controlar la sensación de movimiento del jugador 
	  y hacer que se sienta más ágil o más pesado.

	Parametros de la función:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene
	  toda la información del juego.
	- vi: Índice del vector de aceleración que indica la dirección de la
	  aceleración (0: adelante, 1: atrás, 2: izquierda, 3: derecha).
	- dt: Delta time, tiempo transcurrido desde la última actualización en
	  milisegundos.
	- mo: Puntero a una variable booleana que indica si el jugador se ha
	  movido en este frame.
	
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
	Esta función aplica una fuerza contraria al movimiento del jugador, 
	simulando la fricción que lo desacelera de manera gradual hasta detenerlo. 
	Se utiliza para crear un entorno más realista y controlado.

	Detalles de implementación:
	- **Frenado:** Cuando el jugador no se está moviendo, la fricción normal no
	  es suficiente para detenerlo rápidamente. Por eso, se aplica un 
	  multiplicador adicional a la fricción para aumentar su efecto. Esto es 
	  similar al estilo de movimiento de juegos como Doom o Quake.
	- **Fuerza de frenado:** Otra opción sería aplicar una fuerza adicional par
	  reducir la velocidad después de la fricción. Aunque no es físicamente 
	  realista, es más fácil de ajustar y se utiliza en algunos motores de 
	  juegos clásicos.
	- **Snap to zero:** Para evitar cálculos innecesarios y mejorar la 
	  jugabilidad, se implementa un "snap to zero". Esto significa que, cuando
	  la velocidad es muy baja, se fuerza a cero directamente. Aunque no es 
	  realista, mejora la experiencia de juego y evita problemas matemáticos 
	  con divisiones pequeñas.
	Parametros:
	- mlx: Puntero a la estructura principal del motor gráfico que contiene
	  toda la información del juego.
	- dt: Delta time, tiempo transcurrido desde la última actualización en
	  milisegundos.
	- mo: Variable booleana que indica si el jugador se ha movido en este
	  frame.
*/
void	decelerate_player(t_mlx *mlx, long long dt, bool mo)
{
	float	speed;
	float	friction;

	speed = mlx->player->speed_dt;
	if (speed <= 0.0f)
		return ;
	friction = mlx->phisics.floor_friction;
	if (mo == false)
		friction *= 10;
	speed -= speed * friction * (float)dt;
	if (speed < SPEEDMIN_MS)
		speed = 0.0f;
	mlx->player->speed_dt = speed;
}

/*
	Aceleración del jugador basada en las velocidades de cada uno de los ejes.
	Se descompone el vector velocidad en sus componentes X e Y para aplicar la
	aceleración de manera independiente en cada eje.
	Esto permite implementar, en el futuro, aceleraciones diferentes para cada 
	eje o factores externos que afecten la aceleración (como vientos laterales, 
	explosiones, etc.). Ademas la inercia y adeslizacmiento se aplican de maner
	independiente en cada eje.

	Características:
	- Se corrige el "strafing" para que la velocidad máxima sea la misma en 
	  todas las direcciones. Aunque no es completamente preciso desde el punto 
	  de vista computacional, mejora la experiencia de juego sin afectar 
	  significativamente la tasa de FPS.
*/
void	acelerate_dukedoom(t_mlx *mlx, unsigned int vi, long long dt, bool *mo)
{
	float	aceleration;
	float	speed[2];
	float	max_speed[2];
	float	traccion;

	speed[X] = mlx->player->speed_a[X];
	speed[Y] = mlx->player->speed_a[Y];
	max_speed[X] = mlx->player->max_speed_a[X];
	max_speed[Y] = mlx->player->max_speed_a[Y];
	traccion = mlx->phisics.floor_friction * mlx->player->traccion_k;
	aceleration = mlx->player->aceleration[vi] * traccion;
	if (speed[X] < max_speed[X])
		speed[X] += dt * aceleration;
	if (speed[Y] < max_speed[Y])
		speed[Y] += dt * aceleration;
	if (speed[X] >= max_speed[X])
		speed[X] = max_speed[X];
	if (speed[Y] >= max_speed[Y])
		speed[Y] = max_speed[Y];
	normalize_vector(speed, max_speed);
	mlx->player->speed_a[X] = speed[X];
	mlx->player->speed_a[Y] = speed[Y];
	*mo = true;
}

/*
	Desaceleración en cada componente del vector de velocidad del jugador. 
	Se utiliza para ajustar la posición del jugador en cada eje según su vector
	de dirección. Este enfoque permite incluir aceleraciones diferenciales que 
	afecten la velocidad del jugador en cada eje (por ejemplo, vientos 
	laterales, explosiones, etc.).
*/
void	decelerate_dukedoom(t_mlx *mlx, long long dt, bool mo)
{
	float	speed[2];
	float	friction;

	speed[X] = mlx->player->speed_a[X];
	speed[Y] = mlx->player->speed_a[Y];
	friction = mlx->phisics.floor_friction;
	if (mo == false)
		friction *= 10;
	if (speed[X] > SPEEDMIN_MS)
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
