#pragma once

#include "VehicleCommandMoveBy.h"

/**
 * @brief	Command to move backward
 */
class VehicleCommandMoveBackward : public VehicleCommandMoveBy
{
public:

	/**
	 * @brief	The constructor of VehicleCommandMoveBackward
	 *
	 * @param	movementBackward	Movement backwards [m]
	 */
	VehicleCommandMoveBackward(float movementBackward);

	virtual const char* GetName() override;
};