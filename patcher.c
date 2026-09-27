/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   patcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wribeiro <wribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 18:28:34 by wribeiro          #+#    #+#             */
/*   Updated: 2026/09/06 10:26:07 by wribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	load_stub(t_woody *woody)
{
	int	fd;

	fd = open("asm/stub.bin", O_RDONLY);
	if (fd < 0)
		return (error_close(woody, "Error: Could not open stub.bin"));
	woody->stub_size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	woody->stub_code = malloc(woody->stub_size);
	if (!woody->stub_code)
	{
		close(fd);
		return (1);
	}
	read(fd, woody->stub_code, woody->stub_size);
	close(fd);
	return (0);
}

void	patch_stub(t_woody *woody, uint64_t key)
{
	size_t		i;
	uint64_t	*ptr;
	size_t		cave_vaddr;
	long		diff_text;
	long		diff_oep;

	cave_vaddr = woody->exec_segment->p_vaddr + woody->exec_segment->p_memsz;
	diff_text = (long)woody->text_section->sh_addr - (long)cave_vaddr;
	diff_oep = (long)woody->ehdr->e_entry - (long)cave_vaddr;
	i = 0;
	while (i <= woody->stub_size - 8)
	{
		ptr = (uint64_t *)((char *)woody->stub_code + i);
		if (*ptr == 0x1111111111111111)
			*ptr = key;
		else if (*ptr == 0x2222222222222222)
			*ptr = woody->text_section->sh_size;
		else if (*ptr == 0x3333333333333333)
			*ptr = (uint64_t)diff_text;
		else if (*ptr == 0x4444444444444444)
			*ptr = (uint64_t)diff_oep;
		i++;
	}
	printf("=> Payload injected with real keys and offsets\n");
}

int	inject_payload(t_woody *woody)
{
	size_t	i;
	size_t	cave_offset;
	size_t	new_entry;
	char	*cave_ptr;

	cave_offset = woody->exec_segment->p_offset + woody->exec_segment->p_filesz;
	new_entry = woody->exec_segment->p_vaddr + woody->exec_segment->p_memsz;
	if (cave_offset + woody->stub_size > (size_t)woody->fs)
	{
		printf("Error: Not enough space in the code cave\n");
		return (1);
	}
	woody->ehdr->e_entry = new_entry;
	woody->exec_segment->p_filesz += woody->stub_size;
	woody->exec_segment->p_memsz += woody->stub_size;
	woody->exec_segment->p_flags |= PF_W;
	cave_ptr = (char *)woody->md + cave_offset;
	i = 0;
	while (i < woody->stub_size)
	{
		cave_ptr[i] = ((char *)woody->stub_code)[i];
		i++;
	}
	printf("=> Stub successfully injected into the code cave\n");
	return (0);
}
