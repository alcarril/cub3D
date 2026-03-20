/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_colors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by alejandro         #+#    #+#             */
/*   Updated: 2026/03/19 18:38:01 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

char	*remove_inner_spaces(char *str)
{
	char	*clean;
	int		i;
	int		j;

	if (!str)
		return (NULL);
	clean = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!clean)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != ' ' && str[i] != '\t')
			clean[j++] = str[i];
		i++;
	}
	clean[j] = '\0';
	return (clean);
}

int	parse_color_values(char *rgb_clean, t_carbon_color *color)
{
	char	**rgb_split;
	int		result;

	rgb_split = ft_split(rgb_clean, ',');
	if (!rgb_split)
	{
		printf("Error: Invalid RGB format: %s\n", rgb_clean);
		return (0);
	}
	result = parse_rgb_values(rgb_split, color);
	free_split(rgb_split);
	return (result);
}

int	parse_color(char *line, t_carbon_map *map)
{
	char				*rgb_start;
	char				*rgb_raw;
	char				*rgb_clean;
	t_carbon_color		*color;

	if (!line || !map)
		return (0);
	if (!parse_color_header(line, map, &rgb_start, &color))
		return (0);
	rgb_raw = ft_strtrim(rgb_start, " \t");
	if (!rgb_raw)
		return (0);
	rgb_clean = remove_inner_spaces(rgb_raw);
	free(rgb_raw);
	if (!rgb_clean)
		return (0);
	if (!parse_color_values(rgb_clean, color))
		return (free(rgb_clean), 0);
	return (free(rgb_clean), 1);
}

int	is_color_line(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (0);
	while (line[i] == ' ')
		i++;
	if ((ft_strncmp(&line[i], "F ", 2) == 0)
		|| (ft_strncmp(&line[i], "C ", 2) == 0))
		return (1);
	return (0);
}

int	validate_rgb_values(t_carbon_color *color)
{
	if (!color)
		return (0);
	if (color->r < 0 || color->r > 255
		|| color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
		return (0);
	return (1);
}
