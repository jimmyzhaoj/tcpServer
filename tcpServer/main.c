#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable:4996)
void main()
{
	// Initialze winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	;
	int wsOk = WSAStartup(ver, &wsData);

	int ms;


	if (wsOk != 0)
	{
		printf("Can't Initialize winsock! Quitting");
		return;
	}
	printf("Initialize winsock!\n");
	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		printf("Can't create a socket! Quitting");
		return;
	}
	printf("Create a socket!\n");
	// Bind the ip address and port to a socket
	SOCKADDR_IN hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(59000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton .... 

	bind(listening, (SOCKADDR_IN*)&hint, sizeof(hint));

	// Tell Winsock the socket is for listening 
	listen(listening, SOMAXCONN);
	printf("Waitinf for clinet to connect......\n");
	// Wait for a connection
	SOCKADDR_IN client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (SOCKADDR_IN*)&client, &clientSize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((SOCKADDR_IN*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		printf("%s connected on port %s\n ", host, service);
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		printf( " connected on port %s\n\n", ntohs(client.sin_port));
	}

	// Close listening socket
	closesocket(listening);

	// While loop: accept and echo message back to client
	char buf[4096];
	char* msg= "809ffec10100";
	int number = 0x809ffec10100;
	char msg2[6] = { 0x80, 0x9f, 0xed, 0x93, 0x01,0x00};

	while (1)
	{
		ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			printf ("Error in recv(). Quitting\n");
			break;
		}

		if (bytesReceived == 0)
		{
			printf ("Client disconnected \n");
			break;
		}

		printf(buf);
		printf("\n");

		 printf("\n======[RSD Chuck ACK and NACK Test Menu:]=====\n");
         printf("| 1. Echo message back to client                \n");
         printf("| 2. Send test message to DUT                    \n");
         printf("| 3. Exit?                                      \n");
         printf("================================================\n");
         int iResult;
         (void)scanf("%d", &ms);
		 switch (ms)
         {
               case 1:
				   // Echo message back to client
				  iResult=send(clientSocket, buf, bytesReceived + 1, 0);             
                          
                  if (iResult == SOCKET_ERROR) {
                      printf("send failed with error: %d\n", WSAGetLastError());                    
                  }
                  break;

               case 2:
				   //strcpy(buf, msg);
				   
				   //iResult = send(clientSocket, (char*)&number, sizeof(number)+1, 0);
			//	   iResult = send(clientSocket, (char*)&msg2, sizeof(msg2), 0);
				   iResult = send(clientSocket, msg2, sizeof(msg2), 0);
				   if (iResult == SOCKET_ERROR) {
					   printf("send failed with error: %d\n", WSAGetLastError());
				   }
                   break;
                case 3:
				   closesocket(clientSocket);
				   WSACleanup();
				   return;
                default:
                   printf("Bad Choice!");
                   break;
           }
		   

	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");
}