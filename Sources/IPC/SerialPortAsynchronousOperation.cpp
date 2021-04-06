#include "SerialPortAsynchronousOperation.h"

#include "SerialPort.h"

#ifdef __linux__

#include <unistd.h>

#endif // __linux__

SerialPortAsynchronousOperation::SerialPortAsynchronousOperation(SerialPort* serialPort)
	: serialPort(serialPort)
{
}

bool SerialPortAsynchronousOperation::writeToConnection(const void* buffer, transfer_size_t bytesToWrite)
{
#ifdef _WIN32
	// Write to the port
	BOOL writeFileResult = WriteFile(
		serialPort->internPort, // Serial port handle
		buffer,                 // The buffer that contains the message to write
		bytesToWrite,           // Size of the buffer
		NULL,                   // Number of bytes written (This has to be set to NULL because this serial port works asynchronously)
		&overlapped             // Use overlapping
	);

	return writeFileResult || GetLastError() == ERROR_IO_PENDING;
#elif __linux__
	assignWritingAsynchronousFunction(
		[this, buffer, bytesToWrite]
	{
		return write(serialPort->internPort, buffer, bytesToWrite);
	});

	// On Linux we can't check for errors here
	return true;
#endif // __linux__
}

bool SerialPortAsynchronousOperation::readFromConnection(void* buffer, transfer_size_t bytesToRead)
{
#ifdef _WIN32
	// Read from the port
	BOOL readFileResult = ReadFile(
		serialPort->internPort, // Serial port handle
		buffer,                 // Buffer to write message in
		bytesToRead,            // Size of the buffer
		NULL,                   // Number of bytes read (This has to be set to NULL because this serial port works asynchronously)
		&overlapped             // Use overlapping
	);

	return readFileResult || (GetLastError() == ERROR_MORE_DATA || GetLastError() == ERROR_IO_PENDING);
#elif __linux__
	assignReadingAsynchronousFunction(
		[this, buffer, bytesToRead]
	{
		return read(serialPort->internPort, buffer, bytesToRead);
	});

	// On Linux we can't check for errors here
	return true;
#endif // __linux__
}

Connection_Asynchronous_Operation_Wait_For_Completion_Return SerialPortAsynchronousOperation::virtualWaitForCompletion(transfer_size_t* bytesTransferred, bool* timeout)
{
#ifdef _WIN32
	// Return that the operation is finished if the serial port is closed
	if (!serialPort->IsOpened())
	{
		return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS;
	}

	// Wait for the operation to complete
	BOOL success = GetOverlappedResult(
		serialPort->internPort, // Serial port handle
		&overlapped,            // OVERLAPPED structure
		bytesTransferred,       // Bytes transferred
		TRUE                    // Wait for completion
	);

	if (!success)
	{
		if (GetLastError() == ERROR_OPERATION_ABORTED)
		{
			if (*timeout)
			{
				return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_TIMEOUT;
			}

			return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_CANCELED;
		}

		return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_ERROR;
	}

	return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS;
#elif __linux__
	return ConnectionAsynchronousOperation::virtualWaitForCompletion(bytesTransferred, timeout);
#endif // __linux__
}

Connection_Asynchronous_Operation_Cancel_Return SerialPortAsynchronousOperation::virtualCancel()
{
#ifdef _WIN32
	// Return that the operation is finished if the serial port is closed
	if (!serialPort->IsOpened())
	{
		return CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_ALREADY_FINISHED;
	}

	// Attempt to cancel the operation
	BOOL success = CancelIoEx(serialPort->internPort, &overlapped);

	if (success)
	{
		return CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_CANCELED;
	}

	if (GetLastError() != ERROR_NOT_FOUND)
	{
		return CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_ERROR;
	}

	return CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_ALREADY_FINISHED;
#elif __linux__
	return ConnectionAsynchronousOperation::virtualCancel();
#endif // __linux__
}

bool SerialPortAsynchronousOperation::virtualIsCompleted()
{
#ifdef _WIN32
	// Return that the operation is finished if the serial port is closed
	if (!serialPort->IsOpened())
	{
		return true;
	}

	DWORD bytesTransferred;
	BOOL success = GetOverlappedResult(
		serialPort->internPort, // Serial port handle
		&overlapped,            // OVERLAPPED structure
		&bytesTransferred,      // Bytes transferred
		FALSE                   // Don't wait for completion
	);

	// Return whether the operation is completed
	return success != 0;
#elif __linux__
	return ConnectionAsynchronousOperation::virtualIsCompleted();
#endif // __linux__
}

#ifdef __linux__

int SerialPortAsynchronousOperation::getFileDescriptor()
{
	return serialPort->internPort;
}

#endif // __linux__