#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

int startWinsock(void);


int main()
{
    long rc;
    SOCKET acceptSocket;
    SOCKET connectedSocket;
    SOCKADDR_IN addr;
    char buf[256];
    char buf2[300];
    rc=startWinsock();
    if(rc != 0)
    {
        printf("Fehler: startWinsock, fehler code: %d\n", WSAGetLastError());
        return 1;
    }   
    else
    {
        printf("winSock gestartet!\n");
    }

    acceptSocket=socket(AF_INET, SOCK_STREAM, 0);
    if(acceptSocket==INVALID_SOCKET)
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
    addr.sin_addr.s_addr=ADDR_ANY; // Localhost

    rc = bind(acceptSocket,(SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
    if(rc == SOCKET_ERROR)
    {
        printf("Fehler: Bind gescheitert, fehler code: %d\n", WSAGetLastError());
        return 1;
    }
    else
    {
        printf("Socket an port 12345 gebunden\n");
    }
    rc=listen(acceptSocket, 10);
    if(rc == SOCKET_ERROR)
    {
        printf("Fehler: listen, fehler code: %d\n", WSAGetLastError());
        return 1;
    }
    else
    {
        printf("acceptsocket ist im listen modus....\n");
    }
    connectedSocket = accept(acceptSocket, NULL, NULL);
    if(connectedSocket == INVALID_SOCKET)
    {
         printf("Fehler: accept, fehler code: %d\n", WSAGetLastError());
        return 1;
    }
    else
    {
        printf("Neue Verbindung wurde akzeptiert.\n");
    }
    // Daten austauschen
    while(rc !=SOCKET_ERROR)
    {
        rc=recv(connectedSocket, buf, 256, 0);
        if(rc == 0)
        {
            printf("Server hat die Verbundung getrennt ... \n");
            break;
        }
        if(rc == SOCKET_ERROR)
        {
            printf("Fehler: recv, fehler code: %d\n", WSAGetLastError());
            break;
        }
        buf[rc] ='\0';
        printf("Client sendet: %s\n", buf);
        sprintf(buf2, "Du mich auch %s", buf);
        rc=send(connectedSocket,buf2, strlen(buf2),0);
    }
    closesocket(acceptSocket);
    closesocket(connectedSocket);
    WSACleanup();
    return 0;

}

int  startWinsock(void)
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2,0), &wsa);
}


