#pragma once

#include "DeviceData/DeviceDataHeartbeat.h"
#include "ResponseGetDeviceDataBase.h"

/**
 * @brief	A response of the "GetDeviceData" request to retrieve device data about the heartbeat
 */
struct ResponseGetDeviceDataHeartbeat : public ResponseGetDeviceDataBase
{
protected:

	virtual RequestGetDeviceDataBase* getRequest(Logger* logger) override;

public:

	DeviceDataHeartbeat deviceData;

	/**
	 * @brief	The destructor of ResponseGetDeviceDataHeartbeat
	 */
	virtual ~ResponseGetDeviceDataHeartbeat() override;
};