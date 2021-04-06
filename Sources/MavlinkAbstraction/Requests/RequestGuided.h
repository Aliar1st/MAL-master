#pragma once

#include "MavlinkAbstraction/Responses/ResponseGuided.h"
#include "RequestChangeMode.h"

/**
 * @brief	Changes mode to GUIDED
 */
class RequestGuided : public RequestChangeMode
{
protected:

	virtual float getMode() override;

public:

	/**
	 * @brief	The constructor of RequestGuided
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 */
	RequestGuided(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseGuided* response);

	/**
	 * @brief	The destructor of RequestGuided.
	 */
	virtual ~RequestGuided();

	virtual const char* GetName() override;
};