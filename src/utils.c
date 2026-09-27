/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wribeiro <wribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 15:11:40 by wribeiro          #+#    #+#             */
/*   Updated: 2026/09/05 19:09:01 by wribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "woody.h"

int	error_close(t_woody *woody, char *msg)
{
	perror(msg);
	if (woody->fd >= 0)
		close(woody->fd);
	return (1);
}

int	init_and_map_file(t_woody *woody, char *filename)
{	
	woody->fd = open(filename, O_RDONLY);
	if (woody->fd < 0)
		return (error_close(woody, "Error opening file"));
	woody->fs = lseek(woody->fd, 0, SEEK_END);
	if (woody->fs < 0)
		return (error_close(woody, "Error reading file size"));
	lseek(woody->fd, 0, SEEK_SET);
	if (woody->fs < (off_t) sizeof(Elf32_Ehdr))
	{
		printf("Error: File too small to be a valid ELF.\n");
		close(woody->fd);
		return (1);
	}
	woody->md = mmap(NULL, woody->fs, PROT_READ | PROT_WRITE,
			MAP_PRIVATE, woody->fd, 0);
	if (woody->md == MAP_FAILED)
		return (error_close(woody, "Error mapping file to memory"));
	return (0);
}

int	validate_elf_headers(t_woody *woody)
{
	woody->ehdr = (Elf64_Ehdr *)woody->md;
	if (woody->ehdr->e_ident[EI_MAG0] != ELFMAG0
		|| woody->ehdr->e_ident[EI_MAG1] != ELFMAG1
		|| woody->ehdr->e_ident[EI_MAG2] != ELFMAG2
		|| woody->ehdr->e_ident[EI_MAG3] != ELFMAG3)
	{
		printf("Error: The provided file is not an ELF binary.\n");
		munmap(woody->md, woody->fs);
		close(woody->fd);
		return (1);
	}
	if (woody->ehdr->e_ident[EI_CLASS] != ELFCLASS64)
	{
		printf("File architecture not supported. x86_64 only\n");
		munmap(woody->md, woody->fs);
		close(woody->fd);
		return (1);
	}
	printf("Success: 64-bit ELF file mapped safely into memory\n");
	woody->shdr = (Elf64_Shdr *)((char *)woody->md + woody->ehdr->e_shoff);
	woody->string_table = (char *)woody->md
		+ woody->shdr[woody->ehdr->e_shstrndx].sh_offset;
	return (0);
}

int	write_woody_file(t_woody *woody)
{
	int		fd;
	ssize_t	bytes_written;

	fd = open("woody", O_CREAT | O_WRONLY | O_TRUNC, 0755);
	if (fd < 0)
	{
		perror("Error creating woody file");
		return (1);
	}
	bytes_written = write(fd, woody->md, woody->fs);
	if (bytes_written < 0 || bytes_written != woody->fs)
	{
		perror("Error writing to woody file");
		close(fd);
		return (1);
	}
	close(fd);
	printf("=> File 'woody' created successfully!\n");
	return (0);
}
