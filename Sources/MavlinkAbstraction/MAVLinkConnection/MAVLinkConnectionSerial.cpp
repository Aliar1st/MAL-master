#include "MAVLinkConnectionSerial.h"

#include "MavlinkAbstraction/Logger/Logger.h"

MAVLinkConnectionSerial::MAVLinkConnectionSerial(MavlinkAbstraction* mavlinkAbstraction, Logger* logger)
	: MAVLinkConnection(mavlinkAbstraction, logger)
{
}

MAVLinkConnectionSerial::~MAVLinkConnectionSerial()
{
	Disconnect();
}

bool MAVLinkConnectionSerial::connect(MavlinkAbstractionArguments& arguments)
{
	*logger << logger->StartLogging(Logger_Mode::INFO) << "Opening serial port \"" << arguments.serialPortName << "\" with a baud rate of " << arguments.baudRate << logger->EndLogging();

	Connection_Return serialPortResult = serialPort.Open(arguments.serialPortName, arguments.baudRate);

	if (serialPort.IsError(serialPortResult))
	{
		*logger << logger->StartLogging(Logger_Mode::INFO) << "Could not open serial port" << logger->EndLogging();

		return false;
	}

	*logger << logger->StartLogging(Logger_Mode::INFO) << "Serial port opened" << logger->EndLogging();

	return true;
}

void MAVLinkConnectionSerial::disconnect()
{
	serialPort.Close();
}

void MAVLinkConnectionSerial::onMessageReceived()
{
}

Connection* MAVLinkConnectionSerial::getConnection()
{
	return &serialPort;
}