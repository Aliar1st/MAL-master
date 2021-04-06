#include "MAVLinkConnectionUDP.h"

#include "MavlinkAbstraction/Logger/Logger.h"
#include "Utilities/StringUtil.h"

MAVLinkConnectionUDP::MAVLinkConnectionUDP(MavlinkAbstraction* mavlinkAbstraction, Logger* logger)
	: MAVLinkConnection(mavlinkAbstraction, logger)
{
}

MAVLinkConnectionUDP::~MAVLinkConnectionUDP()
{
	Disconnect();
}

bool MAVLinkConnectionUDP::connect(MavlinkAbstractionArguments& arguments)
{
	*logger << logger->StartLogging(Logger_Mode::INFO) << "Opening UDP socket at port " << arguments.socketPort << logger->EndLogging();

	Connection_Return udpSocketResult = udpSocket.OpenBound(static_cast<socket_port_t>(arguments.socketPort));

	if (udpSocket.IsError(udpSocketResult))
	{
		*logger << logger->StartLogging(Logger_Mode::INFO) << "Could not open UDP socket" << logger->EndLogging();

		return false;
	}

	*logger << logger->StartLogging(Logger_Mode::INFO) << "UDP socket opened" << logger->EndLogging();

	return true;
}

void MAVLinkConnectionUDP::disconnect()
{
	udpSocket.Close();
}

void MAVLinkConnectionUDP::onMessageReceived()
{
	// Read source address
	char* sourceAddress;
	socket_port_t sourcePort;

	udpSocket.GetSourceAddress(&sourceAddress, &sourcePort);

	// Read target address
	char* targetAddress;
	socket_port_t targetPort;

	udpSocket.GetTargetAddress(&targetAddress, &targetPort);

	// Check if changed
	if (sourcePort != targetPort || StringUtil::Strcmp(sourceAddress, targetAddress))
	{
		// Change target address
		udpSocket.SetTargetAddress(sourceAddress, sourcePort);

		*logger << logger->StartLogging(Logger_Mode::INFO) << "Set UDP port's target address to " << sourceAddress << ":" << sourcePort << logger->EndLogging();
	}

	delete[] targetAddress;
	delete[] sourceAddress;
}

Connection* MAVLinkConnectionUDP::getConnection()
{
	return &udpSocket;
}