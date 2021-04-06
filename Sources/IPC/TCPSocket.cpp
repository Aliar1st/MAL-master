#include "TCPSocket.h"

TCPSocket::TCPSocket()
{
	reset();
}

TCPSocket::~TCPSocket()
{
	if (IsOpened())
	{
		Close();
	}
}

Connection_Return TCPSocket::CreateServer(socket_port_t port)
{
	// Open the socket and check for errors
	Connection_Return baseResult = Socket::openBound(port);

	if (IsError(baseResult))
	{
		return baseResult;
	}

	// Set socket as server socket and accept up to 128 queued sockets
	if (listen(internSocket, 128))
	{
		Close();

		return TCP_SOCKET_RETURN_ERROR_COULD_NOT_SET_TO_SERVER;
	}

	isServer = true;

	// No error occurred
	return CONNECTION_RETURN_SUCCESS;
}

Connection_Return TCPSocket::Connect(const char* address, socket_port_t port, TCPSocketAsynchronousOperation* asynchronousOperation)
{
	// Check if the socket is already opened
	if (isOpened)
	{
		return CONNECTION_RETURN_ERROR_ALREADY_OPENED;
	}

	// Define the operation
	std::function<bool(ConnectionAsynchronousOperation* asynchronousOperationObject)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperationObject)
	{
		return static_cast<TCPSocketAsynchronousOperation*>(asynchronousOperationObject)->connectToServer(address, port);
	};

	return performOperation(operation, NULL, 0, TCP_SOCKET_RETURN_ERROR_COULD_NOT_CONNECT_TO_SERVER, asynchronousOperation);
}

Connection_Return TCPSocket::ConnectExtended(const char* address, socket_port_t port, time_interval_t timeoutInterval)
{
	// Define the operation
	std::function<Connection_Return(ConnectionAsynchronousOperation* asynchronousOperation)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperation)
	{
		return Connect(address, port, static_cast<TCPSocketAsynchronousOperation*>(asynchronousOperation));
	};

	return performOperationExtended(operation, NULL, 0, timeoutInterval);
}

Connection_Return TCPSocket::AcceptClient(TCPSocket* clientSocket, TCPSocketAsynchronousOperation* asynchronousOperation)
{
	// Check if the TCP socket is opened
	if (!IsOpened())
	{
		return CONNECTION_RETURN_ERROR_NOT_OPENED;
	}

	// Check if the TCP socket is a server
	if (!isServer)
	{
		return TCP_SOCKET_RETURN_ERROR_SERVER_ONLY;
	}

	// Check if the client socket is already opened
	if (clientSocket->IsOpened())
	{
		return CONNECTION_RETURN_ERROR_ALREADY_OPENED;
	}

	// Define the operation
	std::function<bool(ConnectionAsynchronousOperation* asynchronousOperationObject)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperationObject)
	{
		return static_cast<TCPSocketAsynchronousOperation*>(asynchronousOperationObject)->acceptClient(clientSocket);
	};

	return performOperation(operation, NULL, 0, TCP_SOCKET_RETURN_ERROR_COULD_NOT_ACCEPT_CLIENT, asynchronousOperation);
}

Connection_Return TCPSocket::AcceptClientExtended(TCPSocket* clientSocket, time_interval_t timeoutInterval)
{
	// Define the operation
	std::function<Connection_Return(ConnectionAsynchronousOperation* asynchronousOperation)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperation)
	{
		return AcceptClient(clientSocket, static_cast<TCPSocketAsynchronousOperation*>(asynchronousOperation));
	};

	return performOperationExtended(operation, NULL, 0, timeoutInterval);
}

TCPSocketAsynchronousOperation* TCPSocket::CreateAsynchronousOperation()
{
	return new TCPSocketAsynchronousOperation(this);
}

int TCPSocket::getSocketType()
{
	return SOCK_STREAM;
}

int TCPSocket::getSocketProtocol()
{
	return IPPROTO_TCP;
}

void TCPSocket::reset()
{
	isServer = false;

	Socket::reset();
}