/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_and_echo.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kcatrix <kcatrix@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 14:13:17 by kcatrix           #+#    #+#             */
/*   Updated: 2022/10/10 15:37:04 by kcatrix          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	cmd_echo(char **spli)
{
	int	i;
	int	id;
	int	in;
	char *tmp;
	
	i = 0;
	tmp = ft_strjoin_spli(spli);
	del_quote(tmp);
	spli = ft_split(tmp, ' ');
	free_protect(tmp);
	i = 1;
	pipe(g_stock.pip);
	in = g_stock.pip[0];
	id = fork();
	if (id == 0)
		fixechopip(in, spli, i);
	else
		fixwait(id);
	free_spli_protect(spli);
}

void	free_spli_protect(char **spli)
{
	int	i;

	i = 0;
	if (!spli )
		return ;
	while (spli[i])
	{
		free_protect(spli[i]);
		i++;
	}
	free(spli);
}

char *ft_strjoin_spli(char **spli)
{
	char	*tmp;
	char	*tmp2;
	int i;

	tmp2 = NULL;
	tmp = NULL;
	i = 0;
	tmp = ft_mallocex(spli[i], tmp);
	i++;
	while(spli[i])
	{
		tmp2 = ft_mallocex(tmp, tmp2);
		free_protect(tmp);
		tmp = ft_strjoin_spe(tmp2, spli[i]);
		free_protect(tmp2);
		i++;
	}
	return (tmp);
}

char *ft_strjoin_spe(char *s1, char *s2)
{
	char	*c;
	size_t	lena;
	size_t	lenb;
	size_t	i;
	size_t	j;
	size_t	y;

	if (!s1 || !s2)
		return (NULL);
	lena = strlen(s1);
	lenb = strlen(s2);
	c = (char *)malloc(sizeof(char) * (lena + (lenb + 2)));
	if (!c)
		return (NULL);
	i = 0;
	j = 0 - lena;
	y = 0;
	while (i < lena + (lenb + 1))
	{
		if (i == lena)
		{
			c[i] = 32;
			i++;
		}
		c[i] = s1[y];
		if (i >= lena)
			c[i] = s2[j];
		y++;
		j++;
		i++;
	}
	c[i] = '\0';
	return (c);
}

int	echo_option(char *spli)
{
	int	i;

	i = 1;
	if (spli[0] == '-')
	{
		while (spli[i])
		{
			if (spli[i] == 'n')
			{
				i++;
			}
			else
				return (1);
		}
		if (i == 1)
			return (1);
	}
	else
		return (1);
	return (0);
}

void	cmd_cd(char **spli)
{
	char	**unparun;

	if (g_stock.nbpip > 0)
	{
		fixcd();
		return ;
	}
	if (spli[1])
		unparun = ft_split(spli[1], '/');
	if (!spli[1])
	{
		cmd_cd_noarg();
		return ;
	}
	if (spli[1][0] == '/')
		cmd_cd_absolute(spli[1]);
	else
	{
		cmd_cd_boucle(unparun);
	}
	if (spli[1])
		free_spli(unparun);
}

char	**cmd_cd_dot(void)
{
	int	i;

	i = 0;
	while (g_stock.cpenv[i])
	{
		if (ft_memcmp(g_stock.cpenv[i], "OLDPWD=", 7) == 0 && g_stock.old == 0)
		{
			free(g_stock.cpenv[i]);
			g_stock.cpenv[i] = "OLDPWD=";
			g_stock.cpenv[i] = ft_strjoin(g_stock.cpenv[i], getpwd());
		}
		i++;
	}
	return (NULL);
}

char	*getpwd(void)
{
	int	i;
	int	j;

	i = 0;
	j = 4;
	while (g_stock.cpenv[i])
	{
		if (ft_memcmp(g_stock.cpenv[i], "PWD=", 4) == 0)
			return (g_stock.cpenv[i] + 4);
		i++;
	}
	return (NULL);
}
