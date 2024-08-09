/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcaliqui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:56:25 by dcaliqui          #+#    #+#             */
/*   Updated: 2024/07/24 17:56:30 by dcaliqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "includes/libft.h"

t_string	*g_mensagem = NULL;

void	adicionar_caractere_na_lista(char c);
void	imprimir_e_limpar(void);
void	ft_btoa(int sinal, siginfo_t *info, void *contexto);

void	adicionar_caractere_na_lista(char c)
{
	t_string	*novo_no;
	t_string	*temp;

	novo_no = (t_string *)malloc(sizeof(t_string));
	if (!novo_no)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	novo_no->c = c;
	novo_no->next = NULL;
	if (!g_mensagem)
		g_mensagem = novo_no;
	else
	{
		temp = g_mensagem;
		while (temp->next)
			temp = temp->next;
		temp->next = novo_no;
	}
}

void	imprimir_e_limpar(void)
{
	t_string	*temp;
	t_string	*para_liberar;

	temp = g_mensagem;
	while (temp)
	{
		write(1, &(temp->c), 1);
		para_liberar = temp;
		temp = temp->next;
		free(para_liberar);
	}
	g_mensagem = NULL;
	write(1, "\n", 1);
}

void	ft_btoa(int sinal, siginfo_t *info, void *contexto)
{
	static unsigned char		c = 0;
	static int					bit = 7;

	(void)contexto;
	if (sinal == SIGUSR2)
		c |= (1 << bit);
	bit--;
	if (bit < 0)
	{
		if (c == '\0')
			imprimir_e_limpar();
		else
			adicionar_caractere_na_lista(c);
		bit = 7;
		c = 0;
	}
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	printf("PID do servidor: <%d>\n", getpid());
	sa.sa_sigaction = ft_btoa;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
	return (0);
}
