#pragma once

#include "VehicleCommandMoveBy.h"

/**
 * @brief	Command to move forward
 */
class VehicleCommandMoveForward : public VehicleCommandMoveBy
{
public:

	/**
	 * @brief	The constructor of VehicleCommandMoveForward
	 *
	 * @param	movementForward		Movement in heading direction [m]
	 */
	VehicleCommandMoveForward(float movementForward);

	virtual const char* GetName() override;
};