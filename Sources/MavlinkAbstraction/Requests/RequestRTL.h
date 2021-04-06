#pragma once

#include "MavlinkAbstraction/Responses/ResponseRTL.h"
#include "RequestChangeMode.h"

/**
 * @brief	Changes mode to RTL
 */
class RequestRTL : public RequestChangeMode
{
protected:

	virtual float getMode() override;

public:

	/**
	 * @brief	The constructor of RequestRTL
	 *
	 * @param[in]	mavlinkAbstraction	Mavlink Abstraction this requests belongs to
	 * @param[in]	c2Handler			C2 Handler this requests belongs to
	 * @param[in]	logger				Logger
	 * @param[out]	response			Response. May be NULL
	 */
	RequestRTL(MavlinkAbstraction* mavlinkAbstraction, C2Handler* c2Handler, Logger* logger, ResponseRTL* response);

	/**
	 * @brief	The destructor of RequestRTL.
	 */
	virtual ~RequestRTL();

	virtual const char* GetName() override;
};