#include "UDPSocketAsynchronousOperation.h"

#include "UDPSocket.h"
#include "Utilities/StringUtil.h"

UDPSocketAsynchronousOperation::UDPSocketAsynchronousOperation(UDPSocket* socket)
	: SocketAsynchronousOperation(socket)
{
	currentOperation = UDP_Socket_Asynchronous_Operation_Current_Operation::NONE;
}

bool UDPSocketAsynchronousOperation::writeToConnection(const void* buffer, transfer_size_t bytesToWrite)
{
	currentOperation = UDP_Socket_Asynchronous_Operation_Current_Operation::WRITING;

	// Create the target address object
	SocketAsynchronousOperation::convertToSockaddr(static_cast<UDPSocket*>(socket)->targetAddress, static_cast<UDPSocket*>(socket)->targetPort, &sendRecvSocketAddress);

#ifdef _WIN32
	// Fill the send buffer
	sendRecvBuffer.buf = static_cast<CHAR*>(const_cast<void*>(buffer));
	sendRecvBuffer.len = bytesToWrite;

	// Write to the socket
	BOOL sendResult = WSASendTo(
		static_cast<UDPSocket*>(socket)->internSocket,  // SOCKET object
		&sendRecvBuffer,                                // Send buffer
		1,                                              // Length of the send buffer
		NULL,                                           // Number of bytes sent (This has to be set to NULL because this UDP sockets works asynchronously)
		0,                                              // Use no specific flags
		(SOCKADDR*) &sendRecvSocketAddress,             // Address of the target socket
		sizeof(sendRecvSocketAddress),                  // Size of the address of the target socket
		&overlapped,                                    // Use overlapping
		NULL                                            // Routine to call when the operation has been completed
	);

	return !sendResult || WSAGetLastError() == WSA_IO_PENDING;
#elif __linux__
	assignWritingAsynchronousFunction(
		[this, buffer, bytesToWrite]
	{
		return sendto(static_cast<UDPSocket*>(socket)->internSocket, buffer, bytesToWrite, 0, (const struct sockaddr*) &sendRecvSocketAddress, sizeof(sendRecvSocketAddress));
	});

	// On Linux we can't check for errors here
	return true;
#endif // __linux__
}

bool UDPSocketAsynchronousOperation::readFromConnection(void* buffer, transfer_size_t bytesToRead)
{
	currentOperation = UDP_Socket_Asynchronous_Operation_Current_Operation::READING;

	// Setup the source socket data
	sizeOfSendRecvSocketAddress = sizeof(sendRecvSocketAddress);

#ifdef _WIN32
	// Fill the recv buffer
	sendRecvBuffer.buf = static_cast<CHAR*>(buffer);
	sendRecvBuffer.len = bytesToRead;

	// Reset the flags
	receiveFlags = 0;

	// Read from the socket
	BOOL recvResult = WSARecvFrom(
		static_cast<UDPSocket*>(socket)->internSocket,  // SOCKET object
		&sendRecvBuffer,                                // Receive buffer
		1,                                              // Length of the receive buffer
		NULL,                                           // Number of bytes received (This has to be set to NULL because this UDP socket works asynchronously)
		&receiveFlags,                                  // Use no specific flags (This has to be a pointer to an existing variable because WSA signals if there is more data with this variable)
		(SOCKADDR*) &sendRecvSocketAddress,             // Address of the source socket
		&sizeOfSendRecvSocketAddress,                   // Size of the address of the source socket
		&overlapped,                                    // Use overlapping
		NULL                                            // Routine to call when the operation has been completed
	);

	return !recvResult || WSAGetLastError() == WSA_IO_PENDING;
#elif __linux__
	assignReadingAsynchronousFunction(
		[this, buffer, bytesToRead]
	{
		return recvfrom(static_cast<UDPSocket*>(socket)->internSocket, buffer, bytesToRead, 0, (struct sockaddr*) &sendRecvSocketAddress, &sizeOfSendRecvSocketAddress);
	});

	// On Linux we can't check for errors here
	return true;
#endif // __linux__
}

void UDPSocketAsynchronousOperation::onOperationCompleted(bool completedSuccessfully)
{
	switch (currentOperation)
	{
		case UDP_Socket_Asynchronous_Operation_Current_Operation::READING:
		{
			if (completedSuccessfully)
			{
				// Modify the source address
				std::unique_lock<std::mutex> uniqueSourceAddressLock(static_cast<UDPSocket*>(socket)->sourceAddressMutex);

				delete[] static_cast<UDPSocket*>(socket)->sourceAddress;
				extractFromSockaddr(sendRecvSocketAddress, &(static_cast<UDPSocket*>(socket)->sourceAddress), &(static_cast<UDPSocket*>(socket)->sourcePort));
			}

			break;
		}
	}

	currentOperation = UDP_Socket_Asynchronous_Operation_Current_Operation::NONE;
}