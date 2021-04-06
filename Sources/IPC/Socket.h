#pragma once

#include "Connection.h"
#include "SocketAsynchronousOperation.h"

#ifdef _WIN32

#if defined(INVALID_SOCKET) && !defined(_WINSOCK2API_)
#error _WINSOCKAPI_ must be defined before <windows.h> is included to prevent that <winsock.h> is included
#endif

#ifndef _WINSOCK2API_
#undef _WINSOCKAPI_
#include <winsock2.h>
#endif

#pragma comment(lib,"ws2_32.lib") // Link Winsock library

#endif // _WIN32

/**
 * All possible return values of the functions of Socket.
 */
enum : Connection_Return
{
	SOCKET_RETURN_ERROR_COULD_NOT_INITIALIZE_WINSOCK_DLL = CONNECTION_RETURN_AMOUNT,
	SOCKET_RETURN_ERROR_COULD_NOT_CREATE_SOCKET,
	SOCKET_RETURN_ERROR_COULD_NOT_BIND_TO_PORT,
	SOCKET_RETURN_AMOUNT
};

/**
 * @brief	An abstract socket class that supports Windows and Linux.
 */
class Socket : public Connection
{
	friend class SocketAsynchronousOperation;

protected:

#ifdef _WIN32
	SOCKET internSocket;
#elif __linux__
	int internSocket;
#endif // __linux__

	bool isOpenedDirtily; // This variable indicates whether this socket is opened dirtily (i.e. the internal socket is a valid socket object but the user should not access it in any way)

	/**
	 * @brief	Opens the socket.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_ALREADY_OPENED					The socket is already opened.
	 *			SOCKET_RETURN_ERROR_COULD_NOT_INITIALIZE_WINSOCK_DLL	Could not initialize the Winsock DLL. (Windows only)
	 *			SOCKET_RETURN_ERROR_COULD_NOT_CREATE_SOCKET				Could not create the socket.
	 */
	Connection_Return open();

	/**
	 * @brief	Opens the socket and binds it to a port.
	 *
	 * @param 	port	The port the socket is to be bound to
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_ALREADY_OPENED					The socket is already opened.
	 *			SOCKET_RETURN_ERROR_COULD_NOT_INITIALIZE_WINSOCK_DLL	Could not initialize the Winsock DLL. (Windows only)
	 *			SOCKET_RETURN_ERROR_COULD_NOT_CREATE_SOCKET				Could not create the socket.
	 *			SOCKET_RETURN_ERROR_COULD_NOT_BIND_TO_PORT				Could not bind the socket to the port.
	 */
	Connection_Return openBound(socket_port_t port);

	/**
	 * @brief	Opens the socket dirtily, i.e. creating the internal socket but not marking it to be used by the user.
	 *			Additionally, the socket will be bound to the given address.
	 *			Use this function if the internal socket is to be directly modified before granting the user access to it.
	 *
	 * @param 	port	The port the socket is to be bound to
	 *
	 * @return	True if the socket was opened dirtily; False if not.
	 */
	bool openBoundDirtily(socket_port_t port);

	/**
	 * @brief	Opens the socket dirtily, i.e. creating the internal socket but not marking it to be used by the user.
	 *			Use this function if the internal socket is to be directly modified before granting the user access to it.
	 *
	 * @return	True if the socket was opened dirtily; False if not.
	 */
	bool openDirtily();

	/**
	 * @brief	Closes the socket dirtily, i.e. closing the socket if it was opened dirtily.
	 *
	 * @return	True if the socket was closed; False if not.
	 */
	bool closeDirtily();

	virtual void reset() override;

	/**
	 * @brief	Returns the socket's type
	 *
	 * @return	Socket type
	 */
	virtual int getSocketType() = 0;

	/**
	 * @brief	Returns the socket's protocol
	 *
	 * @return	Socket protocol
	 */
	virtual int getSocketProtocol() = 0;

public:

	/**
	 * @brief	The constructor of Socket
	 */
	Socket();

	/**
	 * @brief	The destructor of CSocket
	 */
	virtual ~Socket() override;

	Connection_Return Close() override;
};