#include "MavlinkAbstraction.h"

#include "C2Handler/C2Handler.h"
#include "Logger/Logger.h"
#include "MAVLinkConnection/MAVLinkConnectionSerial.h"
#include "MAVLinkConnection/MAVLinkConnectionTCP.h"
#include "MAVLinkConnection/MAVLinkConnectionUDP.h"
#include "Requests/RequestAltHold.h"
#include "Requests/RequestArm.h"
#include "Requests/RequestBeep.h"
#include "Requests/RequestDisarm.h"
#include "Requests/RequestGetDeviceData.h"
#include "Requests/RequestGetTimeOfBoot.h"
#include "Requests/RequestGuided.h"
#include "Requests/RequestLand.h"
#include "Requests/RequestMoveBy.h"
#include "Requests/RequestMoveTo.h"
#include "Requests/RequestPosHold.h"
#include "Requests/RequestRTL.h"
#include "Requests/RequestRebootFlightController.h"
#include "Requests/RequestStabilize.h"
#include "Requests/RequestTakeOff.h"

MavlinkAbstraction::MavlinkAbstraction()
	: logger(new Logger)
{
	reset();
}

MavlinkAbstraction::~MavlinkAbstraction()
{
	// Stop if necessary
	Stop();

	// Delete the sub systems if they exist
	deleteSubSystems();

	delete logger;
}

MavlinkAbstraction_Return MavlinkAbstraction::Start(MavlinkAbstractionArguments& arguments, unsigned int timeout)
{
	// Make this function thread safe
	std::unique_lock<std::recursive_mutex> threadSafetyLock(threadSafetyMutex);

	// Check if started
	if (isStarted)
	{
		return MAVLINK_ABSTRACTION_RETURN_ERROR_ALREADY_STARTED;
	}

	// Delete the sub systems if they exist. We unlock the lock as the sub systems might access the Mavlink Abstraction again in order to fully block their threads
	threadSafetyLock.unlock();
	deleteSubSystems();
	threadSafetyLock.lock();

	isStarted = true;

	// Start the logger
	logger->Start(arguments.loggerMode);
	*logger << logger->StartLogging(Logger_Mode::STATUS) << "Starting Mavlink Abstraction..." << logger->EndLogging();

	// Log the arguments
	logArguments(arguments);

	// Create the MAVLink connection
	switch (arguments.mavlinkConnectionType)
	{
		case MAVLink_Connection_Type::SERIAL:
		{
			mavlinkConnection = new MAVLinkConnectionSerial(this, logger);
			break;
		}

		case MAVLink_Connection_Type::UDP:
		{
			mavlinkConnection = new MAVLinkConnectionUDP(this, logger);
			break;
		}

		case MAVLink_Connection_Type::TCP:
		{
			mavlinkConnection = new MAVLinkConnectionTCP(this, logger);
			break;
		}
	}

	// Connect to a MAVLink device
	if (!mavlinkConnection->Connect(arguments, timeout))
	{
		// Stop the Mavlink Abstraction as there was a timeout
		Stop();

		return MAVLINK_ABSTRACTION_RETURN_ERROR_CONNECTION_TIMEOUT;
	}

	// Create the C2 Handler
	c2Handler = new C2Handler(this, logger);
	c2Handler->Start(arguments);

	*logger << logger->StartLogging(Logger_Mode::STATUS) << "Started Mavlink Abstraction" << logger->EndLogging();

	// Event the event
	threadSafetyLock.unlock();

	if (!onStarted())
	{
		// Stop the Mavlink Abstraction
		Stop();

		return MAVLINK_ABSTRACTION_RETURN_ERROR_ON_STARTED;
	}

	return MAVLINK_ABSTRACTION_RETURN_SUCCESS;
}

MavlinkAbstraction_Return MavlinkAbstraction::Stop()
{
	// Make this function thread safe
	std::unique_lock<std::recursive_mutex> threadSafetyLock(threadSafetyMutex);

	// Check if not started
	if (!isStarted)
	{
		return MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED;
	}

	*logger << logger->StartLogging(Logger_Mode::STATUS) << "Stopping Mavlink Abstraction..." << logger->EndLogging();

	// Stop the sub systems
	if (c2Handler)
	{
		c2Handler->Stop();
	}
	mavlinkConnection->Disconnect();

	*logger << logger->StartLogging(Logger_Mode::STATUS) << "Stopped Mavlink Abstraction" << logger->EndLogging();

	// Stop the logger
	logger->Stop();

	// Reset the Mavlink Abstraction
	reset();

	// Event the event
	threadSafetyLock.unlock();
	onStopped();

	return MAVLINK_ABSTRACTION_RETURN_SUCCESS;
}

MavlinkAbstraction_Return MavlinkAbstraction::AltHold(ResponseAltHold* response)
{
	return handleRequest(new RequestAltHold(this, c2Handler, logger, response));
}

MavlinkAbstraction_Return MavlinkAbstraction::Arm(ResponseArm* response)
{
	return handleRequest(new RequestArm(this, c2Handler, logger, response));
}

MavlinkAbstraction_Return MavlinkAbstraction::Beep()
{
	return handleRequest(new RequestBeep(this, c2Handler, logger));
}

MavlinkAbstraction_Return MavlinkAbstraction::Disarm(ResponseDisarm* response, bool force)
{
	return handleRequest(new RequestDisarm(this, c2Handler, logger, response, force));
}

MavlinkAbstraction_Return MavlinkAbstraction::GetDeviceData(const std::vector<ResponseGetDeviceDataBase*>& responses)
{
	return handleRequest(new RequestGetDeviceData(this, c2Handler, logger, responses));
}

MavlinkAbstraction_Return MavlinkAbstraction::GetTimeOfBoot(ResponseGetTimeOfBoot* response)
{
	return handleRequest(new RequestGetTimeOfBoot(this, c2Handler, logger, response));
}

MavlinkAbstraction_Return MavlinkAbstraction::Guided(ResponseGuided* response)
{
	return handleRequest(new RequestGuided(this, c2Handler, logger, response));
}

MavlinkAbstraction_Return MavlinkAbstraction::Land(ResponseLand* response)
{
	return handleRequest(new RequestLand(this, c2Handler, logger, response));
}

MavlinkAbstraction_Return MavlinkAbstraction::MoveBy(ResponseMoveBy* response, float movementForward, float movementRightward, float movementUpward, float rotation)
{
	return handleRequest(new RequestMoveBy(this, c2Handler, logger, response, movementForward, movementRightward, movementUpward, rotation));
}

MavlinkAbstraction_Return MavlinkAbstraction::MoveTo(ResponseMoveTo* response, int32_t targetLatitude, int32_t targetLongitude, float targetAltitude, float targetHeading)
{
	return handleRequest(new RequestMoveTo(this, c2Handler, logger, response, targetLatitude, targetLongitude, targetAltitude, targetHeading));
}

MavlinkAbstraction_Return MavlinkAbstraction::PosHold(ResponsePosHold* response)
{
	return handleRequest(new RequestPosHold(this, c2Handler, logger, response));
}

MavlinkAbstraction_Return MavlinkAbstraction::RebootFlightController(ResponseRebootFlightController* response)
{
	return handleRequest(new RequestRebootFlightController(this, c2Handler, logger, response));
}

MavlinkAbstraction_Return MavlinkAbstraction::RTL(ResponseRTL* response)
{
	return handleRequest(new RequestRTL(this, c2Handler, logger, response));
}

MavlinkAbstraction_Return MavlinkAbstraction::Stabilize(ResponseStabilize* response)
{
	return handleRequest(new RequestStabilize(this, c2Handler, logger, response));
}

MavlinkAbstraction_Return MavlinkAbstraction::TakeOff(ResponseTakeOff* response, float altitude)
{
	return handleRequest(new RequestTakeOff(this, c2Handler, logger, response, altitude));
}

bool MavlinkAbstraction::IsStarted()
{
	return isStarted;
}

bool MavlinkAbstraction::onStarted()
{
	return true;
}

void MavlinkAbstraction::onStopped()
{
}

void MavlinkAbstraction::onMAVLinkMessageReceived(mavlink_message_t* message)
{
	// Make this function thread safe
	std::unique_lock<std::recursive_mutex> threadSafetyLock(threadSafetyMutex);

	// Check if not started
	if (!isStarted)
	{
		return;
	}

	// Forward the message to the C2 Handler
	c2Handler->OnMAVLinkMessageReceived(message);
}

void MavlinkAbstraction::sendMAVLinkMessage(mavlink_message_t* message)
{
	// Make this function thread safe
	std::unique_lock<std::recursive_mutex> threadSafetyLock(threadSafetyMutex);

	// Check if not started
	if (!isStarted)
	{
		delete message;

		return;
	}

	mavlinkConnection->SendMAVLinkMessage(message);
}

MavlinkAbstraction_Return MavlinkAbstraction::handleRequest(Request* request)
{
	// Disallow handling of two requests at the same time
	std::unique_lock<std::mutex> requestHandlingLock(requestHandlingMutex);

	// Make this function thread safe
	std::unique_lock<std::recursive_mutex> threadSafetyLock(threadSafetyMutex);

	// Check if not started
	if (!isStarted)
	{
		delete request;

		return MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED;
	}

	// Log
	*logger << logger->StartLogging(Logger_Mode::INFO) << "Handling request: " << request->GetName() << logger->EndLogging();

	// Forward request to C2 Handler
	c2Handler->HandleRequest(request, threadSafetyLock);

	// Log
	*logger << logger->StartLogging(Logger_Mode::INFO) << "Handled request: " << request->GetName() << logger->EndLogging();

	// Request is not needed anymore
	delete request;

	// Check if still started (It might not be if we waited for a MAVLink message and in the meantime the Mavlink Abstraction got shut-down)
	if (!isStarted)
	{
		return MAVLINK_ABSTRACTION_RETURN_ERROR_NOT_STARTED;
	}

	return MAVLINK_ABSTRACTION_RETURN_SUCCESS;
}

void MavlinkAbstraction::deleteSubSystems()
{
	delete c2Handler;
	c2Handler = nullptr;

	delete mavlinkConnection;
	mavlinkConnection = nullptr;
}

void MavlinkAbstraction::logArguments(MavlinkAbstractionArguments& arguments)
{
	logger->StartLogging(Logger_Mode::INFO);
	*logger << "Arguments:" << Logger::Endl;

	// Connection type
	switch (arguments.mavlinkConnectionType)
	{
		case MAVLink_Connection_Type::SERIAL:
		{
			*logger << Logger::Indent << "MAVLinkConnectionType: serial" << Logger::Endl;
			*logger << Logger::Indent << "Serial Port Name: " << arguments.serialPortName << Logger::Endl;
			*logger << Logger::Indent << "Baud Rate: " << arguments.baudRate << Logger::Endl;
			break;
		}

		case MAVLink_Connection_Type::UDP:
		{
			*logger << Logger::Indent << "MAVLinkConnectionType: udp" << Logger::Endl;
			*logger << Logger::Indent << "Socket Port: " << arguments.socketPort << Logger::Endl;
			break;
		}

		case MAVLink_Connection_Type::TCP:
		{
			*logger << Logger::Indent << "MAVLinkConnectionType: tcp" << Logger::Endl;
			*logger << Logger::Indent << "Socket Port: " << arguments.socketPort << Logger::Endl;
			break;
		}
	}

	// Logging Mode
	switch (arguments.loggerMode)
	{
		case Logger_Mode::NONE:
		{
			*logger << Logger::Indent << "Logging Mode: none";
			break;
		}

		case Logger_Mode::ERRORS:
		{
			*logger << Logger::Indent << "Logging Mode: error";
			break;
		}

		case Logger_Mode::STATUS:
		{
			*logger << Logger::Indent << "Logging Mode: status";
			break;
		}

		case Logger_Mode::INFO:
		{
			*logger << Logger::Indent << "Logging Mode: info";
			break;
		}
	}

	logger->EndLogging();
}

void MavlinkAbstraction::reset()
{
	isStarted = false;
}