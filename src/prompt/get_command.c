/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/03/16 16:30:07 by alelievr          #+#    #+#             */
/*   Updated: 2015/03/18 17:55:35 by fdaudre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"

/*
 * UP
 * DOWN
 * LEFT
 * RIGHT
 * CTRL + LEFT
 * CTRL + RIGHT
 * HOME
 * END
 * CTRL + UP
 * SUPPR
*/

#define pr_move NULL
static t_pr_code		g_pr_codes[] =
{
	{4414235ull, pr_move}, // right
	{4479771ull, pr_move}, // left
	{74982532143899ull, pr_move}, // ctrl + left
	{73883020516123ull, pr_move}, // ctrl + left
	{4741915ull, pr_move}, // begin
	{1ull, pr_move}, // begin
	{4610843ull, pr_move}, // end
	{5ull, pr_move}, // end
	{0, NULL}
};

static void				pr_addchar(t_prompt *d)
{
	size_t				tmp;

	tmp = ft_strlen(d->buff);
	d->buff[tmp + 1] = '\0';
	while (tmp != d->index)
	{
		d->buff[tmp] = d->buff[tmp - 1];
		--tmp;
	}
	d->buff[tmp] = d->key;
	++(d->index);
}

static void				pr_affbuff(t_prompt *d)
{
	ft_putstr(tgetstr("cr", NULL));
	ft_putstr(tgetstr("cd", NULL));
//ft_printf("[%llu]", d->key);
	ft_printf("%{F}$> %{!F}", 123);
	ft_putstr(d->buff);
	ft_putstr(tgetstr("cr", NULL));
	ft_printf("\033[%luC", 3 + d->index);
}

char					*get_command(void)
{
	static t_prompt		d;
	int					i;

	raw_terminal_mode();
	d.index = 0;
	ft_bzero(d.buff, PR_BUF_SIZE);
	while (42)
	{
		pr_affbuff(&d);
		d.key = 0;
		if ((read(0, &(d.key), 8) < 1) || (d.key == 4) || (d.key == 10))
			break ;
		if (ft_isprint(d.key) && (d.key != '\t'))
			pr_addchar(&d);
		i = -1;
		while (g_pr_codes[++i].f != NULL)
			if (d.key == g_pr_codes[i].code)
			{
				g_pr_codes[i].f(&d);
				break ;
			}
	}
	default_terminal_mode();
	return ((d.key == 4) && (!d.index) ? NULL : (char *)d.buff);
}
