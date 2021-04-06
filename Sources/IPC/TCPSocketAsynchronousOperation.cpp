#include "TCPSocketAsynchronousOperation.h"

#include "TCPSocket.h"

#ifdef _WIN32

#include <mswsock.h>
#pragma comment(lib,"mswsock.lib") // Link mswsock library

#endif // _WIN32

TCPSocketAsynchronousOperation::TCPSocketAsynchronousOperation(TCPSocket* socket)
	: SocketAsynchronousOperation(socket)
{
	currentOperation = TCP_Socket_Asynchronous_Operation_Current_Operation::NONE;
}

bool TCPSocketAsynchronousOperation::connectToServer(const char* address, socket_port_t port)
{
	currentOperation = TCP_Socket_Asynchronous_Operation_Current_Operation::CONNECTING;

	// Create the address object
	SocketAsynchronousOperation::convertToSockaddr(address, port, &connectSocketAddress);

#ifdef _WIN32
	// Open the socket dirtily to create the internal socket object
	// ConnectEx requires that the socket is bound so we just bind it to port 0
	if (!static_cast<TCPSocket*>(socket)->openBoundDirtily(0))
	{
		return false;
	}

	// Get the function pointer to ConnectEx
	LPFN_CONNECTEX ConnectEx = NULL;

	GUID connectExID = WSAID_CONNECTEX;
	DWORD outputBytes = 0;
	BOOL connectResult = WSAIoctl(
		static_cast<TCPSocket*>(socket)->internSocket,  // TCP socket
		SIO_GET_EXTENSION_FUNCTION_POINTER,             // We want a function pointer
		&connectExID,                                   // ID for the function we want
		sizeof(connectExID),                            // Size of the ID
		&ConnectEx,                                     // Variable to store the function pointer in
		sizeof(ConnectEx),                              // Size of the variable to store the function pointer in
		&outputBytes,                                   // Afterwards: Actual size of the function pointer
		NULL,                                           // No overlapping
		NULL                                            // Routine to call when the operation has been completed
	);

	// Check if the pointer was acquired
	if (connectResult)
	{
		static_cast<TCPSocket*>(socket)->closeDirtily();

		return false;
	}

	// Connect to the server
	connectResult = ConnectEx(
		static_cast<TCPSocket*>(socket)->internSocket,  // TCP socket
		(const struct sockaddr*) &connectSocketAddress, // Address of the server
		sizeof(connectSocketAddress),                   // Size of the address of the server
		NULL,                                           // Buffer to send after a connection is established
		0,                                              // Size of the buffer to send after a connection is established
		NULL,                                           // Amount of bytes send after the connection is established
		&overlapped                                     // Use overlapping
	);

	if (connectResult && WSAGetLastError() != WSA_IO_PENDING)
	{
		static_cast<TCPSocket*>(socket)->closeDirtily();

		return false;
	}

	return true;
#elif __linux__
	// Open the socket dirtily to create the internal socket object
	if (!static_cast<TCPSocket*>(socket)->openDirtily())
	{
		return false;
	}

	assignWritingAsynchronousFunction(
		[this]
	{
		return connect(static_cast<TCPSocket*>(socket)->internSocket, (const sockaddr*) &connectSocketAddress, sizeof(connectSocketAddress));
	});

	// On Linux we can't check for errors here
	return true;
#endif // __linux__
}

bool TCPSocketAsynchronousOperation::acceptClient(TCPSocket* clientSocket)
{
	currentOperation = TCP_Socket_Asynchronous_Operation_Current_Operation::ACCEPTING;
	this->clientSocket = clientSocket;

	// Open the socket dirtily to create the internal socket object
	if (!clientSocket->openDirtily())
	{
		return false;
	}

#ifdef _WIN32
	// Read from the socket
	BOOL acceptResult = AcceptEx(
		static_cast<TCPSocket*>(socket)->internSocket,  // Server socket
		clientSocket->internSocket,                     // Client socket
		acceptBuffer,                                   // Buffer in which the addresses are stored
		0,                                              // Length of the address buffer to use for actual data
		sizeof(SOCKADDR_IN) + 16,                       // Size of the address buffer to use for the local address
		sizeof(SOCKADDR_IN) + 16,                       // Size of the address buffer to use for the remote address
		&acceptBufferBytesWritten,                      // Amount of bytes received
		&overlapped                                     // Use overlapping
	);

	if (acceptResult && WSAGetLastError() != WSA_IO_PENDING)
	{
		clientSocket->closeDirtily();

		return false;
	}

	return true;
#elif __linux__
	assignReadingAsynchronousFunction(
		[this]
	{
		this->clientSocket->internSocket = accept(static_cast<TCPSocket*>(socket)->internSocket, NULL, NULL);

		// Error handling
		if (this->clientSocket->internSocket == -1)
		{
			return -1;
		}

		return 0;
	});

	// On Linux we can't check for errors here
	return true;
#endif // __linux__
}

bool TCPSocketAsynchronousOperation::writeToConnection(const void* buffer, transfer_size_t bytesToWrite)
{
	currentOperation = TCP_Socket_Asynchronous_Operation_Current_Operation::WRITING;

#ifdef _WIN32
	// Fill the send buffer
	sendRecvBuffer.buf = static_cast<CHAR*>(const_cast<void*>(buffer));
	sendRecvBuffer.len = bytesToWrite;

	// Write to the socket
	BOOL sendResult = WSASend(
		static_cast<TCPSocket*>(socket)->internSocket,  // SOCKET object
		&sendRecvBuffer,                                // Send buffer
		1,                                              // Length of the send buffer
		NULL,                                           // Number of bytes sent (This has to be set to NULL because this TCP sockets works asynchronously)
		0,                                              // Use no specific flags
		&overlapped,                                    // Use overlapping
		NULL                                            // Routine to call when the operation has been completed
	);

	return !sendResult || WSAGetLastError() == WSA_IO_PENDING;
#elif __linux__
	assignWritingAsynchronousFunction(
		[this, buffer, bytesToWrite]
	{
		return send(static_cast<TCPSocket*>(socket)->internSocket, buffer, bytesToWrite, 0);
	});

	// On Linux we can't check for errors here
	return true;
#endif // __linux__
}

bool TCPSocketAsynchronousOperation::readFromConnection(void* buffer, transfer_size_t bytesToRead)
{
	currentOperation = TCP_Socket_Asynchronous_Operation_Current_Operation::READING;

#ifdef _WIN32
	// Fill the recv buffer
	sendRecvBuffer.buf = static_cast<CHAR*>(buffer);
	sendRecvBuffer.len = bytesToRead;

	// Reset the flags
	receiveFlags = 0;

	// Read from the socket
	BOOL recvResult = WSARecv(
		static_cast<TCPSocket*>(socket)->internSocket,  // SOCKET object
		&sendRecvBuffer,                                // Receive buffer
		1,                                              // Length of the receive buffer
		NULL,                                           // Number of bytes received (This has to be set to NULL because this TCP socket works asynchronously)
		&receiveFlags,                                  // Use no specific flags (This has to be a pointer to an existing variable because WSA signals if there is more data with this variable)
		&overlapped,                                    // Use overlapping
		NULL                                            // Routine to call when the operation has been completed
	);

	return !recvResult || WSAGetLastError() == WSA_IO_PENDING;
#elif __linux__
	assignReadingAsynchronousFunction(
		[this, buffer, bytesToRead]
	{
		return recv(static_cast<TCPSocket*>(socket)->internSocket, buffer, bytesToRead, 0);
	});

	// On Linux we can't check for errors here
	return true;
#endif // __linux__
}

void TCPSocketAsynchronousOperation::onOperationCompleted(bool completedSuccessfully)
{
	switch (currentOperation)
	{
		case TCP_Socket_Asynchronous_Operation_Current_Operation::CONNECTING:
		{
			if (completedSuccessfully)
			{
				// Set the client's variables correctly
				static_cast<TCPSocket*>(socket)->isOpened = true;
				static_cast<TCPSocket*>(socket)->isOpenedDirtily = false;
				static_cast<TCPSocket*>(socket)->isServer = false;
			}
			else
			{
				static_cast<TCPSocket*>(socket)->closeDirtily();
			}

			break;
		}

		case TCP_Socket_Asynchronous_Operation_Current_Operation::ACCEPTING:
		{
			if (completedSuccessfully)
			{
				// Set the client's variables correctly
				static_cast<TCPSocket*>(clientSocket)->isOpened = true;
				static_cast<TCPSocket*>(clientSocket)->isOpenedDirtily = false;
				static_cast<TCPSocket*>(clientSocket)->isServer = false;
			}
			else
			{
				static_cast<TCPSocket*>(clientSocket)->closeDirtily();
			}

			break;
		}
	}

	currentOperation = TCP_Socket_Asynchronous_Operation_Current_Operation::NONE;
}