#pragma once

#include "DeviceData/DeviceDataGlobalPosition.h"
#include "ResponseGetDeviceDataBase.h"

/**
 * @brief	A response of the "GetDeviceData" request to retrieve device data about the global position
 */
struct ResponseGetDeviceDataGlobalPosition : public ResponseGetDeviceDataBase
{
protected:

	virtual RequestGetDeviceDataBase* getRequest(Logger* logger) override;

public:

	DeviceDataGlobalPosition deviceData;

	/**
	 * @brief	The destructor of ResponseGetDeviceDataGlobalPosition
	 */
	virtual ~ResponseGetDeviceDataGlobalPosition() override;
};