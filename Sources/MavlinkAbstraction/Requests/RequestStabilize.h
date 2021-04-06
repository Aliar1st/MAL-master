#pragma once

#include "MavlinkAbstraction/Responses/ResponseStabilize.h"
#include "RequestChangeMode.h"

/**
 * @brief	Changes mode to STABILIZE
 */
class RequestStabilize : public RequestChangeMode
{
protected:

	virtual float getMode() override;

public:

	/**
	 * @brief	The constructor of RequestStabilize
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 */
	RequestStabilize(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseStabilize* response);

	/**
	 * @brief	The destructor of RequestStabilize.
	 */
	virtual ~RequestStabilize();

	virtual const char* GetName() override;
};