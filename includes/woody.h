/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   woody.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wribeiro <wribeiro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/05 14:43:06 by wribeiro          #+#    #+#             */
/*   Updated: 2026/09/05 19:24:24 by wribeiro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOODY_H
# define WOODY_H
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/mman.h>
# include <elf.h>

typedef struct s_woody
{
	int			fd;
	off_t		fs;
	void		*md;
	Elf64_Ehdr	*ehdr;
	Elf64_Phdr	*phdr;
	Elf64_Shdr	*shdr;
	char		*string_table;
	Elf64_Shdr	*text_section;
	Elf64_Phdr	*exec_segment;
	void		*stub_code;
	size_t		stub_size;
}	t_woody;

int			ft_strcmp(const char *s1, const char *s2);
int			init_and_map_file(t_woody *woody, char *filename);
int			validate_elf_headers(t_woody *woody);
int			find_elf_targets(t_woody *woody);
int			error_close(t_woody *woody, char *msg);
uint64_t	generate_random_key(void);
void		encrypt_text_section(t_woody *woody, uint64_t key);
int			write_woody_file(t_woody *woody);
int			load_stub(t_woody *woody);
void		patch_stub(t_woody *woody, uint64_t key);
int			inject_payload(t_woody *woody);

#endif