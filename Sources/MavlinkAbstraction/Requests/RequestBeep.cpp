#include "RequestBeep.h"

#include "MavlinkAbstraction/C2Handler/C2Handler.h"

RequestBeep::RequestBeep(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger)
	: Request(mavlinkAbstraction, c2Handler, logger)
{
}

RequestBeep::~RequestBeep()
{
}

void RequestBeep::Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID)
{
	// Define the message
	mavlink_message_t* message = new mavlink_message_t;

	mavlink_msg_play_tune_pack(
		ownSystemID,        // Own system ID
		ownComponentID,     // Own component ID
		message,            // MAVLink message to pack the data into
		targetSystemID,     // Target's system ID
		0,                  // Target's component ID (broadcast)
		"A4",               // Tune in board specific format
		""                  // Tune extension (appended to tune)
	);

	// Send the message
	c2Handler->SendMAVLinkMessage(message);
}

const char* RequestBeep::GetName()
{
	return "Beep";
}