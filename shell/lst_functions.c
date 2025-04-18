#include "minishell.h"

t_token	*ft_token_new(char *content)
{
	t_token	*new_node;

	new_node = (t_token *)malloc(sizeof(t_token));
	if (!new_node)
		return (NULL);
	new_node->content = content;
	new_node->next = NULL;
	return (new_node);
}

void	ft_token_add_back(t_token **lst, t_token *new)
{
	t_token	*head;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
	{
		*lst = new;
		return ;
	}
	head = *lst;
	while (head->next)
		head = head->next;
	head->next = new;
}

void	ft_token_delone(t_token *lst, void (*del)(void *))
{
	if (!lst || !del)
		return ;
	del(lst->content);
	free(lst);
}

void	ft_token_clear(t_token **lst, void (*del)(void *))
{
	t_token	*tmp;
	t_token	*nextnode;

	if (!lst || !del)
		return ;
	tmp = *lst;
	while (tmp)
	{
		nextnode = tmp->next;
		ft_token_delone(tmp, del);
		tmp = nextnode;
	}
	*lst = NULL;
}
