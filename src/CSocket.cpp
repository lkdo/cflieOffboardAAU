#include "CSocket.h"

SOCKET createReadSocket(LPCSTR host, LPCSTR port)
{

	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	int iResult;
	
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		std::cout << "WSAStartup failed with error: " << iResult << std::endl;
		exit(-1);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(host, port, &hints, &result);
	if (iResult != 0) {
		std::cout << "getaddrinfo failed with error: " << iResult << std::endl;
		WSACleanup();
		exit(-1);
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			std::cout << "socket failed with error: " << (int) WSAGetLastError() << std::endl;
			WSACleanup();
			exit(-1);
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		std::cout << "Unable to connect to server!" << std::endl;
		WSACleanup();
		exit(-1);
	}

	return ConnectSocket;
}


int  readFromSocket( SOCKET ConnectSocket, char * recvbuf, int recvbuflen)
 {

		int iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);

		if (iResult > 0)
		{
#ifdef _DEBUG
			std::cout << "Bytes received: " << iResult << std::endl;
#endif
		}
		else if (iResult == 0)
		{
			std::cout << "Socket Connection closed !" << std::endl;
		}
		else
		{ 
			std::cout << "recv failed with error: " << (int) WSAGetLastError() << std::endl;
		}

		return iResult;
 }