#pragma once

#include "SocketAsynchronousOperation.h"

class UDPSocket;

/**
 * @brief	A class used to detect the status of asynchronous operations in a UDP socket.
 */
class UDPSocketAsynchronousOperation : public SocketAsynchronousOperation
{
	friend class UDPSocket;

private:

	enum class UDP_Socket_Asynchronous_Operation_Current_Operation
	{
		NONE,
		WRITING,
		READING
	};

#ifdef _WIN32
	WSABUF sendRecvBuffer;
	DWORD receiveFlags;
#endif // _WIN32

	UDP_Socket_Asynchronous_Operation_Current_Operation currentOperation;

	struct sockaddr_in sendRecvSocketAddress;
	socklen_t sizeOfSendRecvSocketAddress;

protected:

	/**
	 * @brief	The constructor of UDPSocketAsynchronousOperation.
	 *
	 * @param[in]	socket	The UDP socket
	 */
	UDPSocketAsynchronousOperation(UDPSocket* socket);

	virtual bool writeToConnection(const void* buffer, transfer_size_t bytesToWrite) override;

	virtual bool readFromConnection(void* buffer, transfer_size_t bytesToRead) override;

	virtual void onOperationCompleted(bool completedSuccessfully) override;
};