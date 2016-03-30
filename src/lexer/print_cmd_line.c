/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmd_line.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/30 17:45:17 by alelievr          #+#    #+#             */
/*   Updated: 2016/03/30 18:12:32 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

inline static const char	*get_verbose_operate(enum e_operate e)
{
	if (SWITCH(e))
	{
		CASE(OP_PIPE);
		return ("pipe");
		CASE(OP_BIN);
		return ("bin");
		CASE(OP_REDIR_R);
		return ("right redirection");
		CASE(OP_REDIR_L);
		return ("left redirection");
		CASE(OP_DREDIR_L);
		return ("double left redirection");
		CASE(OP_DREDIR_R);
		return ("double right redirection");
		CASE(OP_SEMICOLON);
		return ("semicolon");
		CASE(OP_OR);
		return ("or");
		CASE(OP_AND);
		return ("and");
		CASE(OP_NO_OP);
		return ("no op");
	}
	return ("");
}

static void					print_redirs(t_operator *o)
{
	t_redirection	*r;
	size_t			i;

	r = (t_redirection *)o->redirs;
	i = -1;
	while (r[++i].type != OP_NO_OP)
	{
		if (r[i].file)
			ft_printf("%s -> %s\n", get_verbose_operate(r[i].type), r[i].file);
		else
			ft_printf("%s -> %i\n", get_verbose_operate(r[i].type), r[i].fd);
	}
}

static void					print_operators(t_operator *o)
{
	size_t		i;

	ft_printf("binary: |%s|\n", o->bin);
	i = -1;
	while (o->av_bin[++i])
		ft_printf("av[%i] = %s\n", i, o->av_bin[i]);
	ft_printf("ac = %i\n", o->ac_bin);
	print_redirs(o);
}

static void					print_command(t_command *c)
{
	while (c)
	{
		print_operators(c->list);
		if (c->next)
			ft_printf("pipe\n");
		if (c->background)
			ft_printf("in background\n");
		c = c->next;
	}
}

void						print_cmd_line(t_commandline *c)
{
	while (c)
	{
		print_command(c->command);
		if (c->type != OP_NO_OP)
			ft_printf("%s\n", get_verbose_operate(c->type));
		c = c->next;
	}
}