#include "TestVehicleCommandPrintDeviceData.h"

#include <iostream>

const char* TestVehicleCommandPrintDeviceData::GetName()
{
	return "printDeviceData";
}

const std::vector<const char*> TestVehicleCommandPrintDeviceData::GetAlternativeNames()
{
	return {"print"};
}

const char* TestVehicleCommandPrintDeviceData::GetDescription()
{
	return "Prints all data of the MAVLink device";
}

Vehicle_Command_Execution_Result TestVehicleCommandPrintDeviceData::execute(Vehicle* vehicle)
{
	ResponseGetDeviceDataAttitude responseAttitute;
	ResponseGetDeviceDataExtendedSystemState responseExtendedSystemState;
	ResponseGetDeviceDataGlobalPosition responseGlobalPosition;
	ResponseGetDeviceDataGPSGlobalOrigin responseGPSGlobalOrigin;
	ResponseGetDeviceDataHeartbeat responseHeartbeat;
	ResponseGetDeviceDataLocalPositionNED responseLocalPositionNED;

	if (vehicle->GetDeviceData({&responseAttitute, &responseExtendedSystemState, &responseGlobalPosition, &responseGPSGlobalOrigin, &responseHeartbeat, &responseLocalPositionNED}) != MAVLINK_ABSTRACTION_RETURN_SUCCESS)
	{
		return Vehicle_Command_Execution_Result::VEHICLE_NOT_STARTED;
	}

	if (!responseAttitute.success ||
		!responseExtendedSystemState.success ||
		!responseGlobalPosition.success ||
		!responseGPSGlobalOrigin.success ||
		!responseHeartbeat.success ||
		!responseLocalPositionNED.success)
	{
		return Vehicle_Command_Execution_Result::DENIED;
	}

	// Print
	std::cout << INDENT << "Roll: " << responseAttitute.deviceData.roll << std::endl;
	std::cout << INDENT << "Pitch: " << responseAttitute.deviceData.pitch << std::endl;
	std::cout << INDENT << "Yaw: " << responseAttitute.deviceData.yaw << std::endl;
	std::cout << INDENT << "Roll Speed: " << responseAttitute.deviceData.rollSpeed << std::endl;
	std::cout << INDENT << "Pitch Speed: " << responseAttitute.deviceData.pitchSpeed << std::endl;
	std::cout << INDENT << "Yaw Speed: " << responseAttitute.deviceData.yawSpeed << std::endl;

	std::cout << INDENT << "Landed State: " << static_cast<int>(responseExtendedSystemState.deviceData.landedState) << std::endl;

	std::cout << INDENT << "Latitude: " << responseGlobalPosition.deviceData.latitude << std::endl;
	std::cout << INDENT << "Longitude: " << responseGlobalPosition.deviceData.longitude << std::endl;
	std::cout << INDENT << "Altitude: " << responseGlobalPosition.deviceData.altitude << std::endl;
	std::cout << INDENT << "Relative Altitude: " << responseGlobalPosition.deviceData.relativeAltitude << std::endl;
	std::cout << INDENT << "X Speed: " << responseGlobalPosition.deviceData.xSpeed << std::endl;
	std::cout << INDENT << "Y Speed: " << responseGlobalPosition.deviceData.ySpeed << std::endl;
	std::cout << INDENT << "Z Speed: " << responseGlobalPosition.deviceData.zSpeed << std::endl;
	std::cout << INDENT << "Heading: " << responseGlobalPosition.deviceData.heading << std::endl;

	std::cout << INDENT << "Origin Latitude: " << responseGPSGlobalOrigin.deviceData.latitude << std::endl;
	std::cout << INDENT << "Origin Longitude: " << responseGPSGlobalOrigin.deviceData.longitude << std::endl;
	std::cout << INDENT << "Origin Altitude: " << responseGPSGlobalOrigin.deviceData.altitude << std::endl;

	std::cout << INDENT << "Copter Mode: " << responseHeartbeat.deviceData.copterMode << std::endl;
	std::cout << INDENT << "Mode Flags: " << static_cast<unsigned int>(responseHeartbeat.deviceData.modeFlags) << std::endl;

	std::cout << INDENT << "Local NED X: " << responseLocalPositionNED.deviceData.x << std::endl;
	std::cout << INDENT << "Local NED Y: " << responseLocalPositionNED.deviceData.y << std::endl;
	std::cout << INDENT << "Local NED Z: " << responseLocalPositionNED.deviceData.z << std::endl;
	std::cout << INDENT << "Local NED X Speed: " << responseLocalPositionNED.deviceData.xSpeed << std::endl;
	std::cout << INDENT << "Local NED Y Speed: " << responseLocalPositionNED.deviceData.ySpeed << std::endl;
	std::cout << INDENT << "Local NED Z Speed: " << responseLocalPositionNED.deviceData.zSpeed << std::endl;

	return Vehicle_Command_Execution_Result::SUCCESS;
}