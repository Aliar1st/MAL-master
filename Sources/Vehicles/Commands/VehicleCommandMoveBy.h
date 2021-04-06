#pragma once

#include "VehicleCommand.h"

/**
 * @brief	Command to move by an offset
 */
class VehicleCommandMoveBy : public VehicleCommand
{
private:

	constexpr static const float HORIZONTAL_MOVEMENT_THRESHOLD = 0.25f;
	constexpr static const float VERTICAL_MOVEMENT_THRESHOLD = 0.25f;
	constexpr static const float ROTATION_THRESHOLD = 2.5f;

	float movementForward;
	float movementRightward;
	float movementUpward;
	float rotation;

protected:

	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) override;

public:

	/**
	 * @brief	The constructor of VehicleCommandMoveBy
	 *
	 * @param	movementForward		Movement in heading direction [m]
	 * @param	movementRightward	Movement rightwards [m]
	 * @param	movementUpward		Movement upwards [m]
	 * @param	rotation			Heading rotation offset [deg] (clockwise)
	 */
	VehicleCommandMoveBy(float movementForward, float movementRightward, float movementUpward, float rotation);

	virtual const char* GetName() override;
};