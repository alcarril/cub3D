/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_elements.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/26 00:46:55 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

int	process_element_line(char *line, t_carbon_parse *data,
	t_carbon_map *map, int i)
{
	if (is_empty_line(line))
		return (1);
	if (is_texture_line(line))
	{
		if (!parse_texture(line, map))
			return (0);
		data->texture_count++;
		return (1);
	}
	if (is_color_line(line))
	{
		if (!parse_color(line, map))
			return (0);
		data->color_count++;
		return (1);
	}
	if ((data->texture_count == 4 || data->texture_count == 5)
		&& data->color_count == 2)
	{
		data->map_started = 1;
		return (parse_map_grid(data, map, i));
	}
	printf("Error: Invalid line: %s\n", line);
	return (0);
}

int	validate_element_counts(t_carbon_parse *data)
{
	if ((data->texture_count < 4 || data->texture_count > 5)
		|| data->color_count != 2)
	{
		printf("Error: Missing elements (textures: %d/4-5, colors: %d/2)\n",
			data->texture_count, data->color_count);
		return (0);
	}
	return (1);
}

int	parse_elements(t_carbon_parse *data, t_carbon_map *map)
{
	int	i;
	int	result;

	if (!data || !map || !data->file_content)
		return (0);
	i = 0;
	while (data->file_content[i] && !data->map_started)
	{
		result = process_element_line(data->file_content[i], data, map, i);
		if (result == 0)
			return (0);
		if (data->map_started)
			return (result);
		i++;
	}
	return (validate_element_counts(data));
}
