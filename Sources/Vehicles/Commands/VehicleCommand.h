#pragma once

#include <functional>
#include "Vehicle_Command_Execute_Result.h"
#include "Vehicles/Vehicle.h"

/**
 * @brief	A base command class for a vehicle
 */
class VehicleCommand
{
protected:

	/**
	 * @brief	Executes the command
	 *
	 * @param[in]	vehicle		Vehicle this command performs on
	 *
	 * @return	Vehicle_Command_Execute_Result::SUCCESS					Command executed successfully
	 *			Vehicle_Command_Execute_Result::DENIED					Command was denied by the MAVLink device
	 *			Vehicle_Command_Execute_Result::VEHICLE_NOT_STARTED		Vehicle is not started
	 */
	virtual Vehicle_Command_Execution_Result execute(Vehicle* vehicle) = 0;

	/**
	 * @brief	Waits until the specified condition using the MAVLink device data is satisfied
	 *
	 * @param[in]	vehicle		Vehicle this command performs on
	 * @param[out]	responses	Responses to retrieve; May be empty; May contain NULL.
	 * @param		condition	Returns true if the condition is satisfied; False otherwise
	 *
	 * @return	Vehicle_Command_Execute_Result::SUCCESS					There was no error
	 *			Vehicle_Command_Execute_Result::VEHICLE_NOT_STARTED		Vehicle is not started
	 */
	Vehicle_Command_Execution_Result waitForDeviceDataCondition(Vehicle* vehicle, const std::vector<ResponseGetDeviceDataBase*>& responses, std::function<bool()> condition);

public:

	/**
	 * @brief	The constructor of VehicleCommand
	 */
	VehicleCommand();

	/**
	 * @brief	The destructor of VehicleCommand
	 */
	virtual ~VehicleCommand();

	/**
	 * @brief	Executes the command and, if necessary, prints additional information
	 *
	 * @param[in]	vehicle					Vehicle this command performs on
	 * @param		printName				If true, prints the name of the command before executing it
	 * @param		printExecutionResult	If true, prints the result of the command execution
	 * @param[in]	indent					Indent used for output
	 *
	 * @return	Vehicle_Command_Execute_Result::SUCCESS					Command executed successfully
	 *			Vehicle_Command_Execute_Result::DENIED					Command was denied by the MAVLink device
	 *			Vehicle_Command_Execute_Result::VEHICLE_NOT_STARTED		Vehicle is not started
	 */
	Vehicle_Command_Execution_Result Execute(Vehicle* vehicle, bool printName, bool printExecutionResult, const char* indent = "");

	/**
	 * @brief	Returns the name of the command
	 *
	 * @return	Name
	 */
	virtual const char* GetName() = 0;
};