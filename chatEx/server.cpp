#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>

int startWinsock(void);


int main()
{
    long rc;
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
    return 0;

}

int  startWinsock(void)
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2,0), &wsa);
}


