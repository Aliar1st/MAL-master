#pragma once

#include "VehicleCommandMoveBy.h"

/**
 * @brief	Command to move leftward
 */
class VehicleCommandMoveLeftward : public VehicleCommandMoveBy
{
public:

	/**
	 * @brief	The constructor of VehicleCommandMoveLeftward
	 *
	 * @param	movementLeftward	Movement leftwards [m]
	 */
	VehicleCommandMoveLeftward(float movementLeftward);

	virtual const char* GetName() override;
};