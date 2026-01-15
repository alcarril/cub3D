/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/15 13:58:55 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

static int	g_player_found = 0;

int	check_multiple_players(void)
{
	if (g_player_found)
	{
		printf("Error: Multiple players found\n");
		return (0);
	}
	return (1);
}

void	set_player_data(t_player *player, int i, int y, char direction)
{
	player->x = i + 0.5;
	player->y = y + 0.5;
	player->direction = direction;
	set_player_angle(player);
	g_player_found = 1;
}

int	find_player_position(char *line, t_player *player, int y)
{
	int	i;

	if (!line || !player)
		return (1);
	i = 0;
	while (line[i])
	{
		if (line[i] == 'N' || line[i] == 'S'
			|| line[i] == 'E' || line[i] == 'W')
		{
			if (!check_multiple_players())
				return (0);
			set_player_data(player, i, y, line[i]);
			line[i] = '0';
		}
		i++;
	}
	return (1);
}

void	set_player_angle(t_player *player)
{
	if (!player)
		return ;
	if (player->direction == 'N')
		player->angle = 3 * M_PI / 2;
	else if (player->direction == 'S')
		player->angle = M_PI / 2;
	else if (player->direction == 'E')
		player->angle = 0;
	else if (player->direction == 'W')
		player->angle = M_PI;
}
