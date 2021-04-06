#pragma once

#include "MavlinkAbstraction/Responses/ResponseAltHold.h"
#include "RequestChangeMode.h"

/**
 * @brief	Changes mode to ALTHOLD
 */
class RequestAltHold : public RequestChangeMode
{
protected:

	virtual float getMode() override;

public:

	/**
	 * @brief	The constructor of RequestAltHold
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 */
	RequestAltHold(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseAltHold* response);

	/**
	 * @brief	The destructor of RequestAltHold.
	 */
	virtual ~RequestAltHold();

	virtual const char* GetName() override;
};