#pragma once

#include "TestVehicleCommandCollection.h"

/**
 * @brief	A command that performs the following series of commands automatically:
 *				Guided
 *				Arm
 *				Takeoff
 *				Descend
 *				Ascend
 *				Land
 */
class TestVehicleCommandMission2 : public TestVehicleCommandCollection
{
private:

	constexpr static const float TAKE_OFF_ALTITUDE = 1.5f;
	constexpr static const float UP_DOWN_MOVEMENT = 0.75f;

protected:

	virtual const std::vector<VehicleCommand*> getCommands(Vehicle* vehicle) override;

public:

	virtual const char* GetName() override;

	virtual const char* GetDescription() override;
};