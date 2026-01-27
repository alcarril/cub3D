/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alejandro <alejandro@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/16 00:11:37 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/23 21:38:22 by alejandro        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cube3D.h"

/*
	Función para determinar si un valor del mapa representa una pared
	- Recibe un valor de mapa (char)
	- Compara el valor con los defines WALL y BONUS_WALL
	- Retorna true si el valor es una pared, false en caso contrario
*/
bool	is_wall_tile(char map_value)
{
	if (map_value == WALL || map_value == BONUS_WALL)
		return (true);
	return (false);
}

/*
	Puedoo usar sizeof ya que el compilador lo hace en compilacion
	menos cuando se hace sizeof de un variable no de un tipo de dao
	Ademas podria usar la dicsion constantemente poreel compilador lo trasnforma 
	por bitwise si es potencia de dos y es recurrente
*/
void	ft_memset_int(void *s, int c, size_t n)
{
	size_t	i;
	size_t	jumps;
	int		*ptr;

	ptr = (int *)s;
	i = 0;
	jumps = n / sizeof(int);
	while (i < jumps)
	{
		*(ptr + i) = c;
		i++;
	}
}
