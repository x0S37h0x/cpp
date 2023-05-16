#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

int startWinsock(void);
long getAddrFromString(char* hostnameOrIp, SOCKADDR_IN* addr);


int main(int argc, char** argv)
{
    long rc;
    SOCKET s;
    SOCKADDR_IN addr;
    char buf[256];

    if(argc <2)
    {
        printf("Usage: sock <hostname oder ip des servers>\n");
        return 1;
    }
    rc=startWinsock();
    if(rc != 0)
    {
        std::cout << "Fehler: startWinsock, fehlercode." << rc << std::endl;
        return 1;
    }
    else
    {
        printf("winSock gestartet!\n");
    }

    s=socket(AF_INET, SOCK_STREAM, 0);
    if(s==INVALID_SOCKET)
    {
        printf("Fehler: Der Socket konnte nicht erstellt werden, fehler code: %d\n", WSAGetLastError());
        return 1;
    } 
    else
    {
        printf("Socket erstellt!\n");
    }

    memset(&addr,0,sizeof(SOCKADDR_IN)); 
    addr.sin_family=AF_INET;
    addr.sin_port = htons(12345);
    //addr.sin_addr.s_addr=inet_addr("127.0.0.1"); // Localhost
    rc=getAddrFromString(argv[1], &addr);
     if(rc == SOCKET_ERROR)
    {
        printf("IP für %s konnte nicht aufgeloest werden", argv[1]);
        return 1;
    }
    else
    {
        printf("IP aufgelöst\n");
    }

    rc = connect(s,(SOCKADDR*)&addr, sizeof(SOCKADDR));
    if(rc == SOCKET_ERROR)
    {
        printf("Fehler: connect gescheitert, fehler code: %d\n", WSAGetLastError());
        return 1;
    }
    else
    {
        printf("Verbunden mit 127.0.0.1..\n");
    }
    
    //Daten austauschen
    while(rc != SOCKET_ERROR)
    {
        printf("\nZeichenfolge eingeben max[256]: ");
        gets(buf);
        send(s, buf, strlen(buf), 0);
        rc=recv(s, buf, 256, 0);
        if(rc == 0 )
        {
            printf("Server hat die Verbindung getrennt...\n");
            break;
        }
        if(rc == SOCKET_ERROR)
        {
            printf("Fehler: recv, fehler code: %d\n", WSAGetLastError());
            break;
        }
        buf[rc] ='\0';
        printf("\nServer antwortet: %s\n", buf);
    }
    closesocket(s);
    WSACleanup();
    return 0;

}

int  startWinsock(void)
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2,0), &wsa);
}

long getAddrFromString(char* hostnameOrIp, SOCKADDR_IN* addr)
{
    long rc;
    unsigned long ip;
    HOSTENT* he;

    if(hostnameOrIp == NULL || addr == NULL)
        return SOCKET_ERROR;
    
    ip =inet_addr(hostnameOrIp);

    if(ip != INADDR_NONE)
    {
        addr->sin_addr.s_addr =ip;
        return 0;
    }
    else 
    {
        he=gethostbyname(hostnameOrIp);
        if(he == NULL)
        {
            return SOCKET_ERROR;
        }
        else
        {
            memcpy(&(addr->sin_addr), he->h_addr_list[0],4);
        }
        return 0;
    }
}

