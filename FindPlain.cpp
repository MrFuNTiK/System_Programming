#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct node
{
	int data;
	struct node *next;
   	struct node *prev;
} node;
typedef node *list;
list head=NULL;
list tail=NULL;

void init_list(list *root)
{ 
	(*root)=NULL;
}

int append(list* lst, int value)
{   
	if ((*lst)==NULL)
	{
		list tmp=(list)malloc(sizeof(node));
		tmp->next=NULL;
		tmp->data=value;
		tmp->prev=NULL;
		head=tmp;
		tail=tmp;
		return 1;
	}
	list tmp=(list)malloc(sizeof(node));
	tmp->data=value;
	tmp->prev=(*lst);
	if ((*lst)==tail)
        {
		tmp->next=NULL;
		((*lst)->next)=tmp;
		tail=(*lst)->next;
	}
	else
        {
            tmp->next=((*lst)->next);
            ((*lst)->next)->prev=tmp;
            ((*lst)->next)=tmp;
        }
	return 1;
}

void print(list *lst)
{
	while((*lst)!=NULL)
        {
		printf("%d", (*lst)->data);
		printf(" ");
		lst=&((*lst)->next);
        }
	printf("\n");
}

void findPlains(int low, int high)
{
    bool plain = true;
    int j = 2;
	for (int i = low; i <= high; i++)
	{
		while(j <= sqrt(i))
        {
            if ((i % j) == 0)
            {
                if (j != 1 && j != i)
                plain = false;
            }
            j++;
        }
        if (i != 0 && i != 1)
        {
            if (plain == true)
            {
                append(&tail, i);
            }
        }
        j = 2;
        plain = true;
	}
}

int to_int(std::string str)
{
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i)
        res = res*10 + str[i] - '0';
    return res;
}


int main(int argc, char* argv[])
{
	int low = to_int(argv[1]);
    int high = to_int(argv[2]);
	//std::cout << argv[1] << " " << argv[2];
	pid_t pid = getpid(), parent_pid = getppid();
	std::cout << "    CHILD:\n";
	std::cout << "    My pid: " << pid << "\n";
	std::cout << "    My parent's pid: " << parent_pid << "\n";
	std::cout << "    My bounds: " << low << " " << high << "\n";
	init_list(&head);
	findPlains(low, high);
	printf("    ");
	print(&head);
    return 0;
}