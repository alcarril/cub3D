/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/25 21:52:37 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

int	validate_map(t_carbon_map *map)
{
	if (!map)
		return (0);
	if (!validate_player_found(map))
		return (0);
	if (!validate_map_characters(map))
		return (0);
	if (!validate_map_walls(map))
		return (0);
	return (1);
}

int	validate_player_found(t_carbon_map *map)
{
	if (!map)
		return (0);
	if (map->player.direction == '\0')
	{
		printf("Error: No player found in map\n");
		return (0);
	}
	return (1);
}

int	validate_map_characters(t_carbon_map *map)
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
			if (map->map_grids[i][j] != '0' && map->map_grids[i][j] != '1'
				&& map->map_grids[i][j] != '2' && map->map_grids[i][j] != ' ')
			{
				printf("Error: Invalid character '%c' at (%d, %d)\n",
					map->map_grids[i][j], j, i);
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}
