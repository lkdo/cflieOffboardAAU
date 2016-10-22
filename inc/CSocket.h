#ifndef __C_CSOCKET_H__ 
#define __C_CSOCKET_H__ 

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib

SOCKET createReadSocket(LPCSTR host, LPCSTR port);
int  readFromSocket(SOCKET ConnectSocket, char * recvbuf, int recvbuflen);

#endif __C_CSOCKET_H__