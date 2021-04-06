#pragma once

#include "Socket.h"
#include "TCPSocketAsynchronousOperation.h"

/**
 * Enumerates all possible return values of the functions of CTCPSocket.
 */
enum : Connection_Return
{
	TCP_SOCKET_RETURN_ERROR_SERVER_ONLY = SOCKET_RETURN_AMOUNT,
	TCP_SOCKET_RETURN_ERROR_CLIENT_ONLY,
	TCP_SOCKET_RETURN_ERROR_COULD_NOT_SET_TO_SERVER,
	TCP_SOCKET_RETURN_ERROR_COULD_NOT_CONNECT_TO_SERVER,
	TCP_SOCKET_RETURN_ERROR_COULD_NOT_ACCEPT_CLIENT
};

/**
 * @brief	A TCP socket class that supports Windows and Linux.
 */
class TCPSocket : public Socket
{
	friend class TCPSocketAsynchronousOperation;

private:

	bool isServer;

protected:

	virtual int getSocketType() override;

	virtual int getSocketProtocol() override;

	virtual void reset() override;

public:

	/**
	 * @brief	The constructor of TCPSocket
	 */
	TCPSocket();

	/**
	 * @brief	The destructor of TCPSocket
	 */
	virtual ~TCPSocket() override;

	/**
	 * @brief	Opens the TCP socket as a server and binds it to a port.
	 *
	 * @param 	port	The port the TCP socket is to be bound to
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_ALREADY_OPENED					The socket is already opened.
	 *			SOCKET_RETURN_ERROR_COULD_NOT_INITIALIZE_WINSOCK_DLL	Could not initialize the Winsock DLL. (Windows only)
	 *			SOCKET_RETURN_ERROR_COULD_NOT_CREATE_SOCKET				Could not create the socket.
	 *			SOCKET_RETURN_ERROR_COULD_NOT_BIND_TO_PORT				Could not bind the socket to the port.
	 *			TCP_SOCKET_RETURN_ERROR_COULD_NOT_SET_TO_SERVER			Could not set the socket to a server.
	 */
	Connection_Return CreateServer(socket_port_t port);

	/**
	 * @brief	Opens the TCP socket and connects it to a server at the given address.
	 *
	 * @param[in]	address					The address of the server
	 * @param 		port					The port of the server
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS									The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_ALREADY_OPENED						The socket is already opened.
	 *			SOCKET_RETURN_ERROR_COULD_NOT_INITIALIZE_WINSOCK_DLL		Could not initialize the Winsock DLL. (Windows only)
	 *			SOCKET_RETURN_ERROR_COULD_NOT_CREATE_SOCKET					Could not create the socket.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION					The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED					The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION		Could not wait for the operation.
	 *			TCP_SOCKET_RETURN_ERROR_COULD_NOT_CONNECT_TO_SERVER			Could not connect to the server.
	 */
	Connection_Return Connect(const char* address, socket_port_t port, TCPSocketAsynchronousOperation* asynchronousOperation);

	/**
	 * @brief	Opens the TCP socket and connects it to a server at the given address. This function is blocking until the operation is completed or a given interval is elapsed.
	 *
	 * @param[in]	address				The address of the server
	 * @param 		port				The port of the server
	 * @param		timeoutInterval		The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS									The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_ALREADY_OPENED						The socket is already opened.
	 *			SOCKET_RETURN_ERROR_COULD_NOT_INITIALIZE_WINSOCK_DLL		Could not initialize the Winsock DLL. (Windows only)
	 *			SOCKET_RETURN_ERROR_COULD_NOT_CREATE_SOCKET					Could not create the socket.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION					The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED					The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT					The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION		Could not wait for the operation.
	 *			TCP_SOCKET_RETURN_ERROR_COULD_NOT_CONNECT_TO_SERVER			Could not connect to the server.
	 */
	Connection_Return ConnectExtended(const char* address, socket_port_t port, time_interval_t timeoutInterval);

	/**
	 * @brief	Accepts a new client socket and creates it.
	 *
	 * @param[out]	clientSocket			The client socket to be created. The given socket must be closed.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The TCP socket is not opened but needs to be for this operation.
	 *			TCP_SOCKET_RETURN_ERROR_SERVER_ONLY						The TCP socket is a client socket but needs to be a server for this operation.
	 *			CONNECTION_RETURN_ERROR_ALREADY_OPENED					The given client socket is already opened.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 *			TCP_SOCKET_RETURN_ERROR_COULD_NOT_ACCEPT_CLIENT			Could not accept a client.
	 */
	Connection_Return AcceptClient(TCPSocket* clientSocket, TCPSocketAsynchronousOperation* asynchronousOperation);

	/**
	 * @brief	Accepts a new client socket and creates it. This function is blocking until the operation is completed or a given interval is elapsed.
	 *
	 * @param[out]	clientSocket			The client socket to be created. The given socket must be closed.
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The TCP socket is not opened but needs to be for this operation.
	 *			TCP_SOCKET_RETURN_ERROR_SERVER_ONLY						The TCP socket is a client socket but needs to be a server for this operation.
	 *			CONNECTION_RETURN_ERROR_ALREADY_OPENED					The given client socket is already opened.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT					The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 *			TCP_SOCKET_RETURN_ERROR_COULD_NOT_ACCEPT_CLIENT			Could not accept a client.
	 */
	Connection_Return AcceptClientExtended(TCPSocket* clientSocket, time_interval_t timeoutInterval);

	virtual TCPSocketAsynchronousOperation* CreateAsynchronousOperation() override;
};