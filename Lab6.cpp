#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>


pthread_mutex_t mutex;//объявляем мьютекс

typedef struct args_t{//структура для аргументов потока
    int low;//нижняя раница
    int high;//верхняя граница
}args_t;

typedef struct node//список
{
	int data;
	struct node *next;
   	struct node *prev;
} node;
typedef node *list;
list head=NULL;//начало списка
list tail=NULL;//конец списка

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
		//free(tmp);
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
	//free(tmp);
	return 1;
}

void print(list *lst)
{
	if ((*lst) == NULL)
	{
		printf ("No values\n");
	}
	while((*lst)!=NULL)
        {
		printf("%d", (*lst)->data);
		printf(" ");
		lst=&((*lst)->next);
        }
	printf("\n");
}

int destroy(list *root)
{   
	list x,y;
	x=*root;
	y=*root;
	while (x!=NULL)
	{
		y=x->next;
		free(x);
		x=y;
	}
	*root=NULL;
	return 1;
}

void* findPlains(void* arguments)
{
    args_t *args = (args_t*) arguments;
    int low = args->low;
    int high = args->high;
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
				pthread_mutex_lock(&mutex);//блокируем мьютекс перед добавлением в список
                append(&tail, i);
				pthread_mutex_unlock(&mutex);//освобождаем мьютекс
            }
        }
        j = 2;
        plain = true;
	}
    return NULL;
}


int main()
{
	pthread_mutex_init(&mutex, NULL);//инициализируем мьютекс
	unsigned int length = 20, ammount, low, high, last;
	pid_t pid = getpid();
	int status;
    init_list(&head);
	std::cout << "Enter low and high bound\n";
	std::cin >> low >> high;


	ammount = (high - low) / length;
	last = (high - low) % length;
	if (last != 0)
		ammount++;
	
    pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t)*ammount); 
	for (unsigned int i = 0; i < ammount; i++)
	{
		unsigned int first = i * length + low;
		unsigned int second = i * length + low + length - 1;
		if (second > high)
			second = high;
        args_t* args = (args_t*)malloc(sizeof(args_t));//создаем структуру и заполняем аргументы
        args->low = first;
        args->high = second;
		std::cout << "\nPARENT:\n" << "Trying to start child thread " << i+1 << "\n";
        status = pthread_create(&tid[i], NULL, findPlains, (void*)args);//создаем поток
        switch (status)
        {
        case 0:
			std::cout << "PID: " << pid << "\n" << "TID: " << tid[i] << "\n";
            break;
        
        default:
			free(args);
            break;
        }
	}
    for (unsigned int i = 0; i < ammount; i++)
    {
        pthread_join( tid[ i ], NULL);//синхронизируем потоки
    }
    print(&head);
	printf("That is all. I am exiting\n");
	pthread_mutex_destroy(&mutex);
	free (tid);
    return 0;
}