#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

typedef struct secToFirst
{
    int first_sock;
    int second_sock;
    char *buf;
} args_t;

void *thread_funk(void *arguments)
{
    int m, n;
    args_t *args = (args_t *)arguments;
    int newsockfd1 = args->first_sock;
    int newsockfd2 = args->second_sock;
    char *buffer2 = args->buf;
    while (1)
    {
        //if (buffer2[0] != 0)
        //{
            m = recv(newsockfd2, buffer2, 255, 0); //чтение данных из сокета newsockfd1 в буфер buffer1 с размером 255 без флагов
            if (n < 0)
                perror("ERROR reading from socket");

            n = send(newsockfd1, buffer2, 255, 0); //отправляем сообщение
            if (n < 0)
                perror("ERROR writing to socket");
            bzero(buffer2, 255);
        //}
    }
}

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd1, sockfd2 //этот сокет для соединения
        ,
        newsockfd1, newsockfd2 //этот создается при соединении
        ,
        portno1, portno2;
    socklen_t clilen1, clilen2;
    char buffer1[256], buffer2[256];
    struct sockaddr_in serv_addr1, serv_addr2, cli_addr1, cli_addr2;
    int n, m, status;
    pthread_t* tid = (pthread_t*)malloc(sizeof(pthread_t)*2);
    if (argc < 3)
    {
        fprintf(stderr, "ERROR, no port provided\n");
        exit(1);
    }
    sockfd1 = socket(AF_INET, SOCK_STREAM, 0); //инициализируем сокет
    sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd1 < 0)
        error("ERROR opening socket1");
    if (sockfd2 < 0)
        error("ERROR opening socket2");
    bzero((char *)&serv_addr1, sizeof(serv_addr1)); //чистим структуру адреса сервера
    bzero((char *)&serv_addr2, sizeof(serv_addr2));
    portno1 = atoi(argv[1]); //порт, который слушаем
    portno2 = atoi(argv[2]);

    //заполняем структуру адреса сервера
    serv_addr1.sin_family = AF_INET;         //домен - интернет
    serv_addr1.sin_addr.s_addr = INADDR_ANY; //универсальный адрес
    serv_addr1.sin_port = htons(portno1);    //узловой порядок байт в сетевой (хз)

    serv_addr2.sin_family = AF_INET;
    serv_addr2.sin_addr.s_addr = INADDR_ANY;
    serv_addr2.sin_port = htons(portno2);

    if (bind(sockfd1, (struct sockaddr *)&serv_addr1, sizeof(serv_addr1)) < 0) //связываем сокет с адресом сервера
        error("ERROR on binding");
    if (bind(sockfd2, (struct sockaddr *)&serv_addr2, sizeof(serv_addr2)) < 0) //связываем сокет с адресом сервера
        error("ERROR on binding");

    listen(sockfd1, 5); //перевод сокета в режим приема с размером очереди 5
    listen(sockfd2, 5);

    clilen1 = sizeof(cli_addr1); //размер структуры адреса клиента
    clilen2 = sizeof(cli_addr2);

    newsockfd1 = accept(sockfd1, (struct sockaddr *)&cli_addr1, &clilen1); //создание нового сокета для обмена
    if (newsockfd1 < 0)
        error("ERROR on accept");

    newsockfd2 = accept(sockfd2, (struct sockaddr *)&cli_addr2, &clilen2);
    if (newsockfd2 < 0)
        error("ERROR on accept");
    bzero(buffer1, 256); //сбрасываем буфер
    bzero(buffer2, 256);

    args_t *args1_2 = (args_t *)malloc(sizeof(args_t));
    args_t *args2_1 = (args_t *)malloc(sizeof(args_t));

    args1_2->first_sock = newsockfd2;
    args1_2->second_sock = newsockfd1;
    args1_2->buf = &buffer1[0];

    args2_1->first_sock = newsockfd1;
    args2_1->second_sock = newsockfd2;
    args2_1->buf = &buffer2[0];

    status = pthread_create(&tid[0], NULL, thread_funk, args1_2);
    status = pthread_create(&tid[1], NULL, thread_funk, args2_1); //2->1


    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    free(tid);
    free(args1_2);
    free(args2_1);

    close(newsockfd1); //закрываем сокеты
    close(newsockfd2);
    close(sockfd1);
    close(sockfd2);
    return 0;
}