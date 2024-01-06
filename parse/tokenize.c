/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junkim2 <junkim2@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 16:53:35 by junkim2           #+#    #+#             */
/*   Updated: 2024/01/06 19:36:40 by junkim2          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sep_by_space(t_token **list, char *str)
{
	int	start;
	int	end;
	int	quote;

	start = 0;
	end = 0;
	while (str[end])
	{
		quote = is_quote(str, end);
		if (quote != 0)
		{
			end++;
			while (str[end] && is_quote(str, end) == 0)
				end++;
		}
		else if (str[end] == ' ')
		{
			make_token(list, str, start, end);
			start = end + 1;
		}
		else if (is_double_operator(str[end], str[end + 1]))
		{
			make_token(list, str, start, end + 2);
			start = ++end + 1;
		}
		else if (is_operator(str[end + 1]) || (is_operator(str[end]) && !is_operator(str[end + 1])))
		{
			make_token(list, str, start, end + 1);
			start = end + 1;
		}
		end++;
	}
	make_token(list, str, start, end);
}

void	remove_quote(t_token **list)
{
	t_token	*cur;
	char	*new_str;
	char	qoute;
	char	*tmp;
	int		i;
	int		j;

	cur = *list;
	while (cur)
	{
		new_str = (char *)ft_calloc(ft_strlen(cur->str) + 1, sizeof(char));
		if (new_str == NULL)
			exit(EXIT_FAILURE);
		i = 0;
		j = 0;
		while (cur->str[j])
		{
			qoute = is_quote(cur->str, j);
			if (qoute != 0)
			{
				j++;
				while (is_quote(cur->str, j) == 0)
				{
					if (cur->str[j] == '\\' &&
					(cur->str[j + 1] == '\'' || cur->str[j + 1] == '\"'))
						j++;
					else
						new_str[i++] = cur->str[j++];
				}
			}
			else
				new_str[i++] = cur->str[j];
			j++;
		}
		tmp = cur->str;
		cur->str = new_str;
		free(tmp);
		cur = cur->next;
	}
}