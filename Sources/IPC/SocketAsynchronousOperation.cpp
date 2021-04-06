#include "SocketAsynchronousOperation.h"

#include "Socket.h"
#include "Utilities/ConversionUtil.h"

#ifdef _WIN32

#include <WS2tcpip.h>

#elif __linux__

#include <arpa/inet.h>
#include <netdb.h>

#endif // __linux__

SocketAsynchronousOperation::SocketAsynchronousOperation(Socket* socket)
	: ConnectionAsynchronousOperation(),
	  socket(socket)
{
#ifdef _WIN32
	// Remove the existing event and create a new one
	CloseHandle(overlapped.hEvent);
	overlapped.hEvent = WSACreateEvent();
#endif // _WIN32
}

SocketAsynchronousOperation::~SocketAsynchronousOperation()
{
#ifdef _WIN32
	// Close the event
	WSACloseEvent(overlapped.hEvent);

	// Invalidate the underlying event
	overlapped.hEvent = INVALID_HANDLE_VALUE;
#endif // _WIN32
}

Connection_Asynchronous_Operation_Wait_For_Completion_Return SocketAsynchronousOperation::virtualWaitForCompletion(transfer_size_t* bytesTransferred, bool* timeout)
{
#ifdef _WIN32
	// Return that the operation is finished if the socket is closed
	if (!socket->IsOpened())
	{
		return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS;
	}

	DWORD flags;

	// Wait for the operation to complete
	BOOL success = WSAGetOverlappedResult(
		socket->internSocket,   // SOCKET object
		&overlapped,            // OVERLAPPED structure
		bytesTransferred,       // Bytes transferred
		TRUE,                   // Wait for completion
		&flags                  // Output flags (Required but not used)
	);

	if (!success)
	{
		if (WSAGetLastError() == WSA_OPERATION_ABORTED)
		{
			if (*timeout)
			{
				return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_TIMEOUT;
			}
			else
			{
				return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_CANCELED;
			}
		}
		else
		{
			return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_ERROR;
		}
	}
	else
	{
		return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS;
	}
#elif __linux__
	return ConnectionAsynchronousOperation::virtualWaitForCompletion(bytesTransferred, timeout);
#endif // __linux__
}

Connection_Asynchronous_Operation_Cancel_Return SocketAsynchronousOperation::virtualCancel()
{
#ifdef _WIN32
	// Return that the operation is finished if the socket is closed
	if (!socket->IsOpened())
	{
		return CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_ALREADY_FINISHED;
	}

	// Attempt to cancel the operation
	BOOL success = CancelIoEx((HANDLE) socket->internSocket, &overlapped);

	if (success)
	{
		return CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_CANCELED;
	}

	if (GetLastError() != ERROR_NOT_FOUND)
	{
		return CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_ERROR;
	}

	return CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_ALREADY_FINISHED;
#elif __linux__
	return ConnectionAsynchronousOperation::virtualCancel();
#endif // __linux__
}

bool SocketAsynchronousOperation::virtualIsCompleted()
{
#ifdef _WIN32
	// Return that the operation is finished if the socket is closed
	if (!socket->IsOpened())
	{
		return true;
	}

	DWORD flags;
	DWORD bytesTransferred;

	// Wait for the operation to complete
	return WSAGetOverlappedResult(
		socket->internSocket,   // SOCKET object
		&overlapped,            // OVERLAPPED structure
		&bytesTransferred,      // Bytes transferred
		FALSE,                  // Wait for completion
		&flags                  // Output flags (Required but not used)
	);
#elif __linux__
	return ConnectionAsynchronousOperation::virtualIsCompleted();
#endif // __linux__
}

void SocketAsynchronousOperation::convertToSockaddr(const char* socketAddress, const socket_port_t socketPort, sockaddr_in* socketAddr)
{
	socketAddr->sin_family = AF_INET;

#ifdef _WIN32
#ifdef UNICODE
	wchar_t* convertedAddress = ConversionUtil::StrToWStr(socketAddress);

	InetPton(AF_INET, convertedAddress, &(socketAddr->sin_addr.s_addr));
	delete[] convertedAddress;
#else
	InetPton(AF_INET, socketAddress, &(socketAddr->sin_addr.s_addr));
#endif // UNICODE
#elif __linux__
	inet_pton(AF_INET, socketAddress, &(socketAddr->sin_addr.s_addr));
#endif // __linux__

	socketAddr->sin_port = htons(socketPort);
}

void SocketAsynchronousOperation::extractFromSockaddr(const struct sockaddr_in sockAddr, char** socketAddress, socket_port_t* socketPort)
{
	if (socketAddress)
	{
#ifdef _WIN32
#ifdef UNICODE
		WCHAR localSourceAddress[NI_MAXHOST];

		InetNtop(AF_INET, &sockAddr.sin_addr, localSourceAddress, NI_MAXHOST);
		*socketAddress = ConversionUtil::WStrToStr(localSourceAddress);
#else
		*socketAddress = new char[NI_MAXHOST];
		InetNtop(AF_INET, &sockAddr.sin_addr, *socketAddress, NI_MAXHOST);
#endif // UNICODE
#elif __linux__
		*socketAddress = new char[NI_MAXHOST];
		inet_ntop(AF_INET, &sockAddr.sin_addr, *socketAddress, NI_MAXHOST);
#endif // __linux__
	}

	if (socketPort)
	{
		*socketPort = ntohs(sockAddr.sin_port);
	}
}

#ifdef __linux__

int SocketAsynchronousOperation::getFileDescriptor()
{
	return socket->internSocket;
}

#endif // __linux_