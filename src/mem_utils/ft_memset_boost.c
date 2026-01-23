/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset_boost.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 00:43:30 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 20:53:11 by alejandro        ###   ########.fr       */
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
void	ft_memsetboost(void *s, int c, size_t n)
{
	while (n > 0 && ((uintptr_t)s % sizeof(unsigned long long)) != 0)
	{
		*(unsigned char *)s = ((unsigned char *)&c)[0];
		s = (unsigned char *)s + 1;
		n--;
	}
	if (n >= sizeof(unsigned long long))
		ft_memsetlonglong(s, c, n);
	else if (n >= sizeof(unsigned long))
		ft_memsetlong(s, c, n);
	else if (n >= sizeof(int))
		ft_memsetint(s, c, n);
	else
		ft_memsetchar(s, c, n);
}

/*
	#if ULONG_MAX > 0xFFFFFFFFUL
	data |= data << 32;   // Si long es 8 bytes
	#endif
*/
void	ft_memsetlonglong(void *s, int c, size_t n)
{
	unsigned long long	data;
	unsigned long long	*ptr;
	size_t				jumps;
	size_t				i;

	data = (unsigned int)c;
	data |= data << 32;
	ptr = (unsigned long long *)s;
	jumps = n >> 3;
	i = 0;
	while (i < jumps)
		ptr[i++] = data;
	s = (void *)(ptr + i);
	n -= jumps * sizeof(unsigned long long);
	ft_memsetboost(s, c, n);
}

/*
	#if ULONG_MAX > 0xFFFFFFFFUL
	data |= data << 32;   // Si long es 8 bytes
	#endif
*/
void	ft_memsetlong(void *s, int c, size_t n)
{
	unsigned long	data;
	unsigned long	*ptr;
	size_t			jumps;
	size_t			i;

	data = (unsigned int)c;
	data |= data << 8;
	data |= data << 16;
	ptr = (unsigned long *)s;
	jumps = n >> 3;
	i = 0;
	while (i < jumps)
		ptr[i++] = data;
	s = (void *)(ptr + i);
	n -= jumps * sizeof(unsigned long);
	ft_memsetboost(s, c, n);
}

void	ft_memsetint(void *s, int c, size_t n)
{
	int		*ptr;
	size_t	jumps;
	size_t	i;

	ptr = (int *)s;
	jumps = n >> 2;
	i = 0;
	while (i < jumps)
		ptr[i++] = c;
	s = (void *)(ptr + i);
	n -= jumps * sizeof(int);
	ft_memsetboost(s, c, n);
}

void	ft_memsetchar(void *s, int c, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	ptr = (unsigned char *)s;
	i = 0;
	while (i < n)
		ptr[i++] = (unsigned char)c;
}
