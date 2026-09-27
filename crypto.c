/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   crypto.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wribeiro <wribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 16:53:35 by wribeiro          #+#    #+#             */
/*   Updated: 2026/09/05 18:22:18 by wribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

uint64_t	generate_random_key(void)
{
	uint64_t	key;
	int			fd;

	key = 0;
	fd = open("/dev/urandom", O_RDONLY);
	if (fd < 0)
	{
		perror("Warning: /dev/urandom failed");
		key = 0x0123456789ABCDEF;
	}
	else
	{
		read(fd, &key, sizeof(key));
		close(fd);
	}
	printf("key_value: %016lX\n", key);
	return (key);
}

static void	encrypt_remainder(t_woody *woody, uint64_t key, size_t i,
	uint64_t *ptr8)
{
	char	*ptr1;
	char	*key_ptr;
	size_t	j;

	ptr1 = (char *)ptr8;
	key_ptr = (char *)&key;
	j = 0;
	while (i < woody->text_section->sh_size)
	{
		*ptr1 ^= key_ptr[j];
		ptr1++;
		i++;
		j++;
	}
}

void	encrypt_text_section(t_woody *woody, uint64_t key)
{
	uint64_t	*ptr8;
	size_t		i;

	ptr8 = (uint64_t *)((char *)woody->md + woody->text_section->sh_offset);
	i = 0;
	while (i + 8 <= woody->text_section->sh_size)
	{
		*ptr8 ^= key;
		ptr8++;
		i += 8;
	}
	if (i < woody->text_section->sh_size)
		encrypt_remainder(woody, key, i, ptr8);
	printf("=> Section .text successfully obfuscated in memory!\n");
}
