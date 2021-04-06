#pragma once

#include "VehicleCommandMoveBy.h"

/**
 * @brief	Command to move downward
 */
class VehicleCommandMoveDownward : public VehicleCommandMoveBy
{
public:

	/**
	 * @brief	The constructor of VehicleCommandMoveDownward
	 *
	 * @param	movementDownward	Movement downwards [m]
	 */
	VehicleCommandMoveDownward(float movementDownward);

	virtual const char* GetName() override;
};