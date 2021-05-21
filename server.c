#include <stdio.h>
#include <windows.h>
//#include <locale.h>
#define PORT 3550
#define BACKLOG 2
#define MAXDATASIZE 100
char *open_file(char name[20],int N)
{
    static char str[2000];
    memset(str,"",strlen(str));
    FILE *f;
    int i=0;
    f = fopen(name,"r");
    if (f== NULL)
    {
        printf("Error opening file %s\n",name);
        return "Error opening file";
    }
    while((!feof(f))&&(i<N))
    {
        str[i] = fgetc(f);
        i++;
	}
	str[i-1]='\0';
    fclose(f);
    return str;
}
main()
{
    //setlocale(LC_ALL, "Rus");
    WSADATA wsdata;
    WSAStartup(0x0101,&wsdata);
    int fd, fd2,n,N;
    int numbytes=-1;
    char buffer[256];
    char bytes[4];
    char file_name[20];
    struct sockaddr_in server;
    struct sockaddr_in client;
    int sin_size;
    if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 )
    {
        printf("socket() error\n");
        exit(-1);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;
    if (bind(fd,(struct sockaddr*)&server, sizeof(struct sockaddr))==-1)
    {
        printf ("bind() error\n");
        exit (-1);
    }
    if (listen (fd,BACKLOG) == -1)
    {
        printf ("listen() error\n");
        exit (-1);
    }

    sin_size=sizeof (struct sockaddr_in);
    if ((fd2 = accept(fd,(struct sockaddr *)&client,&sin_size)) == -1)
    {
        printf ("accept() error\n");
        exit (-1);
    }
    printf ("You got a connection from %s",inet_ntoa(client.sin_addr) );
    printf (":%d\n",PORT);
    send (fd2,"Welcome client_1.\n",22,0);
    while(1)
    {
        if ((numbytes=recv(fd2,buffer,22,0)) == -1)
        {
            printf("recv() error\n");
        }
        printf("Client Message: %s\n",buffer);
        if (strcmp(buffer,"file()")==0)
        {
            if ((numbytes=recv(fd2,bytes,4,0)) == -1)
            {
                printf("recv() error\n");
            }
            N=atoi(bytes);
            if ((numbytes=recv(fd2,file_name,22,0)) == -1)
            {
                printf("recv() error\n");
            }
            send (fd2,open_file(file_name,N),22,0);
        }
        if (strcmp(buffer,"quit()")==0)
        {
            return 0;
        }
    }
    closesocket (fd2);
    WSACleanup();
    return 0;
}
