#include "Socket.h"

#ifdef __linux__

#include <unistd.h>

#endif // __linux__

Socket::Socket()
{
	reset();
}

Socket::~Socket()
{
	if (isOpened)
	{
		Close();
	}
}

Connection_Return Socket::openBound(socket_port_t port)
{
	// Open the socket
	Connection_Return openResult = open();

	if (IsError(openResult))
	{
		return openResult;
	}

	// Reuse the given port
	int reuseAddress = 1;

	if (setsockopt(internSocket, SOL_SOCKET, SO_REUSEADDR, (const char*) &reuseAddress, sizeof(reuseAddress)))
	{
		Close();

		return SOCKET_RETURN_ERROR_COULD_NOT_BIND_TO_PORT;
	}

	// Create the address object
	struct sockaddr_in newAddress;

	newAddress.sin_family = AF_INET;
	newAddress.sin_addr.s_addr = INADDR_ANY;
	newAddress.sin_port = htons(port);

	// Bind to the port
	if (bind(internSocket, (const struct sockaddr*) &newAddress, sizeof(newAddress)))
	{
		Close();

		return SOCKET_RETURN_ERROR_COULD_NOT_BIND_TO_PORT;
	}

	return CONNECTION_RETURN_SUCCESS;
}

Connection_Return Socket::open()
{
	// Check if the socket is already opened
	if (isOpened)
	{
		return CONNECTION_RETURN_ERROR_ALREADY_OPENED;
	}

#ifdef _WIN32
	// Initialize the Winsock DLL version 2.2
	const WORD winsockDLLVersionRequested = MAKEWORD(2, 2);
	WSAData wsaData;

	if (WSAStartup(winsockDLLVersionRequested, &wsaData))
	{
		reset();

		return SOCKET_RETURN_ERROR_COULD_NOT_INITIALIZE_WINSOCK_DLL;
	}

	// Create the socket
	internSocket = WSASocketW(AF_INET, getSocketType(), getSocketProtocol(), NULL, 0, WSA_FLAG_OVERLAPPED);
	if (internSocket == INVALID_SOCKET)
	{
		WSACleanup();
		reset();

		return SOCKET_RETURN_ERROR_COULD_NOT_CREATE_SOCKET;
	}
#elif __linux__
	// Create the socket
	internSocket = socket(AF_INET, getSocketType(), getSocketProtocol());
	if (internSocket == -1)
	{
		reset();

		return SOCKET_RETURN_ERROR_COULD_NOT_CREATE_SOCKET;
	}
#endif // __linux__

	// No error occurred
	isOpened = true;

	return CONNECTION_RETURN_SUCCESS;
}

Connection_Return Socket::Close()
{
	// Check if the socket is opened
	if (!isOpened)
	{
		return CONNECTION_RETURN_ERROR_NOT_OPENED;
	}

#ifdef _WIN32
	// Close the socket
	closesocket(internSocket);

	// Terminate the use of the Winsock DLL
	WSACleanup();
#elif __linux__
	// Close the socket
	close(internSocket);
#endif // __linux__

	reset();

	return CONNECTION_RETURN_SUCCESS;
}

bool Socket::openBoundDirtily(socket_port_t port)
{
	if (!openDirtily())
	{
		return false;
	}

	// Create the address object
	struct sockaddr_in newAddress;

	newAddress.sin_family = AF_INET;
	newAddress.sin_addr.s_addr = INADDR_ANY;
	newAddress.sin_port = htons(port);

	// Bind to the port
	if (bind(internSocket, (const struct sockaddr*) &newAddress, sizeof(newAddress)))
	{
		closeDirtily();

		return false;
	}

	return true;
}

bool Socket::openDirtily()
{
	// Make sure the socket is not opened already
	if (isOpened)
	{
		return false;
	}

	// Close the socket if it is already opened dirtily
	closeDirtily();

#ifdef _WIN32
	// Initialize the Winsock DLL version 2.2
	const WORD winsockDLLVersionRequested = MAKEWORD(2, 2);
	WSAData wsaData;

	if (WSAStartup(winsockDLLVersionRequested, &wsaData))
	{
		reset();

		return false;
	}

	// Create the socket
	internSocket = WSASocketW(AF_INET, getSocketType(), getSocketProtocol(), NULL, 0, WSA_FLAG_OVERLAPPED);
	if (internSocket == INVALID_SOCKET)
	{
		WSACleanup();
		reset();

		return false;
	}
#elif __linux__
	// Create the socket
	internSocket = socket(AF_INET, getSocketType(), getSocketProtocol());
	if (internSocket == -1)
	{
		reset();

		return false;
	}
#endif // __linux__

	isOpenedDirtily = true;

	return true;
}

bool Socket::closeDirtily()
{
	if (!isOpenedDirtily)
	{
		return false;
	}

#ifdef _WIN32
	// Close the socket
	closesocket(internSocket);

	// Terminate the use of the Winsock DLL
	WSACleanup();
#elif __linux__
	// Close the socket
	close(internSocket);
#endif // __linux__

	reset();

	return true;
}

void Socket::reset()
{
	// Set the opened state
	isOpenedDirtily = false;
	isOpened = false;

	Connection::reset();
}