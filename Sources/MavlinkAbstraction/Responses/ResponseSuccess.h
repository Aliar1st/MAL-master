#pragma once

#include "Response.h"

/**
 * @brief	A response from the Mavlink Abstraction only containing whether a request was successfully executed
 */
struct ResponseSuccess : public Response
{
	// Whether the corresponding request was successful
	bool success;
};