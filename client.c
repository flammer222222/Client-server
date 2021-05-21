#include <windows.h>
#include <stdio.h>
#include <strings.h>
//#include <locale.h>
#define PORT 3550
#define MAXDATASIZE 100

int main(int argc, char *argv[])
{
    //setlocale(LC_ALL, "Rus");
    WSADATA wsdata;
    WSAStartup(0x0101,&wsdata);
    int fd, numbytes,n,i;
    char buf[MAXDATASIZE];
    char buffer[22];
    char zero="";
    struct hostent *he;
    struct sockaddr_in server;
    int proverka=0;
//hostent         *host = 0;
/*
if (argc <3)
{
printf("Usage: %s <IP Address>\n",argv[0]);
exit(-1);
}
*/

    if ((he=gethostbyname(argv[1])) == NULL)  // Конвертирует имя хоста в IP адрес
    {
        printf("gethostbyname() error\n");
        exit(-1);
    }
    if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1)
    {
        printf("socket() error\n");
        exit(-1);
    }

    server.sin_family = AF_INET;  // Указываем тип сокета Интернет
    server.sin_port = htons(PORT);  // Указываем порт сокета
    /*
host = gethostbyname("192.168.0.101");//Указываем конкретый ип сокеа
CopyMemory(&server.sin_addr, host->h_addr_list[0], host->h_length);//
*/
    server.sin_addr= *((struct in_addr *)he->h_addr); // Указаваем адрес IP сокета

    if(connect(fd, (struct sockaddr *)&server,sizeof(struct sockaddr))==-1)     // Устанавливаем соединение
    {
        printf("connect() error\n");
        exit(-1);
    }

    if ((numbytes=recv(fd,buf,MAXDATASIZE,0)) == -1)
    {
        printf("recv() error\n");
        exit(-1);
    }

    buf[numbytes] = '\0';
    printf("Server Message: %s\n",buf);
    memset(buf,zero,22);
    // Вводим сообщение из консоли
    while(1)
    {
        if (proverka==0)
        {
            printf("Please enter the message: ");
            fgets(buffer, 22, stdin);
            i = strlen(buffer)-1;
            if(buffer[i]=='\n') buffer[i] = '\0';
            n=send (fd,buffer,22,0);// Отправляем данные
            if (n== -1)
            {
                printf("send() error\n");
            }
        }
        if (proverka==1)
        {
            if ((numbytes=recv(fd,buf,MAXDATASIZE,0)) == -1)
            {
                printf("recv() error\n");
                exit(-1);
            }
            buf[numbytes] = '\0';
            printf("File data: %s\n",buf);
            proverka=0;
        }
        if(strcmp(buffer,"file()")==0)
        {
            printf("Enter the number of bytes: ");
            fgets(buffer,4, stdin);
            i = strlen(buffer)-1;
            if(buffer[i]=='\n') buffer[i] = '\0';
            n=send (fd,buffer,4,0);// Отправляем данные
            if (n== -1)
            {
                printf("send() error\n");
            }
            if (n== -1)
            {
                printf("send() error\n");
            }
            proverka=1;
            printf("Enter the file name: ");
            fgets(buffer, 22, stdin);
            i = strlen(buffer)-1;
            if(buffer[i]=='\n') buffer[i] = '\0';
            n=send (fd,buffer,22,0);// Отправляем данные
            if (n== -1)
            {
                printf("send() error\n");
            }
            proverka=1;
        }
        memset(buffer,zero, 22); // Сбрасываем буфер
    }
    closesocket(fd);
    WSACleanup();
    return 0;
}

