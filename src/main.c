/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wribeiro <wribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 13:37:39 by wribeiro          #+#    #+#             */
/*   Updated: 2026/09/05 19:27:08 by wribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*p;

	p = (unsigned char *)s;
	while (n > 0)
	{
		*p = 0;
		p++;
		n--;
	}
}

int	cleanup_exit(t_woody *woody, int exit_code)
{
	if (woody->md && woody->fs > 0)
		munmap(woody->md, woody->fs);
	if (woody->fd >= 0)
		close(woody->fd);
	if (woody->stub_code)
		free(woody->stub_code);
	return (exit_code);
}

int	main(int ac, char **av)
{
	t_woody		woody;
	uint64_t	key;

	ft_bzero(&woody, sizeof(t_woody));
	if (ac != 2)
	{
		printf("Usage: %s <binary_file>/n", av[0]);
		return (1);
	}
	if (init_and_map_file(&woody, av[1]) != 0)
		return (1);
	if (validate_elf_headers(&woody) != 0)
		return (cleanup_exit(&woody, 1));
	if (find_elf_targets(&woody) != 0)
		return (cleanup_exit(&woody, 1));
	key = generate_random_key();
	encrypt_text_section(&woody, key);
	if (load_stub(&woody) != 0)
		return (cleanup_exit(&woody, 1));
	patch_stub(&woody, key);
	if (inject_payload(&woody) != 0)
		return (cleanup_exit(&woody, 1));
	if (write_woody_file(&woody) != 0)
		return (cleanup_exit(&woody, 1));
	return (cleanup_exit(&woody, 0));
}
