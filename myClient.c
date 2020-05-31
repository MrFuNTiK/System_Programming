#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

typedef struct listen_args
{
    int sock;
    char *buf;
} args_t;

void *thread_listen_funk(void *arguments)
{
    args_t *args = (args_t *)arguments;
    int sockfd = args->sock;
    char *buffer = args->buf;
    while (1)
    {
        int m = recv(sockfd, buffer, 255, 0); //получаем
        if (m < 0)
            error("ERROR reading from socket");
        printf("%s\n", buffer);
        bzero(buffer, 256);
    }
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n, m, menu;
    struct sockaddr_in serv_addr; //структура адреса сокета в интернете
    struct hostent *server;

    pthread_t listen_tid;
    int status;

    char buffer[256];
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]); //порт, который слушаем
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]); //получаем информацию о машине в сети
    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    //заполняем структуру адреса сервера
    bzero((char *)&serv_addr, sizeof(serv_addr));                                        //чистим структуру адреса сервера
    serv_addr.sin_family = AF_INET;                                                      //указываем домен
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length); //копируем область памяти длиной serv->len по первому адресу во второй адрес
    serv_addr.sin_port = htons(portno);                                                  //узловой порядок байт в сетевой (хз)
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)           //подключаемся к серверу
        error("ERROR connecting");

    args_t *args = (args_t *)malloc(sizeof(args_t));
    args->buf = &buffer[0];
    args->sock = sockfd;
    status = pthread_create(&listen_tid, NULL, thread_listen_funk, args); //запускаем поток на вывод входящих

    while (1)
    {
        printf("Enter 1 to write a message\n");
        printf("Enter 0 to exit\n");
        scanf("%d", &menu);
        switch (menu)
        {
        case 1:
        {
            printf("Please enter the message: ");
            bzero(buffer, 256); //очищаем буфер
            scanf("%c", buffer);
            fgets(buffer, 255, stdin);        //заполняем буфер из stdin
            n = send(sockfd, buffer, 255, 0); //отправляем
            if (n < 0)
                error("ERROR writing to socket");
            bzero(buffer, 256);
            break;
        }
        case 0:
            free(args);
            close(sockfd);
            return 0;
        default:
            printf("Try again\n");
        }
    }
    close(sockfd);
    return 0;
}