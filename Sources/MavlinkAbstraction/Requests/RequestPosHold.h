#pragma once

#include "MavlinkAbstraction/Responses/ResponsePosHold.h"
#include "RequestChangeMode.h"

/**
 * @brief	Changes mode to POSHOLD
 */
class RequestPosHold : public RequestChangeMode
{
protected:

	virtual float getMode() override;

public:

	/**
	 * @brief	The constructor of RequestPosHold
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 */
	RequestPosHold(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponsePosHold* response);

	/**
	 * @brief	The destructor of RequestPosHold.
	 */
	virtual ~RequestPosHold();

	virtual const char* GetName() override;
};