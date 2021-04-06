#include "Connection.h"

#include <cassert>

Connection::Connection()
{
	reset();
}

Connection::~Connection()
{
	assert(!isOpened); // The connection is still opened but should be closed by now. Use Close() in the subclass' destructor
}

Connection_Return Connection::WaitForOperation(ConnectionAsynchronousOperation* asynchronousOperation, transfer_size_t* bytesTransferred)
{
	return WaitForOperationExtended(asynchronousOperation, bytesTransferred, NO_TIMEOUT);
}

void Connection::waitForOperationExtendedTimerTimeout(bool timePassed, void* data)
{
	// Get the argument
	Connection_WaitForOperationExtended* argument = static_cast<Connection_WaitForOperationExtended*>(data);

	if (timePassed)
	{
		// Cancel the operation and check whether it was completed before
		*(argument->pTimeout) = true;

		if (CancelOperation(argument->asyncOperation) != CONNECTION_RETURN_SUCCESS)
		{
			*(argument->pTimeout) = false;
		}
	}

	// Delete the argument
	delete argument;
}

Connection_Return Connection::WaitForOperationExtended(ConnectionAsynchronousOperation* asynchronousOperation, transfer_size_t* bytesTransferred, time_interval_t timeoutInterval)
{
	// Create a timer if necessary
	Timer* timer = NULL;
	bool timeout = false;

	if (timeoutInterval != NO_TIMEOUT)
	{
		// Create the function pointer and the function argument
		std::function<void(bool timePassed, void* data)> functionPointer = std::bind(&Connection::waitForOperationExtendedTimerTimeout, this, std::placeholders::_1, std::placeholders::_2);
		Connection_WaitForOperationExtended* functionArgument = new Connection_WaitForOperationExtended(&timeout, asynchronousOperation);

		// Start the timer
		timer = new Timer();
		timer->InitializeData(timeoutInterval, false, functionPointer, (void*) functionArgument);
		timer->Start();
	}

	transfer_size_t localBytesTransfered;
	Connection_Asynchronous_Operation_Wait_For_Completion_Return result = asynchronousOperation->waitForCompletion(&localBytesTransfered, &timeout);

	// Join the timer and destroy it
	delete timer;

	if (bytesTransferred)
	{
		*bytesTransferred = localBytesTransfered;
	}

	// Check for errors
	if (result == CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_ERROR)
	{
		return CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION;
	}

	if (result == CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_CANCELED)
	{
		return CONNECTION_RETURN_ERROR_OPERATION_ABORTED;
	}

	if (result == CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_TIMEOUT)
	{
		return CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT;
	}

	// No error occurred
	return CONNECTION_RETURN_SUCCESS;
}

bool Connection::IsOperationCompleted(ConnectionAsynchronousOperation* asynchronousOperation)
{
	return asynchronousOperation->isCompleted();
}

Connection_Return Connection::CancelOperation(ConnectionAsynchronousOperation* asynchronousOperation)
{
	Connection_Asynchronous_Operation_Cancel_Return result = asynchronousOperation->cancel();

	// Check if the operation was canceled
	if (result == CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_CANCELED)
	{
		return CONNECTION_RETURN_SUCCESS;
	}

	// Check if an unexpected error occurred
	if (result == CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_ERROR)
	{
		return CONNECTION_RETURN_ERROR_COULD_NOT_CANCEL_OPERATION;
	}

	// The operation was completed already
	return CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED;
}

bool Connection::IsOpened()
{
	return isOpened;
}

bool Connection::IsError(Connection_Return returnValue)
{
	return returnValue != CONNECTION_RETURN_SUCCESS && returnValue != CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED;
}

void Connection::reset()
{
	isOpened = false;
}

Connection_Return Connection::performOperation(std::function<bool(ConnectionAsynchronousOperation* asynchronousOperationObject)>& operation, transfer_size_t* elementsTransfered, uint8_t elementSize, Connection_Return errorValue, ConnectionAsynchronousOperation* asynchronousOperation)
{
	// Create a new operation object if necessary
	ConnectionAsynchronousOperation* asynchronousOperationObject = asynchronousOperation ? asynchronousOperation : CreateAsynchronousOperation();

	// Check if the operation object is valid
	if (!asynchronousOperationObject->IsValid())
	{
		if (!asynchronousOperation)
		{
			delete asynchronousOperationObject;
		}

		return CONNECTION_RETURN_ERROR_INVALID_OPERATION;
	}

	// Perform the operation given
	bool success = operation(asynchronousOperationObject);

	// Check for errors
	if (!success)
	{
		if (!asynchronousOperation)
		{
			delete asynchronousOperationObject;
		}
		else
		{
			CancelOperation(asynchronousOperation);
		}

		// An error occurred
		return errorValue;
	}

	// Wait for the operation to be completed if demanded
	if (!asynchronousOperation)
	{
		Connection_Return errorValue = WaitForOperation(asynchronousOperationObject, elementsTransfered);

		delete asynchronousOperationObject;

		// Account for the size of one element
		if (elementsTransfered)
		{
			*elementsTransfered /= elementSize;
		}

		// Check for errors
		if (errorValue != CONNECTION_RETURN_SUCCESS)
		{
			return errorValue;
		}
	}

	// No error occurred
	return CONNECTION_RETURN_SUCCESS;
}

Connection_Return Connection::performOperationExtended(std::function<Connection_Return(ConnectionAsynchronousOperation* asynchronousOperationObject)>& operation, transfer_size_t* elementsTransfered, uint8_t elementSize, time_interval_t timeoutInterval)
{
	ConnectionAsynchronousOperation* asynchronousOperation = CreateAsynchronousOperation();

	Connection_Return result = operation(asynchronousOperation);

	if (result == CONNECTION_RETURN_SUCCESS)
	{
		result = WaitForOperationExtended(asynchronousOperation, elementsTransfered, timeoutInterval);
		if (elementsTransfered)
		{
			*elementsTransfered /= elementSize;
		}
	}

	delete asynchronousOperation;

	return result;
}

Connection_Return Connection::writeToConnection(const void* buffer, uint8_t elementSize, transfer_size_t elementsToWrite, transfer_size_t* elementsWritten, ConnectionAsynchronousOperation* asynchronousOperation)
{
	// Check if the connection is opened
	if (!IsOpened())
	{
		return CONNECTION_RETURN_ERROR_NOT_OPENED;
	}

	// Define the operation
	std::function<bool(ConnectionAsynchronousOperation* asynchronousOperationObject)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperationObject)
	{
		return asynchronousOperationObject->writeToConnection(buffer, elementsToWrite * elementSize);
	};

	return performOperation(operation, elementsWritten, elementSize, CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION, asynchronousOperation);
}

Connection_Return Connection::readFromConnection(void* buffer, uint8_t elementSize, transfer_size_t elementsToRead, transfer_size_t* elementsRead, ConnectionAsynchronousOperation* asynchronousOperation)
{
	// Check if the connection is opened
	if (!IsOpened())
	{
		return CONNECTION_RETURN_ERROR_NOT_OPENED;
	}

	// Define the operation
	std::function<bool(ConnectionAsynchronousOperation* asynchronousOperationObject)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperationObject)
	{
		return asynchronousOperationObject->readFromConnection(buffer, elementsToRead * elementSize);
	};

	return performOperation(operation, elementsRead, elementSize, CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION, asynchronousOperation);
}