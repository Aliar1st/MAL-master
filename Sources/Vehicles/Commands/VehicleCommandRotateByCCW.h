#pragma once

#include "VehicleCommandMoveBy.h"

/**
 * @brief	Command to rotate counter clockwise by an offset
 */
class VehicleCommandRotateByCCW : public VehicleCommandMoveBy
{
public:

	/**
	 * @brief	The constructor of VehicleCommandRotateByCCW
	 *
	 * @param	rotation	Heading rotation offset [deg]
	 */
	VehicleCommandRotateByCCW(float rotation);

	virtual const char* GetName() override;
};