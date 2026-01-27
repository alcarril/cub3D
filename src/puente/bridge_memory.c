/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bridge_memory.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/26 00:00:00 by carbon-m          #+#    #+#             */
/*   Updated: 2026/01/26 01:20:56 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/cube3D.h"

int	is_floor_char(char c)
{
	return (c == '0' || c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

char	**alloc_alca(int height, int width)
{
	char	**grid;
	int		i;

	if (height <= 0 || width <= 0)
		return (NULL);
	grid = malloc(height * sizeof(char *));
	if (!grid)
		return (NULL);
	i = 0;
	while (i < height)
	{
		grid[i] = malloc(width * sizeof(char));
		if (!grid[i])
		{
			free_alcarril_grid(grid, i);
			return (NULL);
		}
		ft_bzero(grid[i], width);
		i++;
	}
	return (grid);
}

void	free_alcarril_grid(char **grid, int height)
{
	int	i;

	if (!grid)
		return ;
	i = 0;
	while (i < height)
	{
		if (grid[i])
			free(grid[i]);
		i++;
	}
	free(grid);
}

void	cleanup_bridge(t_parse_bridge *bridge)
{
	if (!bridge)
		return ;
	if (bridge->grid)
		free_map_grid(bridge->grid, bridge->height);
	bridge->grid = NULL;
}
