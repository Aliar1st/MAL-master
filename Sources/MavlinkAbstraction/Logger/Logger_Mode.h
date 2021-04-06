#pragma once

/**
 * @brief	Defines the type of data that shall be logged
 */
enum class Logger_Mode
{
	NONE, // No data is logged
	ERRORS, // Errors are logged
	STATUS, // Errors and status messages are logged
	INFO // Everything is logged
};