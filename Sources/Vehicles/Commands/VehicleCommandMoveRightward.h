#pragma once

#include "VehicleCommandMoveBy.h"

/**
 * @brief	Command to move rightward
 */
class VehicleCommandMoveRightward : public VehicleCommandMoveBy
{
public:

	/**
	 * @brief	The constructor of VehicleCommandMoveRightward
	 *
	 * @param	movementRightward	Movement rightwards [m]
	 */
	VehicleCommandMoveRightward(float movementRightward);

	virtual const char* GetName() override;
};