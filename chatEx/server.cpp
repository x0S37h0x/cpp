#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

#define MAX_CLIENTS 10

int startWinsock(void);


int main()
{
    long rc;
    SOCKET acceptSocket;
    //SOCKET connectedSocket;
    SOCKADDR_IN addr;
    char buf[256];
    char buf2[300];
    FD_SET fdSet;
    SOCKET clients[MAX_CLIENTS];
    int i;

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
    for(i=0;i<MAX_CLIENTS; i++)
    {
        clients[i] = INVALID_SOCKET;
    }
    /*
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
    */
   while(1)
   {
        FD_ZERO(&fdSet);
        FD_SET(acceptSocket, &fdSet);
        for(i=0; i< MAX_CLIENTS; i++)
        {
            if(clients[i] != INVALID_SOCKET)
            {
                FD_SET(clients[i], &fdSet);
            }
        }

  
    rc=select(0,&fdSet,NULL, NULL, NULL);
    if(rc == SOCKET_ERROR)
    {
        printf("Fehler: select, fehler code: %d\n", WSAGetLastError());
        return 1;
    }
   
    if(FD_ISSET(acceptSocket, &fdSet))
    {
        for(i=0;i<MAX_CLIENTS; i++)
        {
            if(clients[i] == INVALID_SOCKET)
            {
                clients[i] = accept(acceptSocket, NULL, NULL);
                printf("Neuen Client angenommen (%d)\n", i);
                break;
            }
        }
    }
    for (i=0; i < MAX_CLIENTS; i++)
    {
        if(clients[i] == INVALID_SOCKET)
        {
            continue;
        }
        if(FD_ISSET(clients[i], &fdSet))
        {
            rc = recv(clients[i], buf, 256, 0);
            if(rc == 0 || rc == SOCKET_ERROR)
            {
                printf("Client %d hat die Verbindung geschlossen\n", i);
                closesocket(clients[i]);
                clients[i] = INVALID_SOCKET;
            }
            else
            {
                buf[rc] = '\0';
                printf("Client %d hat folgendes gesandt : %s\n", i, buf);
                sprintf(buf2, "Du mich auch %s\n",buf);
                send(clients[i], buf2, (int)strlen(buf2), 0);       
            }
        
        }
    }
    
 }

}

int  startWinsock(void)
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2,0), &wsa);
}


