/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map_borders.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 00:00:00 by carbon-m         #+#    #+#             */
/*   Updated: 2026/01/15 00:00:00 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

int	check_floor_borders(t_carbon_map *map, int i, int j)
{
	if (i == 0 || i == map->height - 1 || j == 0 || j == map->width - 1)
	{
		printf("Error: Map not closed - floor at border (%d, %d)\n", j, i);
		return (0);
	}
	return (1);
}

int	check_floor_spaces(t_carbon_map *map, int i, int j)
{
	if ((map->map_grids[i - 1][j] == ' ') || (map->map_grids[i + 1][j] == ' ')
		|| (map->map_grids[i][j - 1] == ' ')
		|| (map->map_grids[i][j + 1] == ' '))
	{
		printf("Error: Map not closed - floor next to space at (%d, %d)\n",
			j, i);
		return (0);
	}
	return (1);
}

int	validate_map_walls(t_carbon_map *map)
{
	int	i;
	int	j;

	if (!map)
		return (0);
	i = 0;
	while (i < map->height)
	{
		j = 0;
		while (j < map->width)
		{
			if (map->map_grids[i][j] == '0')
			{
				if (!check_floor_borders(map, i, j))
					return (0);
				if (!check_floor_spaces(map, i, j))
					return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}
