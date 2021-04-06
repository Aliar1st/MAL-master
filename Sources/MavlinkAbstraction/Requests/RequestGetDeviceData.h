#pragma once

#include <vector>
#include "MavlinkAbstraction/Responses/ResponseGetDeviceDataBase.h"
#include "RequestCommand.h"
#include "RequestGetDeviceDataBase.h"

/**
 * @brief	Gets data of the MAVLink device
 */
class RequestGetDeviceData : public RequestCommand
{
private:

	constexpr static const unsigned int REQUESTED_MESSAGES_TIMEOUT = 250;

	std::vector<RequestGetDeviceDataBase*> requests;

	bool* responseReceived;
	bool allResponsesReceived;
	std::condition_variable_any allResponsesReceivedCondition;

	/**
	 * @brief	Marks the response as received and checks whether all responses were received
	 *
	 * @param	index	Index of the received response in the response received array
	 */
	void receivedResponse(size_t index);

protected:

	virtual void getCommandValues(size_t index, uint16_t* commandID, float params[7]) override;

public:

	/**
	 * @brief	The constructor of RequestGetDeviceData
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	responses			Responses. May be NULL
	 */
	RequestGetDeviceData(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, const std::vector<ResponseGetDeviceDataBase*>& responses);

	/**
	 * @brief	The destructor of RequestGetDeviceData.
	 */
	virtual ~RequestGetDeviceData();

	virtual void Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID) override;

	virtual void OnMAVLinkMessageReceived(mavlink_message_t* message) override;

	virtual const char* GetName() override;
};