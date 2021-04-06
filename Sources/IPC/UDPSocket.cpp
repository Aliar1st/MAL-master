#include "UDPSocket.h"

#include "Utilities/StringUtil.h"

UDPSocket::UDPSocket()
{
	sourceAddress = NULL;
	targetAddress = NULL;

	reset();
}

UDPSocket::~UDPSocket()
{
	if (IsOpened())
	{
		Close();
	}
}

Connection_Return UDPSocket::Open()
{
	return Socket::open();
}

Connection_Return UDPSocket::OpenBound(socket_port_t port)
{
	return Socket::openBound(port);
}

UDPSocketAsynchronousOperation* UDPSocket::CreateAsynchronousOperation()
{
	return new UDPSocketAsynchronousOperation(this);
}

void UDPSocket::GetSourceAddress(char** sourceAddress, socket_port_t* sourcePort)
{
	// Read the source address
	std::unique_lock<std::mutex> uniqueSourceAddressLock(sourceAddressMutex);

	if (sourceAddress)
	{
		*sourceAddress = StringUtil::Strcpy(this->sourceAddress);
	}

	if (sourcePort)
	{
		*sourcePort = this->sourcePort;
	}
}

Connection_Return UDPSocket::SetTargetAddress(const char* targetAddress, socket_port_t targetPort)
{
	// Check if the connection is opened
	if (!IsOpened())
	{
		return CONNECTION_RETURN_ERROR_NOT_OPENED;
	}

	delete[] this->targetAddress;
	this->targetAddress = StringUtil::Strcpy(targetAddress);
	this->targetPort = targetPort;

	return CONNECTION_RETURN_SUCCESS;
}

void UDPSocket::GetTargetAddress(char** targetAddress, socket_port_t* targetPort)
{
	if (targetAddress)
	{
		*targetAddress = StringUtil::Strcpy(this->targetAddress);
	}

	if (targetPort)
	{
		*targetPort = this->targetPort;
	}
}

int UDPSocket::getSocketType()
{
	return SOCK_DGRAM;
}

int UDPSocket::getSocketProtocol()
{
	return IPPROTO_UDP;
}

void UDPSocket::reset()
{
	delete[] sourceAddress;
	delete[] targetAddress;

	sourceAddress = StringUtil::Strcpy("");
	sourcePort = 0;
	targetAddress = StringUtil::Strcpy("");
	targetPort = 0;

	Socket::reset();
}