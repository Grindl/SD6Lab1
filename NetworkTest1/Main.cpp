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

enum PROTOCOL_TYPE {PROTOCOL_TCP, PROTOCOL_UDP};

void startTCPServer(const char* port)
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
	closesocket(mySocket);
	closesocket(listeningSocket);
	WSACleanup();
}

void startUDPServer(const char* port)
{
	WSAData myWSAData;
	SOCKET mySocket = INVALID_SOCKET;
	SOCKET listeningSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	struct sockaddr_in mySockAddr, theirSockAddr;
	int theirSockLength;
	int WSAResult;
	int messageInLength = 0;
	char readBuffer [256];

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;

	mySockAddr.sin_family = AF_INET;
	mySockAddr.sin_addr.s_addr = INADDR_ANY;
	mySockAddr.sin_port = atoi(port);

	WSAResult = WSAStartup(MAKEWORD(2,2), &myWSAData);
	//WSAResult = getaddrinfo(NULL, port, &hints, & result);
	//mySocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	mySocket = socket(AF_INET, SOCK_DGRAM, 0);
	WSAResult = bind(mySocket, (struct sockaddr *)&mySockAddr, sizeof(mySockAddr));
	//WSAResult = listen(mySocket,SOMAXCONN);
	//listeningSocket = accept(mySocket, NULL, NULL);	
	do
	{
		messageInLength = recvfrom(listeningSocket, readBuffer, 256, 0, (struct sockaddr *)&theirSockAddr, &theirSockLength);
		std::cout<< readBuffer;
		sendto(listeningSocket, readBuffer, messageInLength, 0, (struct sockaddr *)&theirSockAddr, theirSockLength);
	} while(readBuffer != "quit");
	closesocket(mySocket);
	closesocket(listeningSocket);
	WSACleanup();
}

void startTCPClient(const char* IPAddress, const char* port)
{
	WSAData myWSAData;
	int WSAResult;
	SOCKET mySocket = INVALID_SOCKET;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	char buffer [256];
	std::string cleanedBuffer;

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

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

void startUDPClient(const char* IPAddress, const char* port)
{
	WSAData myWSAData;
	int WSAResult;
	SOCKET mySocket = INVALID_SOCKET;
	struct addrinfo *result = NULL;
	struct addrinfo hints;
	SOCKADDR UDPSocketAddress;
	long IPAsLong = inet_addr(IPAddress);
	char buffer [256];
	std::string cleanedBuffer;

	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
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
	std::string protocol;
	PROTOCOL_TYPE protocolType;
	char readBuffer [256]; //TODO security vulnerability
	while(mode != "client" && mode !="server")
	{
		std::cout<<"client/server: ";
		std::cin>>readBuffer;
		mode = readBuffer;
	}
	while (protocol != "tcp" && protocol != "udp")
	{
		std::cout<<"tcp/udp: ";
		std::cin>>readBuffer;
		protocol = readBuffer;
	}
	if (protocol == "tcp")
	{
		protocolType = PROTOCOL_TCP;

		if (mode == "client")
		{
			std::cout<<"IP address: ";
			std::cin>>readBuffer;
			IPIn = readBuffer;
			std::cout<<"Port: ";
			std::cin>>readBuffer;
			portIn = readBuffer;
			startTCPClient(IPIn.c_str(), portIn.c_str());
		}
		else
		{
			std::cout<<"Port: ";
			std::cin>>readBuffer;
			portIn = readBuffer;
			startTCPServer(portIn.c_str());
		}
	}
	else
	{
		protocolType = PROTOCOL_UDP;

		if (mode == "client")
		{
			std::cout<<"IP address: ";
			std::cin>>readBuffer;
			IPIn = readBuffer;
			std::cout<<"Port: ";
			std::cin>>readBuffer;
			portIn = readBuffer;
			startUDPClient(IPIn.c_str(), portIn.c_str());
		}
		else
		{
			std::cout<<"Port: ";
			std::cin>>readBuffer;
			portIn = readBuffer;
			startUDPServer(portIn.c_str());
		}
	}



}