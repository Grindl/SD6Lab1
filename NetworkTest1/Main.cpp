#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")

char* ServerIPAddress = "129.119.228.206";
char* portNumber = "6000";
//char* portNumber = "5000";



void startServer(const char* IPAddress, const char* port)
{
	WSAData myWSAData;
	SOCKET mySocket = INVALID_SOCKET;
	SOCKET listeningSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	int WSAResult;
	int messageInLength = 0;
	char readBuffer [256];

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	WSAResult = WSAStartup(MAKEWORD(2,2), &myWSAData);
	WSAResult = getaddrinfo(NULL, port, &hints, & result);
	mySocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	WSAResult = bind(mySocket, result->ai_addr, (int)result->ai_addrlen);
	WSAResult = listen(mySocket,SOMAXCONN);
	listeningSocket = accept(mySocket, NULL, NULL);	
	do
	{
		messageInLength = recv(listeningSocket, readBuffer, 256, 0);
		std::cout<< readBuffer;
		send(listeningSocket, readBuffer, messageInLength, 0);
	} while(readBuffer != "quit");

}

void startClient(const char* IPAddress, const char* port)
{
	WSAData myWSAData;
	int WSAResult;
	SOCKET mySocket = INVALID_SOCKET;
	//struct addrinfo *ptr = NULL;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	SOCKADDR UDPSocketAddress;
	long IPAsLong = inet_addr(IPAddress);
	char buffer [256];
	std::string cleanedBuffer;

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	//hints.ai_socktype = SOCK_DGRAM;
	//hints.ai_protocol = IPPROTO_UDP;
	//UDPSocketAddress.sa_data = IPAddress;

	WSAResult = WSAStartup(MAKEWORD(2,2), &myWSAData);
	WSAResult = getaddrinfo(IPAddress, port, &hints, &result);
	mySocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	WSAResult = connect(mySocket, result->ai_addr, (int)result->ai_addrlen);
	while(cleanedBuffer != "quit")
	{
		std::cin>>buffer;
		cleanedBuffer = buffer;
		send(mySocket, cleanedBuffer.c_str(), cleanedBuffer.length()+1, 0);
		recv(mySocket, buffer, 256, 0);
		std::cout<<buffer<<std::endl;
	}
	closesocket(mySocket);
	WSACleanup();
}

int main(_In_ int _Argc, _In_count_(_Argc) _Pre_z_ char ** _Argv, _In_z_ char ** _Env)
{
	std::string IPIn;
	std::string portIn;
	std::string mode;
	char readBuffer [256];
	while(mode != "client" && mode !="server")
	{
		std::cin>>readBuffer;
		mode = readBuffer;
	}
	std::cin>>readBuffer;
	IPIn = readBuffer;
	std::cin>>readBuffer;
	portIn = readBuffer;
	if (mode == "client")
	{
		startClient(IPIn.c_str(), portIn.c_str());
	}
	else
	{
		startServer(IPIn.c_str(), portIn.c_str());
	}
	

}