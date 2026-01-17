/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 21:01:39 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/16 20:43:14 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Funcion para aplicar las sombra a un color. Cuando descomponemos el color en sus
	componentes RGB, multiplicamos cada componente por el factor de sombra (shade).
	Primero se descompone el color, una vez decoompuesto se aplica el factor de sombra
	que es una valor entre 0.0 y 1.0. Esto es poruqe los coloeres cuando se oscurecen
	mantiene la misma porportcion entre rgb para que el color no cambien pero cuanto mayor
	sea el valor de las componenertes mas oscuro se vera el color resultante cunado se vuelva
	a componer. Finalmente se vuelve a componer el color y se retorna.
	Mejoras de microporcesador:
	- Se usan enteros para las operaciones de multiplicacion y desplazamiento de bits
	  en lugar de flotantes, lo que es mas rapido en la mayoria de los microprocesadores.
	- Se evitan operaciones de division y modulo, que son mas lentas.
	- Se usan operaciones de desplazamiento de bits para extraer y combinar los componentes RGB,
	  que son mas eficientes que las operaciones aritmeticas tradicionales. (mucho menos ciclos de procesador)
*/
unsigned int apply_shade(unsigned int color, float shade)
{
	int r = (int)(((color >> 16) & 255) * shade);
	int g = (int)(((color >> 8) & 255) * shade);
	int b = (int)((color & 255) * shade);

	if (r > 255) 
		r = 255; 
	if (r < 0) 
		r = 0;
	if (g > 255) 
		g = 255; 
	if (g < 0) 
		g = 0;
	if (b > 255) 
		b = 255; 
	if (b < 0) 
		b = 0;
	return ((r << 16) | (g << 8) | b);
}

/*
	Estas funciones sirven para poder calcular normalizar el factor de shde entre 0.0 y 1.0
	en funcion de la distancia y la distancia maxima. El factor de sombra se calcula restando
	la proporcion de la distancia actual sobre la distancia maxima a 1.0. Si la distancia es
	mayor que la distancia maxima, el factor de sombra se establece en 0.0 (completamente oscuro).
	Con esto conseguimos simular profundidad y atenuacion de la luz con la distancia sin tener que 
	usar un postporcesado o precporcesado de la imgagen, lo hacemos mientras elegimos el color de
	cada pixel de la matriz de pixeles de la pantalla cunado estamos renderizando las paredes, suelo y techo.
	Cada una de estas fucniones usa un metodo diferente para calcular el factor de sombra.
	- Metodo lineal: El factor de sombra disminuye linealmente con la distancia. Es el metodo
	  mas sencillo y comunmente usado. pero no simula bien nieblas densas o efectos atmosfericos.
	- Metodo inverso: El factor de sombra disminuye inversamente con la distancia. Ete metodo es
	  mas eficiente computacionalmente y simula nieblas ligeras y efectos atmosfericos.
	- Metodo exponencial: El factor de sombra disminuye exponencialmente con la distancia Este es muy caro
	  computacionalmente hablando pero simula nieblas densas y efectos atmosfericos.
*/

unsigned int shade_linear(unsigned int color, float dist, float max_dist)
{
	float shade = 1.0f - (dist / max_dist);
	if (shade < 0.0f) 
		shade = 0.0f;
	if (shade > 1.0f) 
		shade = 1.0f;
	return (apply_shade(color, shade));
}

unsigned int shade_inverse(unsigned int color, float k, float proportion_dist)
{
	float	shade;
	
	shade  = 1.0f / (1.0f + proportion_dist * k);
	return (apply_shade(color, shade));
}

unsigned int shade_exponential(unsigned int color, float density, float proportion_dist)
{	
	float shade;
	
	shade  = expf(-proportion_dist * density);
	return (apply_shade(color, shade));
}

