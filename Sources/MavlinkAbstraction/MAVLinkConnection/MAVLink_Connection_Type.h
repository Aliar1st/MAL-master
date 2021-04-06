#pragma once

/**
 * @brief	Defines the type of MAVLink connection to use
 */
enum class MAVLink_Connection_Type
{
	SERIAL,
	UDP,
	TCP
};