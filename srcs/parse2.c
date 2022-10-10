/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcatrix <kcatrix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 11:26:09 by kevyn             #+#    #+#             */
/*   Updated: 2022/10/10 16:40:35 by kcatrix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**parse(char **spli)
{
	int	i;

	i = 0;
	if (spli[0][0] == '/')
		spli[0] = ft_replace_absolute(spli);
	if (cote_simple(spli) == 0)
		return (spli);
	while (ft_checkdollar(spli) == 1)
	{
		while (spli[i])
		{
			if (spli[i][0] == '$' && spli[i][1] == '?' && spli[i][2] == '\0')
				spli = cmd_dollar_int(spli, i);
			else if ((spli[i][0] == '$') ||
					(spli[i][0] == '"' && spli[i][1] == '$'))
				spli = cmd_dollar(spli, i);
			else if (spli[i] != NULL)
				i++;
		}
		i = 0;
	}
	i = 0;
	spli = ft_checkdollar_inside(spli);
	return (spli);
}

int cote_simple(char **spli)
{
	int i;

	i = 0;
	while(spli[i])
	{
		if (spli[i][0] == '\'')
		{
			printf("cote find\n");
			return(0); 
		}
		i++;
	}
	return (1);
}

int	ft_checkdollar(char **spli)
{
	int	i;

	i = 0;
	while (spli[i])
	{
		if ((spli[i][0] == '$') ||
			(spli[i][0] == '"' && spli[i][1] == '$'))
			return (1);
		i++;
	}
	return (0);
}

int	verif_space(char *line)
{
	size_t	i;

	i = 0;
	while ((line[i] == 32) || (line[i] >= 9 && line[i] <= 13))
		i++;
	if (i == ft_strlen(line))
		return (0);
	return (1);
}

char	**cmd_dollar(char **spli, int y)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = ft_strjoin_spli(spli);
	free_spli(spli);
	del_quote(tmp);
	spli = ft_split(tmp, ' ');
	free(tmp);
	y = pos_dollard(spli);
	while (g_stock.cpenv[i])
	{
		if (ft_memcmp(g_stock.cpenv[i],
				spli[y] + 1, ft_strlen(spli[y]) - 1) == 0
			&& g_stock.cpenv[i][ft_strlen(spli[y]) - 1] == '=')
		{
			free(spli[y]);
			spli[y] = ft_mallocex(g_stock.cpenv[i]
					+ ft_strlen(spli[y]), spli[y]);
			return (spli);
		}
		i++;
	}
	printf("fini\n");
	spli = ft_sup_dollar(spli, y);
	return (spli);
}

int	pos_dollard(char **spli)
{
	int	i;

	i = 0;
	while(spli[i])
	{
		if(spli[i][0] == '$')
			return(i);
		i++;
	}
	return(i);
}

void	remove_backslash(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '"')
			skip(str, &i);
		if (str[i] == '\'')
			skip(str, &i);
		if (str[i] == '\\')
			rm_char(str, i--);
		i++;
	}
}
