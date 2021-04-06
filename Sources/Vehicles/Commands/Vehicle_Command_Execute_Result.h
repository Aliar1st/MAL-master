#pragma once

/**
 * @brief	Enumerates all possible result of the execution of a vehicle command
 */
enum class Vehicle_Command_Execution_Result
{
	SUCCESS,
	DENIED,
	VEHICLE_NOT_STARTED
};