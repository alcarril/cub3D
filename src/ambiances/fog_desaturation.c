/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fog_desaturation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/10 15:40:14 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/22 21:03:20 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Aplica un efecto de fog niebla o particulas de un color e n fucnion de la
	distancia proporcional p entre 0.0 y 1.0. Si p es 0.0 no se aplica ningun
	efecto de fog y el color original se mantiene. Con esto se consigue simular
	la atenuacion del color con la distancia, haciendo que los objetos lejanos
	se vean mas afectados por el color de la niebla. Da profundidad de manera
	barata sin terner que usar postprocesados complejos.
	Pasos:
	- Se descompone el color original y el color de fog en sus componentes RGB.
	- Se calcula el nuevo color mezclando cada componente del color original 
	  con la componente correspondiente del color de fog, ponderado por 
	  el factor p.
	- Finalmente se vuelve a componer el color y se retorna el nuevo color con
	  el efecto de fog aplicado.
	Parametros:
	- col: Color original en formato hexadecimal (0xRRGGBB).
	- fog_color: Color de la niebla en formato hexadecimal (0xRRGGBB).
	- p: Proporcion de la distancia entre 0.0 (sin fog) y 1.0 
	  (completamente fog).
	Mejoras de microporcesador:
	- Se usan operaciones de desplazamiento de bits para extraer y combinar los
	  componentes RGB, que son mas eficientes que las operaciones aritmeticas
	  tradicionales. (mucho menos ciclos de procesador por poreracion)
	- Se evitan operaciones de division y modulo, que son mas lentas.
	- Uso de arrays de variables locales para usar registros de microprocesador
	  en lugar de memoria, lo que mejora la velocidad de acceso a datos.
*/
unsigned int	apply_fog_pixel(unsigned int col, unsigned int fog_c, float p)
{
	unsigned char	rgb[3];
	unsigned char	fog[3];
	int				i;

	if (p > 1.0f)
		p = 1.0f;
	rgb[R] = (col >> 16) & 0xFF;
	rgb[G] = (col >> 8) & 0xFF;
	rgb[B] = col & 0xFF;
	fog[R] = (fog_c >> 16) & 0xFF;
	fog[G] = (fog_c >> 8) & 0xFF;
	fog[B] = fog_c & 0xFF;
	i = 0;
	while (i < 3)
	{
		rgb[i] = (unsigned char)((1.0f - p) * rgb[i] + p * fog[i]);
		i++;
	}
	return ((rgb[R] << 16) | (rgb[G] << 8) | rgb[B]);
}

/*
	Esta funcion aplica un efecto de desaturacion a un color dado en funcion
	del factor de desaturacion. Cuando el factor es 0.0, el color original se
	mantiene sin cambios. Cuando el factor es 1.0, el color se convierte en
	gris (completamente desaturado). Valores intermedios producen colores
	parcialmente desaturados.
	- Se descompone el color original en sus componentes RGB.
	- Se calcula el valor de gris utilizando la formula de luminosidad,
	  que pondera cada componente RGB segun su contribucion a la percepcion
	  humana del brillo.
	- Se mezcla cada componente RGB del color original con el valor de gris,
	  ponderado por el factor de desaturacion.
	- Finalmente se vuelve a componer el color y se retorna el nuevo color
	  con el efecto de desaturacion aplicado.
	Mejoras de microporcesador:
	- Se usan operaciones de desplazamiento de bits para extraer y combinar los
	  componentes RGB, que son mas eficientes que las operaciones aritmeticas
	  tradicionales. (mucho menos ciclos de procesador por poreracion)
	- Se evitan operaciones de division y modulo, que son mas lentas.
*/
unsigned int	apply_desaturation(unsigned int color, float factor)
{
	unsigned char	rgb[3];
	unsigned char	gray;
	int				i;

	if (factor < 0.0f)
		factor = 0.0f;
	if (factor > 1.0f)
		factor = 1.0f;
	rgb[R] = (color >> 16) & 0xFF;
	rgb[G] = (color >> 8) & 0xFF;
	rgb[B] = color & 0xFF;
	gray = (unsigned char)(0.299f * rgb[R] + 0.587f * rgb[G] + 0.114f * rgb[B]);
	i = 0;
	while (i < 3)
	{
		rgb[i] = (unsigned char)((1.0f - factor) * rgb[i] + factor * gray);
		i++;
	}
	return ((rgb[R] << 16) | (rgb[G] << 8) | rgb[B]);
}
