/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wribeiro <wribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 16:11:00 by wribeiro          #+#    #+#             */
/*   Updated: 2026/09/05 17:33:53 by wribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

static void	find_text_section(t_woody *woody)
{
	int		i;
	char	*section_name;

	i = 0;
	while (i < woody->ehdr->e_shnum)
	{
		section_name = woody->string_table + woody->shdr[i].sh_name;
		if (ft_strcmp(section_name, ".text") == 0)
		{
			woody->text_section = &woody->shdr[i];
			printf("\n=> Section .text found!\n");
			printf("Offset in file: 0x%lx\n", woody->shdr[i].sh_offset);
			printf("Size: %lu bytes\n", woody->shdr[i].sh_size);
			printf("Virtual Address: 0x%lx\n", woody->shdr[i].sh_addr);
			break ;
		}
		i++;
	}
}

static void	find_exec_segment(t_woody *woody)
{
	int		i;

	woody->phdr = (Elf64_Phdr *)((char *)woody->md + woody->ehdr->e_phoff);
	i = 0;
	while (i < woody->ehdr->e_phnum)
	{
		if (woody->phdr[i].p_type == PT_LOAD
			&& (woody->phdr[i].p_flags & PF_X))
		{
			woody->exec_segment = &woody->phdr[i];
			printf("\n=> Executable Segment Found!\n");
			printf("Offset: 0x%lx\n", woody->phdr[i].p_offset);
			printf("Size in memory: %lu bytes\n", woody->phdr[i].p_memsz);
			break ;
		}
		i++;
	}
}

int	find_elf_targets(t_woody *woody)
{
	woody->text_section = NULL;
	woody->exec_segment = NULL;
	find_text_section(woody);
	find_exec_segment(woody);
	if (!woody->text_section || !woody->exec_segment)
	{
		printf("Error: Could not find required sections/segments.\n");
		return (1);
	}
	return (0);
}
