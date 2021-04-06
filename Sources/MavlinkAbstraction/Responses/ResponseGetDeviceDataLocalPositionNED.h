#pragma once

#include "DeviceData/DeviceDataLocalPositionNED.h"
#include "ResponseGetDeviceDataBase.h"

/**
 * @brief	A response of the "GetDeviceData" request to retrieve device data about the local position in the NED frame
 */
struct ResponseGetDeviceDataLocalPositionNED : public ResponseGetDeviceDataBase
{
protected:

	virtual RequestGetDeviceDataBase* getRequest(Logger* logger) override;

public:

	DeviceDataLocalPositionNED deviceData;

	/**
	 * @brief	The destructor of ResponseGetDeviceDataLocalPositionNED
	 */
	virtual ~ResponseGetDeviceDataLocalPositionNED() override;
};