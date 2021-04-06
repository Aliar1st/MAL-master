#pragma once

#include "MavlinkAbstraction/Responses/ResponseLand.h"
#include "RequestChangeMode.h"

/**
 * @brief	Changes mode to LAND
 */
class RequestLand : public RequestChangeMode
{
protected:

	virtual float getMode() override;

public:

	/**
	 * @brief	The constructor of RequestLand
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 */
	RequestLand(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseLand* response);

	/**
	 * @brief	The destructor of RequestLand.
	 */
	virtual ~RequestLand();

	virtual const char* GetName() override;
};