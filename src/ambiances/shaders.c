/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shaders.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 21:01:39 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/22 21:16:58 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Funcion para aplicar las sombra a un color. Cuando descomponemos 
	el color en sus componentes RGB, multiplicamos cada componente por el 
	factor de sombra (shade).
	Primero se descompone el color, una vez decoompuesto se aplica el factor 
	de sombra que es una valor entre 0.0 y 1.0. Esto es poruqe los coloeres 
	cuando se oscurecen mantiene la misma porportcion entre rgb para que el 
	color no cambien pero cuanto mayor sea el valor de las componenertes mas o
	scuro se vera el color resultante cunado se vuelva a componer. 
	Finalmente se vuelve a componer el color y se retorna.
	Mejoras de microporcesador:
	- Se usan enteros para las operaciones de multiplicacion y desplazamiento 
	  de bits en lugar de flotantes, lo que es mas rapido en la mayoria de 
	  los microprocesadores CPU.
	- Se evitan operaciones de division y modulo, que son mas lentas.
	- Se usan operaciones de desplazamiento de bits para extraer y combinar 
	  los componentes RGB, que son mas eficientes que las operaciones 
	  aritmeticas tradicionales. (mucho menos ciclos de procesador)
*/
unsigned int	apply_shade(unsigned int color, float shade)
{
	int	r;
	int	g;
	int	b;

	r = (int)(((color >> 16) & 255) * shade);
	g = (int)(((color >> 8) & 255) * shade);
	b = (int)((color & 255) * shade);
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
	Estas funciones calculan y normalizan el factor de sombra (entre 0.0 y 1.0)
	en función de la distancia y la distancia máxima. El factor de sombra se
	calcula restando la proporción de la distancia actual sobre la distancia
	máxima a 1.0. Si la distancia es mayor que la distancia máxima, el factor
	de sombra se establece en 0.0 (completamente oscuro). Con esto conseguimos
	simular profundidad y atenuación de la luz con la distancia sin tener que
	usar un postprocesado o preprocesado de la imagen.

	El cálculo del factor de sombra se realiza para cada píxel de la matriz de
	píxeles de la pantalla en el renderizado. Cada píxel representa una pequeña 
	porción de la escena, y su color se ajusta en función de la distancia al 
	observador y el efecto de iluminación deseado. Esto permite renderizar 
	paredes, suelos y techos con efectos de iluminación realistas.

	Cada una de estas funciones usa un método diferente para calcular el factor
	de sombra:
	- Método lineal: El factor de sombra disminuye linealmente con la distancia.
	  Es el método más sencillo y comúnmente usado, pero no simula bien nieblas
	  densas o efectos atmosféricos.
	- Método inverso: El factor de sombra disminuye inversamente con la
	  distancia. Este método es más eficiente computacionalmente y simula
	  nieblas ligeras y efectos atmosféricos.
	- Método exponencial: El factor de sombra disminuye exponencialmente con la
	  distancia. Este es más costoso computacionalmente, pero simula nieblas
	  densas y efectos atmosféricos complejos.
*/
unsigned int	shade_linear(unsigned int color, float dist, float max_dist)
{
	float	shade;

	shade = 1.0f - (dist / max_dist);
	if (shade < 0.0f)
		shade = 0.0f;
	if (shade > 1.0f)
		shade = 1.0f;
	return (apply_shade(color, shade));
}

unsigned int	shade_inverse(unsigned int color, float k, float prop_dist)
{
	float	shade;

	shade = 1.0f / (1.0f + prop_dist * k);
	return (apply_shade(color, shade));
}

unsigned int	shade_exponential(unsigned int color, float dens, float prop)
{
	float	shade;

	shade = expf(-prop * dens);
	return (apply_shade(color, shade));
}
