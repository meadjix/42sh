/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pr_vim_export.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alelievr <alelievr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/03/17 22:50:41 by alelievr          #+#    #+#             */
/*   Updated: 2016/03/18 02:44:42 by alelievr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_42sh.h"
#include <fcntl.h>

static char		base64_char(int r)
{
	if (r >= 0 && r <= 9)
		return (r + '0');
	if (r >= 10 && r <= 36)
		return (r + 'A' - 10);
	if (r >= 37 && r <= 63)
		return (r + 'a' - 37);
	return ('_');
}

static char		*gen_rand_name(void)
{
	static char	buff[0xF00];
	int			i;
	int			n;

	ft_strcpy(buff, "/tmp/");
	i = -1;
	n = 5;
	while (++i < 8)
		buff[n + i] = base64_char(rand() % 64);
	buff[n + i] = 0;
	return (buff);
}

static void		exec_vim_file(char *fname)
{
	pid_t		pid;
	const char	*argv[] = {"vim", fname, NULL};
	char		*vim_path;

	if (!(vim_path = get_binhash_path(hashstring("vim"))))
		return ;
	if ((pid = fork()) == 0)
	{
		exit(execve(vim_path, (char **)argv, g_env));
	}
	if (pid > 0)
		waitpid(pid, NULL, 0);
}

static void		read_file_content(t_prompt *p, char *fname)
{
	int				ret;
	int				fd;

	if ((fd = open(fname, O_RDONLY)) != -1)
	{
		ret = read(fd, p->buff, sizeof(p->buff));
		p->index = 0;
		p->len = ret;
		p->buff[ret] = 0;
	}
}

void			pr_vim_export(t_prompt *p)
{
	size_t			wr;
	long			len;
	char			*fname;
	int				fd;

	fname = gen_rand_name();
	if ((fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644)) != -1)
	{
		len = ft_strlen(p->buff);
		wr = 0;
		while (len > 0)
		{
			write(fd, p->buff + wr, (len < 0xF000) ? len : 0xF000);
			wr += 0xF000;
			len -= 0xF000;
		}
		close(fd);
		exec_vim_file(fname);
		read_file_content(p, fname);
		unlink(fname);
	}
}