/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbohmert <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/01/24 20:59:49 by rbohmert          #+#    #+#             */
/*   Updated: 2016/03/26 22:48:58 by rbohmert         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_realloc(char *buf, int size)
{
	char	*new;
	int		i;

	i = -1;
	if (!(new = (char *)malloc(size + 1)))
		return (NULL);
	ft_bzero(new, size + 1);
	while (buf[++i] != 0)
		new[i] = buf[i];
	free(buf);
	return (new);
}

int		get_line(int fd, char **line, char *rest, int i)
{
	char	*buf;
	char	*pn;
	int		ret;
	int		ret2;

	if (!(buf = (char *)malloc(BUFF_SIZE + 1)))
		return (-1);
	ft_bzero(buf, BUFF_SIZE + 1);
	ft_bzero(rest, BUFF_SIZE);
	if ((ret = read(fd, buf, BUFF_SIZE)) == -1)
		return (-1);
	ret2 = ret;
	while (!(pn = ft_strchr(buf, '\n')) && ret > 0 && ++i >= 0)
	{
		buf = ft_realloc(buf, BUFF_SIZE * (i + 1));
		ret = read(fd, buf + (BUFF_SIZE * i), BUFF_SIZE);
	}
	if (ret > 0)
	{
		ft_bzero(rest, BUFF_SIZE);
		ft_strncpy(rest, pn + 1, BUFF_SIZE - ((pn - buf + 1) % BUFF_SIZE));
		ft_bzero(pn, BUFF_SIZE - ((pn - buf) % BUFF_SIZE));
	}
	*line = buf;
	return (ret2 > 0 ? 1 : ret);
}

int		sort(int fd, char **line, char *rest)
{
	char	*pn;
	char	*buf;
	char	*tmp;
	char	*tmpr;
	int		ret;

	if ((pn = ft_strchr(rest, '\n')))
	{
		if (!(buf = (char *)malloc(BUFF_SIZE + 1)))
			return (-1);
		ft_bzero(buf, BUFF_SIZE + 1);
		ft_strncpy(buf, rest, pn - rest);
		ft_memmove(rest, pn + 1, BUFF_SIZE - (pn - rest + 1));
		*line = buf;
		return (1);
	}
	tmpr = ft_strdup(rest);
	ret = get_line(fd, &tmp, rest, 0);
	*line = ft_strjoin(tmpr, tmp);
	free(tmpr);
	free(tmp);
	return (ret);
}

int		get_next_line(int fd, char **line)
{
	static t_list *list;
	t_list *tmp;
	char	*str;
	int ret;

	ft_push_back(&list, (void *)0, 0);
	if (!(line))
		return (-1);
	if (fd < 0)
		return (-1);
	tmp = list->next;
	while (tmp && fd != (int)tmp->content_size)
		tmp = tmp->next;
	if (!tmp)
	{
		if (!(str = (char *)malloc(BUFF_SIZE)))
			return (-1);
		ft_push_back(&list, str, fd);
		ret = get_line(fd, line, str, 0);
	}
	else
		ret = sort(fd, line, tmp->content);
	return ((ret >= 0 && (**line)) ? 1 : ret);
}
