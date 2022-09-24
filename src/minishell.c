/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yogun <yogun@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/18 18:45:59 by yogun             #+#    #+#             */
/*   Updated: 2022/09/24 16:57:39 by yogun            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// Greeting shell during startup
void init_shell()
{
    clear();
    printf("\n\n\n\n******************"
        "************************");
    printf("\n\n\n\t****MINISHELL****");
    printf("\n\n\t-USE AT YOUR OWN RISK-");
    printf("\n\n\n\n*******************"
        "***********************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    sleep(2);
    clear();
}

// This function does exactly what pwd does.
void printdir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("%s\n", cwd);
}

// This function frees the node from the memory.
void	ft_free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		if (env->value)
			free(env->value);
		env->value = NULL;
		free(env->key);
		env->key = NULL;
		env = env->next;
		tmp->next = NULL;
		free(tmp);
	}
}

// New node is being created under this function for env variable that has been sent.
t_env	*ft_new_env(char *str)
{
	t_env	*new;
	int		i;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	new->key = ft_substr(str, 0, i);
	new->value = ft_strdup(str + i + 1);
	new->next = NULL;
	return (new);
}

// Here I will take env and tokenize them with linked list.
t_env *tokenize_env(t_data *data)
{
	t_env	*env;
	t_env	*tmp;
	t_env	*tmp2;
	int		i;

	i = 0;
	env = NULL;
	tmp2 = NULL;
	while (data->env[i])
	{
		tmp = ft_new_env(data->env[i]);
		if (!tmp)
		{
			ft_free_env(env);
			write(2, "Error: Memory allocation has failed!\n", 36);
			exit(EXIT_FAILURE);
		}
		if (!env)
			env = tmp;
		else
			tmp2->next = tmp;
		tmp2 = tmp;
		i++;
	}
	return (env);
}



int	main(int argc, char **argv, char **envp)
{	
	t_data data;
	t_env *env;

	if (argc == 1)
	{
		init_shell();
		data.argv = argv;
		data.env = envp;
		//I will parse env variables 
		env = tokenize_env(&data);
		while(1)
		{
			data.cmd_line = readline("miniSH > ");
			data.cmd_line = ft_strtrim(data.cmd_line, " ");
			if(ft_strlen(data.cmd_line)>0)
				add_history(data.cmd_line);
			if(!ft_strcmp(data.cmd_line , "exit"))
				break;	
			else if(!ft_strcmp(data.cmd_line , "pwd"))		
				printdir();
			else if(!ft_strncmp(data.cmd_line , "echo", 4))	
				printf("%s\n",data.cmd_line+4);
			else if (!ft_strncmp(data.cmd_line, "cd", 2))
			{
				// according to rest of the cmd_line chdir function will work
				//chdir("some dir");
				printdir();
			}
			else if (!ft_strncmp(data.cmd_line, "env", 3))
			{
				while(env)
				{
					printf("%s", env->key);
					printf("=%s\n",env->value);
					env = env->next;
				}
			}
			else if (!ft_strncmp(data.cmd_line, "export", 6))
			{
				printf("export will be here\n");
			}
			else if (!ft_strncmp(data.cmd_line, "unset", 5))
			{
				printf("unset will be here\n");
			}
		}	
	}
	else
	{
		printf("Too many arguments.\n");
	}

		
	return 0;
}
