#pragma once

#include "VehicleCommandMoveBy.h"

/**
 * @brief	Command to move upward
 */
class VehicleCommandMoveUpward : public VehicleCommandMoveBy
{
public:

	/**
	 * @brief	The constructor of VehicleCommandMoveUpward
	 *
	 * @param	movementUpward	Movement upwards [m]
	 */
	VehicleCommandMoveUpward(float movementUpward);

	virtual const char* GetName() override;
};