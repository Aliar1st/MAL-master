#pragma once

#include "VehicleCommandMoveBy.h"

/**
 * @brief	Command to rotate clockwise by an offset
 */
class VehicleCommandRotateByCW : public VehicleCommandMoveBy
{
public:

	/**
	 * @brief	The constructor of VehicleCommandRotateByCW
	 *
	 * @param	rotation	Heading rotation offset [deg]
	 */
	VehicleCommandRotateByCW(float rotation);

	virtual const char* GetName() override;
};