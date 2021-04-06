#pragma once

#include "ConnectionAsynchronousOperation.h"

#ifdef _WIN32

#if defined(INVALID_SOCKET) && !defined(_WINSOCK2API_)
#error _WINSOCKAPI_ must be defined before <windows.h> is included to prevent that <winsock.h> is included
#endif

#ifndef _WINSOCK2API_
#undef _WINSOCKAPI_
#include <winsock2.h>
#endif

#include <ws2tcpip.h>

typedef u_short socket_port_t;

#elif __linux__

#include <netinet/in.h>

typedef unsigned short socket_port_t;

#endif // __linux__

class Socket;

/**
 * @brief	A class used to detect the status of asynchronous operations in a socket class.
 */
class SocketAsynchronousOperation : public ConnectionAsynchronousOperation
{
	friend class Socket;

protected:

	Socket* socket;

	/**
	 * @brief	The constructor of SocketAsynchronousOperation.
	 *
	 * @param[in]	socket	The socket
	 */
	SocketAsynchronousOperation(Socket* socket);

	/**
	 * @brief	The destructor of SocketAsynchronousOperation.
	 */
	~SocketAsynchronousOperation();

	virtual Connection_Asynchronous_Operation_Wait_For_Completion_Return virtualWaitForCompletion(transfer_size_t* bytesTransferred, bool* timeout) override;

	virtual Connection_Asynchronous_Operation_Cancel_Return virtualCancel() override;

	virtual bool virtualIsCompleted() override;

	/**
	 * @brief	Converts the given address into a sockaddr_in object
	 *
	 * @param[in]	address		The address
	 * @param		socketPort	The port
	 * @param[out]	socketAddr	The socket_in object
	 */
	void convertToSockaddr(const char* socketAddress, const socket_port_t socketPort, sockaddr_in* socketAddr);

	/**
	 * @brief	Extracts the socket address and socket port from a sockaddr_in object.
	 *
	 * @param		sockAddr	The sockaddr_in object to extract the data from
	 * @param[out]	address		The socket address. If non-NULL, a string will be dynamically allocated.
	 * @param[out]	socketPort	The socket port. May be NULL
	 */
	void extractFromSockaddr(const struct sockaddr_in sockAddr, char** socketAddress, socket_port_t* socketPort);

#ifdef __linux__
	virtual int getFileDescriptor() override;
#endif // __linux__
};