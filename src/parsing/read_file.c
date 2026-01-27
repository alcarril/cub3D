/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: carbon-m <carbon-m@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/24 00:00:00 by alejandro         #+#    #+#             */
/*   Updated: 2026/01/15 13:58:55 by carbon-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/carbon.h"

int	read_and_process_lines(t_carbon_parse *data)
{
	int	i;

	i = 0;
	data->line = get_next_line(data->fd);
	while (data->line != NULL)
	{
		data->file_content[i] = ft_strtrim(data->line, "\n");
		free(data->line);
		if (!data->file_content[i])
			return (0);
		i++;
		data->line = get_next_line(data->fd);
	}
	return (1);
}

int	read_file_content(t_carbon_parse *data)
{
	int	lines;

	if (!data || !data->filename)
		return (0);
	data->fd = open(data->filename, O_RDONLY);
	if (data->fd < 0)
		return (0);
	lines = count_file_lines(data->fd);
	if (lines == 0)
		return (0);
	data->file_content = ft_calloc(lines + 1, sizeof(char *));
	if (!data->file_content)
		return (0);
	close(data->fd);
	data->fd = open(data->filename, O_RDONLY);
	if (data->fd < 0)
		return (0);
	return (read_and_process_lines(data));
}

int	process_file_lines(int fd)
{
	char	*line;
	int		count;

	count = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		count++;
		free(line);
		line = get_next_line(fd);
	}
	return (count);
}

int	count_file_lines(int fd)
{
	if (fd < 0)
		return (0);
	return (process_file_lines(fd));
}
