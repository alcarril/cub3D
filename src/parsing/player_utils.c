/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/25 08:54:12 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

int	check_multiple_players(t_carbon_map *map)
{
	if (map->player_found)
	{
		printf("Error: Multiple players found\n");
		return (0);
	}
	return (1);
}

void	set_player_data(t_carbon_map *map, int i, int y, char direction)
{
	map->player.x = i + 0.5;
	map->player.y = y + 0.5;
	map->player.direction = direction;
	set_player_angle(&map->player);
	map->player_found = 1;
}

int	find_player_position(char *line, t_carbon_map *map, int y)
{
	int	i;

	if (!line || !map)
		return (1);
	i = 0;
	while (line[i])
	{
		if (line[i] == 'N' || line[i] == 'S'
			|| line[i] == 'E' || line[i] == 'W')
		{
			if (!check_multiple_players(map))
				return (0);
			set_player_data(map, i, y, line[i]);
			line[i] = '0';
		}
		i++;
	}
	return (1);
}

void	set_player_angle(t_carbon_player *player)
{
	if (!player)
		return ;
	if (player->direction == 'N')
		player->angle = 90.0;
	else if (player->direction == 'S')
		player->angle = 270.0;
	else if (player->direction == 'E')
		player->angle = 0.0;
	else if (player->direction == 'W')
		player->angle = 180.0;
}
