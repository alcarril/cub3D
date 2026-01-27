/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memfillboost.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 20:25:34 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 20:50:41 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Puedoo usar sizeof ya que el compilador lo hace en compilacion
	menos cuando se hace sizeof de un variable no de un tipo de dao
	Ademas podria usar la dicsion constantemente poreel compilador lo trasnform
	por bitwise si es potencia de dos y es recurrente.

	libc internals
	optimización de memoria
	arquitectura de CPU
	ABI
	Estás tocando:
		alineación de memoria
		word writes
		construcción de patrones por shifts
		dispatch por tamaño
		Esto es exactamente lo que se estudia en:
		implementaciones de libc
		kernels
		motores de memoria
		
*/
void	ft_memfillboost(void *s, int c, size_t n)
{
	while (n > 0 && ((uintptr_t)s % sizeof(unsigned long long)) != 0)
	{
		*(unsigned char *)s = ((unsigned char *)&c)[0];
		s = (unsigned char *)s + 1;
		n--;
	}
	if (n >= sizeof(unsigned long long))
		ft_memfilllonglong(&s, c, &n);
	if (n >= sizeof(unsigned long))
		ft_memfilllong(&s, c, &n);
	if (n >= sizeof(int))
		ft_memfillint(&s, c, &n);
	else
		ft_memfillchar(s, c, n);
}

/*
	// jumps = *n / sizeof(unsigned long long);
*/
void	ft_memfilllonglong(void **s, int c, size_t *n)
{
	unsigned long long	data;
	unsigned long long	*ptr;
	size_t				jumps;
	size_t				i;

	data = (unsigned int)c;
	data |= data << 32;
	ptr = (unsigned long long *)(*s);
	jumps = *n >> 3;
	i = 0;
	while (i < jumps)
		ptr[i++] = data;
	*s = (void *)(ptr + i);
	*n -= jumps * sizeof(unsigned long long);
}

/*
	#if ULONG_MAX > 0xFFFFFFFFUL
	data |= data << 32;   // Si long es 8 bytes
	#endif
	// jumps = *n / sizeof(unsigned long long);
	Este comentario lo dejams porque hay que comporbar
	si al arquitectura de So es de 64 bits o 32 bits
	en los ordenadores de 42. Cundo lo pucliquemos podemos
	poner las directivas del preporcesador dentro de la fuuncion in
	dicando que nos estamos saltando la norma por compatibilidad
*/
void	ft_memfilllong(void **s, int c, size_t *n)
{
	unsigned long	data;
	unsigned long	*ptr;
	size_t			jumps;
	size_t			i;

	data = (unsigned int)c;
	data |= data << 32;
	ptr = (unsigned long *)(*s);
	jumps = *n >> 3;
	i = 0;
	while (i < jumps)
		ptr[i++] = data;
	*s = (void *)(ptr + i);
	*n -= jumps * sizeof(unsigned long);
}

/*
	// jumps = *n / sizeof(int);
*/
void	ft_memfillint(void **s, int c, size_t *n)
{
	int		*ptr;
	size_t	jumps;
	size_t	i;

	ptr = (int *)(*s);
	jumps = *n >> 2;
	i = 0;
	while (i < jumps)
		ptr[i++] = c;
	*s = (void *)(ptr + i);
	*n -= jumps * sizeof(int);
}

void	ft_memfillchar(void *s, int c, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
		ptr[i++] = (unsigned char)c;
}
