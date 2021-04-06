#pragma once

#include <condition_variable>
#include <stddef.h>
#include "Request.h"

/**
 * @brief	A command request to the Mavlink Abstraction with the functionality to wait for an acknowledgement
 *			If no acknowledgement is received in time, the corresponding command will be resend
 *
 *			TODO: What happens if we don't receive the acknowledgement in time and re-send the command? Will it be handled twice? There are in any case two acknowledgements
 */
class RequestCommand : public Request
{
private:

	constexpr static const unsigned int ACKNOWLEDGEMENT_TIMEOUT = 250;

	bool succeeded;
	bool acknowledgementReceived;
	std::condition_variable_any acknowledgementReceivedCondition;

protected:

	/**
	 * @brief	Retrieves the MAVLink command values
	 *
	 * @param		index		Index of the command. Used to allow sending multiple commands of different types
	 * @param[out]	commandID	Command ID
	 * @param[out]	params		Parameters of the command
	 */
	virtual void getCommandValues(size_t index, uint16_t* commandID, float params[7]) = 0;

	/**
	 * @brief	Executes the command. If no acknowledgement is received in time, the command is resend
	 *
	 * @param[in]	uniqueLock		Unique lock used by the Mavlink Abstraction. Unlocking this allows other threads to use the Mavlink Abstraction and is required for OnMAVLinkMessageReceived to be called
	 * @param		ownSystemID		Own system ID
	 * @param		componentID		Own component ID
	 * @param		targetSystemID	Target's system ID
	 * @param		index			Index of the command. Used to allow sending multiple commands of different types. This parameter is directly passed to getCommandValues().
	 *
	 * @return	True if the command was successfully executed; False otherwise
	 */
	bool executeCommand(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID, size_t index);

public:

	/**
	 * @brief	The constructor of RequestCommand
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 */
	RequestCommand(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger);

	/**
	 * @brief	The destructor of RequestCommand.
	 */
	virtual ~RequestCommand();

	virtual void OnMAVLinkMessageReceived(mavlink_message_t* message) override;
};