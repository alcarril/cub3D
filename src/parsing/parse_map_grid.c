/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map_grid.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/25 08:54:07 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

int	validate_map_dimensions(t_carbon_parse *data, t_carbon_map *map,
		int start_line)
{
	int	i;

	i = start_line;
	while (data->file_content[i])
		i++;
	map->height = i - start_line;
	if (map->height == 0)
	{
		printf("Error: No map found\n");
		return (0);
	}
	map->width = find_max_width(data->file_content, start_line);
	if (map->width == 0)
	{
		printf("Error: Map width (%d) invalid\n", map->width);
		return (0);
	}
	return (1);
}

int	parse_map_grid(t_carbon_parse *data, t_carbon_map *map, int start_line)
{
	int	i;
	int	j;

	if (!data || !map || !data->file_content)
		return (0);
	if (!validate_map_dimensions(data, map, start_line))
		return (0);
	map->map_grids = allocate_map_grid(map->height, map->width);
	if (!map->map_grids)
	{
		printf("Error: Failed to allocate memory for map grid\n");
		return (0);
	}
	j = 0;
	i = start_line;
	while (data->file_content[i] && j < map->height)
	{
		normalize_map_line_to_grid(data->file_content[i],
			map->map_grids[j], map->width);
		if (!find_player_position(map->map_grids[j], map, j))
			return (0);
		i++;
		j++;
	}
	return (1);
}

void	normalize_map_line_to_grid(char *line, char *grid_row, int width)
{
	int	i;
	int	len;

	if (!line || !grid_row || width <= 0)
		return ;
	len = ft_strlen(line);
	i = 0;
	while (i < width)
	{
		if (i < len)
			grid_row[i] = line[i];
		else
			grid_row[i] = ' ';
		i++;
	}
	grid_row[width] = '\0';
}

char	*normalize_map_line(char *line, int width)
{
	char	*normalized;
	int		i;
	int		len;

	if (!line || width <= 0)
		return (NULL);
	normalized = ft_calloc(width + 1, sizeof(char));
	if (!normalized)
		return (NULL);
	len = ft_strlen(line);
	i = 0;
	while (i < width)
	{
		if (i < len)
			normalized[i] = line[i];
		else
			normalized[i] = ' ';
		i++;
	}
	return (normalized);
}

int	find_max_width(char **content, int start_line)
{
	int	max_width;
	int	current_width;
	int	i;

	if (!content)
		return (0);
	max_width = 0;
	i = start_line;
	while (content[i])
	{
		current_width = ft_strlen(content[i]);
		if (current_width > max_width)
			max_width = current_width;
		i++;
	}
	return (max_width);
}
