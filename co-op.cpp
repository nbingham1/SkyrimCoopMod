#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "common\skyscript.h"
#include "common\obscript.h"
#include "common\types.h"
#include "common\enums.h"
#include "common\plugin.h"

#include "sdk_addon.h"
#include "cmd_queue.h"
#include "ref_list.h"
#include "packets.h"

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

#pragma comment(lib, "Ws2_32.lib")

#define CONFIG_FILE "co-op.ini"
#define SCR_NAME "Co-op"

int thread_count = 0;
HANDLE doneMutex = NULL;
HANDLE threadMutex = NULL;

struct connection
{
	connection()
	{
		for (int i = 0; i < 256; i++)
		{
			address[i] = '\0';
			port[i] = '\0';
		}
	}
	~connection()
	{
		for (int i = 0; i < 256; i++)
		{
			address[i] = '\0';
			port[i] = '\0';
		}
	}

	char address[256];
	char port[256];
};

void ServerThread(void *c)
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
		return;

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	SOCKET server_socket = INVALID_SOCKET, client_socket = INVALID_SOCKET;

	PrintNote("[%s] : Creating Server.", SCR_NAME);
	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, ((connection*)c)->port, &hints, &result) != 0)
	{
		PrintNote("[%s] : Get Address Info failed.", SCR_NAME);
		WSACleanup();
		WaitForSingleObject(threadMutex, INFINITE);
		thread_count--;
		ReleaseMutex(threadMutex);
		return;
	}
			
	server_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (server_socket == INVALID_SOCKET)
	{
		PrintNote("[%s] : Error opening socket.", SCR_NAME);
		freeaddrinfo(result);
		WSACleanup();
		WaitForSingleObject(threadMutex, INFINITE);
		thread_count--;
		ReleaseMutex(threadMutex);
		return;
	}

	if (bind(server_socket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR)
	{
		PrintNote("[%s] : Error binding socket.", SCR_NAME);
		freeaddrinfo(result);
		closesocket(server_socket);
		WSACleanup();
		WaitForSingleObject(threadMutex, INFINITE);
		thread_count--;
		ReleaseMutex(threadMutex);
		return;
	}

	freeaddrinfo(result);

	PrintNote("[%s] : Listening for clients at %s, on port %s", SCR_NAME, ((connection*)c)->address, ((connection*)c)->port);

	if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(server_socket);
		WSACleanup();
		WaitForSingleObject(threadMutex, INFINITE);
		thread_count--;
		ReleaseMutex(threadMutex);
		return;
	}

	client_socket = accept(server_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET) 
	{
		PrintNote("[%s] : Error accepting client socket.", SCR_NAME);
		closesocket(server_socket);
		WSACleanup();
		WaitForSingleObject(threadMutex, INFINITE);
		thread_count--;
		ReleaseMutex(threadMutex);
		return;
	}

	closesocket(server_socket);

	while (WaitForSingleObject(doneMutex, 0) == WAIT_TIMEOUT)
	{
		if (!rlSend(client_socket)) 
		{
			cqPushCommand("SetGlobalTimeMultiplier 1.0", NULL);
			PrintNote("[%s] : Error sending data to client.", SCR_NAME);
			break;
		}

		if (!rlReceive(client_socket, false))
		{
			cqPushCommand("SetGlobalTimeMultiplier 1.0", NULL);
			PrintNote("[%s] : Error receiving data from client.", SCR_NAME);
			break;
		}
	}

	if (shutdown(client_socket, SD_SEND) == SOCKET_ERROR)
		PrintNote("[%s] : Error shutting down connection.", SCR_NAME);

	closesocket(client_socket);
	WSACleanup();

	WaitForSingleObject(threadMutex, INFINITE);
	thread_count--;
	ReleaseMutex(threadMutex);
}

void ClientThread(void *c)
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
		return;

	struct addrinfo *result = NULL, *ptr = NULL, hints;
	SOCKET server_socket = INVALID_SOCKET, client_socket = INVALID_SOCKET;

	PrintNote("[%s] : Creating Client.", SCR_NAME);
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(((connection*)c)->address, ((connection*)c)->port, &hints, &result) != 0)
	{
		PrintNote("[%s] : Get Address Info failed.", SCR_NAME);
		WSACleanup();
		WaitForSingleObject(threadMutex, INFINITE);
		thread_count--;
		ReleaseMutex(threadMutex);
		return;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) 
	{
		// Create a SOCKET for connecting to server
		server_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (server_socket == INVALID_SOCKET)
		{
			PrintNote("[%s] : Error opening socket.", SCR_NAME);
			WSACleanup();
			WaitForSingleObject(threadMutex, INFINITE);
			thread_count--;
			ReleaseMutex(threadMutex);
			return;
		}

		// Connect to server.
		if (connect(server_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == SOCKET_ERROR)
		{
			closesocket(server_socket);
			server_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (server_socket == INVALID_SOCKET)
	{
		PrintNote("[%s] : Unable to connect to server.", SCR_NAME);
		WSACleanup();
		WaitForSingleObject(threadMutex, INFINITE);
		thread_count--;
		ReleaseMutex(threadMutex);
		return;
	}

	while (WaitForSingleObject(doneMutex, 0) == WAIT_TIMEOUT)
	{
		if (!rlReceive(server_socket, true))
		{
			cqPushCommand("SetGlobalTimeMultiplier 1.0", NULL);
			PrintNote("[%s] : Error receiving data from server.", SCR_NAME);
			break;
		}

		if (!rlSend(server_socket)) 
		{
			cqPushCommand("SetGlobalTimeMultiplier 1.0", NULL);
			PrintNote("[%s] : Error sending data to server.", SCR_NAME);
			break;
		}
	}

	if (server_socket == INVALID_SOCKET || shutdown(server_socket, SD_SEND) == SOCKET_ERROR)
		PrintNote("[%s] : Error shutting down connection.", SCR_NAME);

	closesocket(server_socket);
	WSACleanup();

	WaitForSingleObject(threadMutex, INFINITE);
	thread_count--;
	ReleaseMutex(threadMutex);
}

void main()
{
	connection con;

	// reading key from the config file
	IniReadString(CONFIG_FILE, "main", "address", "192.168.1.12", con.address, 256);
	IniReadString(CONFIG_FILE, "main", "port", "20081", con.port, 256);
	BYTE serve_key		= IniReadInt(CONFIG_FILE, "main", "serve_key", 0);
	BYTE connect_key	= IniReadInt(CONFIG_FILE, "main", "connect_key", 0);
	BYTE disconnect_key	= IniReadInt(CONFIG_FILE, "main", "disconnect_key", 0);
	PrintNote("[%s] : Press '%s' to start a server and '%s' to connect to a server", SCR_NAME, GetKeyName(serve_key).c_str(), GetKeyName(connect_key).c_str());	
	
	cqInit();
	rlInit();

	doneMutex = CreateMutex(NULL, TRUE, NULL);
	threadMutex = CreateMutex(NULL, FALSE, NULL);

	while (TRUE)
	{
		if (GetKeyPressed(serve_key) && thread_count == 0) 
		{
			WaitForSingleObject(threadMutex, INFINITE);
			thread_count++;
			ReleaseMutex(threadMutex);
			_beginthread(ServerThread, 5000, &con);
		}
		else if (GetKeyPressed(connect_key) && thread_count == 0)
		{
			WaitForSingleObject(threadMutex, INFINITE);
			thread_count++;
			ReleaseMutex(threadMutex);
			_beginthread(ClientThread, 5000, &con);
		}
		else if (GetKeyPressed(disconnect_key))
		{
			ReleaseMutex(doneMutex);
			PrintNote("[%s] : Disconnected.", SCR_NAME);
		}

		rlUpdate();
		cqExecuteCommand();

		tsWait(0);
	}

	tsWait(1000);

	rlRelease();
}