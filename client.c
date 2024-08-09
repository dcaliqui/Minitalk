/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dcaliqui <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 17:56:02 by dcaliqui          #+#    #+#             */
/*   Updated: 2024/07/24 17:56:10 by dcaliqui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "includes/libft.h"

volatile sig_atomic_t	g_ack_recebido = 0;

void	manipulador_ack(int sinal);
void	enviar_caractere(pid_t pid, unsigned char c);
void	enviar_mensagem(pid_t pid, const char *mensagem);
void	tratar_erros(pid_t pid, char **argv);

void	manipulador_ack(int sinal)
{
	(void)sinal;
	g_ack_recebido = 1;
}

void	enviar_caractere(pid_t pid, unsigned char c)
{
	int	bit;

	bit = 7;
	while (bit >= 0)
	{
		g_ack_recebido = 0;
		if (c & (1 << bit))
			kill(pid, SIGUSR2);
		else
			kill(pid, SIGUSR1);
		while (!g_ack_recebido)
			usleep(100);
		bit--;
	}
}

void	enviar_mensagem(pid_t pid, const char *mensagem)
{
	while (*mensagem)
		enviar_caractere(pid, *mensagem++);
	enviar_caractere(pid, '\0');
}

void	tratar_erros(pid_t pid, char **argv)
{
	if (pid <= 0)
	{
		ft_putstr_fd(argv[1], 1);
		ft_putstr_fd(":PID inválido\n", 1);
		exit(EXIT_FAILURE);
	}
	if (kill(pid, 0) == -1)
	{
		ft_putstr_fd("Nenhum processo encontrado para o PID fornecido\n", 1);
		exit(EXIT_FAILURE);
	}
	if (argv[2][0] == '\0')
	{
		ft_putstr_fd("Mensagem vazia\n", 1);
		exit(EXIT_FAILURE);
	}
}

int	main(int argc, char **argv)
{
	pid_t				pid;
	struct sigaction	sa;

	if (argc != 3)
	{
		ft_putstr_fd("Uso:  <pid> <mensagem>\n", 1);
		exit(EXIT_FAILURE);
	}
	pid = (pid_t)atoi(argv[1]);
	tratar_erros(pid, argv);
	sa.sa_handler = manipulador_ack;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	ft_putstr_fd("Enviando mensagem:\n", 1);
	ft_putstr_fd(argv[2], 1);
	enviar_mensagem(pid, argv[2]);
	ft_putstr_fd("Mensagem enviada com sucesso\n", 1);
	return (0);
}	
