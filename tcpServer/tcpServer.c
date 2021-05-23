
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")      
#pragma warning(disable:4996) 
int main()
{
    WORD ws_version = MAKEWORD(2, 2);   
    WSADATA wsaData;                    
    int recv_status;
    int iResult;
    WSAStartup(ws_version, &wsaData);

    /*socket*/
    SOCKET s_server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    SOCKADDR_IN addr_server;
    addr_server.sin_family = AF_INET;   
    addr_server.sin_port = htons(5050); 
    addr_server.sin_addr.s_addr = htonl(INADDR_ANY);    

    /*bind*/
    int bind_status;
    bind_status = bind(s_server, (SOCKADDR*)&addr_server, sizeof(SOCKADDR));
    if (bind_status == SOCKET_ERROR)
    {
        printf("bind error : fail to bind! \n");
    }
    else
    {
        printf("bind successfully!\n");
    }

    /*listen*/
    listen(s_server, 5);//max=5
    printf("listening ... \n");

    SOCKADDR_IN addr_client;           
    int len = sizeof(SOCKADDR);
    int count = 0;                       
    SOCKET s_client;                    

    char buf[1000];

    const char* sendData;

    while (1)
    {
        printf("\nWaiting for the connection...\n");
        /*accept*/
        s_client = accept(s_server, (SOCKADDR*)&addr_client, &len);
        if (s_client == INVALID_SOCKET)
        {
            printf("Accept error : fail to accept client! ");
        }
        else
        {
            count++;
            printf("\nAccept successfully!\n");

            printf(" No - %d \n", count);
            printf(" Port- %d\n", ntohs(addr_client.sin_port));
            printf(" IP - %s\n", inet_ntoa(addr_client.sin_addr));
            sendData = "Hello! Client, This is Server\n";
            iResult=send(s_client, sendData, strlen(sendData), 0);
            if (iResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                
            }
            do 
            {
                recv_status = recv(s_client, buf, 100, 0);
                if (recv_status > 0)
                {
                    printf("Received: ");
                    buf[recv_status] = 0x00;
                    printf(buf);
                }
                sendData = "Server taks: ";
                send(s_client, sendData, strlen(sendData), 0);
                sendData = "*0221155667788";
                send(s_client, sendData, strlen(sendData), 0);
            } while (recv_status > 0);
            closesocket(s_client);
        }
    }

    closesocket(s_server);      
    WSACleanup();

    return 0;
}