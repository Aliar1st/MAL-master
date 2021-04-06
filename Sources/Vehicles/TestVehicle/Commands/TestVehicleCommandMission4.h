#pragma once

#include "TestVehicleCommandCollection.h"

/**
 * @brief	A command that performs the following series of commands automatically:
 *				Guided
 *				Arm
 *				Takeoff
 *				Move Forward-Up
 *              Land
 */
class TestVehicleCommandMission4 : public TestVehicleCommandCollection
{
private:

	constexpr static const float TAKE_OFF_ALTITUDE = 0.5f;
	constexpr static const float UP_DOWN_MOVEMENT = 0.5f;
	constexpr static const float FRONT_BACK_MOVEMENT = 0.5f;

protected:

	virtual const std::vector<VehicleCommand*> getCommands(Vehicle* vehicle) override;

public:

	virtual const char* GetName() override;

	virtual const char* GetDescription() override;
};