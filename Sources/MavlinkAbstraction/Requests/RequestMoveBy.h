#pragma once

#include "MavlinkAbstraction/Responses/ResponseMoveBy.h"
#include "Request.h"

/**
 * @brief	Moves by an offset
 */
class RequestMoveBy : public Request
{
private:

	ResponseMoveBy* response;

	float movementForward;
	float movementRightward;
	float movementUpward;
	float rotation;

public:

	/**
	 * @brief	The constructor of RequestMoveBy
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 * @param		movementForward		Movement in heading direction [m]
	 * @param		movementRightward	Movement rightwards [m]
	 * @param		movementUpward		Movement upwards [m]
	 * @param		rotation			Heading rotation offset [deg] (clockwise)
	 */
	RequestMoveBy(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseMoveBy* response, float movementForward, float movementRightward, float movementUpward, float rotation);

	/**
	 * @brief	The destructor of RequestMoveBy.
	 */
	virtual ~RequestMoveBy();

	virtual void Execute(std::unique_lock<std::recursive_mutex>& uniqueLock, uint8_t ownSystemID, uint8_t ownComponentID, uint8_t targetSystemID) override;

	virtual const char* GetName() override;
};