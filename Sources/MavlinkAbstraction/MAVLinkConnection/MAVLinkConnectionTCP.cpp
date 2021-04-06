#include "MAVLinkConnectionTCP.h"

#include "MavlinkAbstraction/Logger/Logger.h"

MAVLinkConnectionTCP::MAVLinkConnectionTCP(MavlinkAbstraction* mavlinkAbstraction, Logger* logger)
	: MAVLinkConnection(mavlinkAbstraction, logger)
{
}

MAVLinkConnectionTCP::~MAVLinkConnectionTCP()
{
	Disconnect();
}

bool MAVLinkConnectionTCP::connect(MavlinkAbstractionArguments& arguments)
{
	Connection_Return tcpSocketResult;

	// Create server socket if needed
	if (!tcpServerSocket.IsOpened())
	{
		*logger << logger->StartLogging(Logger_Mode::INFO) << "Creating TCP server socket at port " << arguments.socketPort << logger->EndLogging();

		// Create server
		tcpSocketResult = tcpServerSocket.CreateServer(static_cast<socket_port_t>(arguments.socketPort));

		if (tcpServerSocket.IsError(tcpSocketResult))
		{
			*logger << logger->StartLogging(Logger_Mode::INFO) << "Could not create TCP server socket" << logger->EndLogging();

			return false;
		}
	}

	// Accept a client if needed
	if (acceptClientAsyncOp == nullptr)
	{
		acceptClientAsyncOp = tcpServerSocket.CreateAsynchronousOperation();
		tcpSocketResult = tcpServerSocket.AcceptClient(&tcpClientSocket, acceptClientAsyncOp);

		if (tcpServerSocket.IsError(tcpSocketResult))
		{
			*logger << logger->StartLogging(Logger_Mode::INFO) << "Could not accept client at TCP server socket" << logger->EndLogging();

			return false;
		}
	}

	// Check if a client is connected
	bool clientConnected = tcpServerSocket.IsOperationCompleted(acceptClientAsyncOp);

	if (!clientConnected)
	{
		*logger << logger->StartLogging(Logger_Mode::INFO) << "No TCP client connected yet" << logger->EndLogging();

		return false;
	}

	// We do not need the server anymore
	delete acceptClientAsyncOp;
	acceptClientAsyncOp = nullptr;
	tcpServerSocket.Close();

	*logger << logger->StartLogging(Logger_Mode::INFO) << "TCP client socket opened" << logger->EndLogging();

	return true;
}

void MAVLinkConnectionTCP::disconnect()
{
	// Close client
	tcpClientSocket.Close();

	// Close server
	delete acceptClientAsyncOp;
	acceptClientAsyncOp = nullptr;
	tcpServerSocket.Close();
}

void MAVLinkConnectionTCP::onMessageReceived()
{
}

Connection* MAVLinkConnectionTCP::getConnection()
{
	return &tcpClientSocket;
}