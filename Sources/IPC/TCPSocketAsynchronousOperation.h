#pragma once

#include "SocketAsynchronousOperation.h"

class TCPSocket;

/**
 * @brief	A class used to detect the status of asynchronous operations in a TCP socket.
 */
class TCPSocketAsynchronousOperation : public SocketAsynchronousOperation
{
	friend class TCPSocket;

private:

	enum class TCP_Socket_Asynchronous_Operation_Current_Operation
	{
		NONE,
		CONNECTING,
		ACCEPTING,
		WRITING,
		READING
	};

#ifdef _WIN32
	CHAR acceptBuffer[2 * (sizeof(SOCKADDR_IN) + 16)];
	DWORD acceptBufferBytesWritten;

	WSABUF sendRecvBuffer;
	DWORD receiveFlags;
#endif // _WIN32

	TCP_Socket_Asynchronous_Operation_Current_Operation currentOperation;

	TCPSocket* clientSocket;

	struct sockaddr_in connectSocketAddress;

protected:

	/**
	 * @brief	The constructor of TCPSocketAsynchronousOperation.
	 *
	 * @param[in]	socket	The TCP socket
	 */
	TCPSocketAsynchronousOperation(TCPSocket* socket);

	/**
	 * @brief	Queues this asynchronous operation to connect a socket to a server.
	 *
	 * @param[in]	address		The address of the server
	 * @param 		port		The port of the server
	 *
	 * @return	True if there was no error queuing the operation; False otherwise
	 */
	bool connectToServer(const char* address, socket_port_t port);

	/**
	 * @brief	Queues this asynchronous operation to accept a client from a TCP server.
	 *
	 * @param[out]	clientSocket	The client socket to be created. The given socket must be closed.
	 *
	 * @return	True if there was no error queuing the operation; False otherwise
	 */
	bool acceptClient(TCPSocket* clientSocket);

	virtual bool writeToConnection(const void* buffer, transfer_size_t bytesToWrite) override;

	virtual bool readFromConnection(void* buffer, transfer_size_t bytesToRead) override;

	virtual void onOperationCompleted(bool completedSuccessfully) override;
};