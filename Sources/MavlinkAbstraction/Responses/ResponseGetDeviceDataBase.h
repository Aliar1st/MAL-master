#pragma once

#include "MavlinkAbstraction/Requests/RequestGetDeviceDataBase.h"
#include "ResponseSuccess.h"

/**
 * @brief	Base structure for a response of the "GetDeviceData" request
 */
struct ResponseGetDeviceDataBase : public ResponseSuccess
{
	friend class RequestGetDeviceData;

protected:

	/**
	 * @brief	Creates the request object corresponding
	 *
	 * @param[in]	logger	Logger
	 *
	 * @return	Request. This object is allocated using new
	 */
	virtual RequestGetDeviceDataBase* getRequest(Logger* logger) = 0;

public:

	/**
	 * @brief	The destructor of ResponseGetDeviceDataBase
	 */
	virtual ~ResponseGetDeviceDataBase();
};