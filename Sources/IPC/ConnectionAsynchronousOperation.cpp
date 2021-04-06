#include "ConnectionAsynchronousOperation.h"

#include <string.h>

#ifdef __linux__

#include <sys/eventfd.h>
#include <unistd.h>

#endif // __linux__

ConnectionAsynchronousOperation::ConnectionAsynchronousOperation()
{
#ifdef _WIN32
	// Set all variables to zero to avoid parameter errors while using this object
	overlapped.Internal = 0;
	overlapped.InternalHigh = 0;
	overlapped.Offset = 0;
	overlapped.OffsetHigh = 0;
	overlapped.Pointer = 0;

	// Assign an event
	overlapped.hEvent = CreateEvent(
		NULL,   // Default security attribute
		TRUE,   // Manual-reset event
		TRUE,   // Initial state (= signaled)
		NULL    // Unnamed event object
	);
#elif __linux__
	completed = true;
	joined = true;
	queued = false;
	state = CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_ALREADY_FINISHED;
	bytesTransferred = 0;
#endif // __linux__
}

ConnectionAsynchronousOperation::~ConnectionAsynchronousOperation()
{
	// Cancel the operation if it is running
	cancel();

#ifdef _WIN32
	// Close the event
	if (overlapped.hEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(overlapped.hEvent);
	}
#endif // _WIN32
}

bool ConnectionAsynchronousOperation::IsValid()
{
#ifdef _WIN32
	return overlapped.hEvent != INVALID_HANDLE_VALUE;
#elif __linux__
	return true;
#endif // __linux__
}

Connection_Asynchronous_Operation_Wait_For_Completion_Return ConnectionAsynchronousOperation::waitForCompletion(transfer_size_t* bytesTransferred, bool* timeout)
{
	Connection_Asynchronous_Operation_Wait_For_Completion_Return result = virtualWaitForCompletion(bytesTransferred, timeout);

	onOperationCompleted(result == CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS);

	return result;
}

Connection_Asynchronous_Operation_Cancel_Return ConnectionAsynchronousOperation::cancel()
{
	Connection_Asynchronous_Operation_Cancel_Return result = virtualCancel();

	onOperationCompleted(result == CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_ALREADY_FINISHED);

	return result;
}

bool ConnectionAsynchronousOperation::isCompleted()
{
	bool result = virtualIsCompleted();

	if (result)
	{
		onOperationCompleted(true);
	}

	return result;
}

void ConnectionAsynchronousOperation::onOperationCompleted(bool completedSuccessfully)
{
}

void ConnectionAsynchronousOperation::setSignaled()
{
#ifdef _WIN32
	overlapped.Internal = STATUS_WAIT_0;
#endif // _WIN32
}

Connection_Asynchronous_Operation_Wait_For_Completion_Return ConnectionAsynchronousOperation::virtualWaitForCompletion(transfer_size_t* bytesTransferred, bool* timeout)
{
#ifdef _WIN32
	// Windows logic needs to be implemented on a higher level
	return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_ERROR;
#elif __linux__
	// Join the thread if it is queued
	if (queued)
	{
		join();
	}

	if (state == CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_ALREADY_FINISHED)
	{
		*bytesTransferred = static_cast<transfer_size_t>(this->bytesTransferred);

		return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS;
	}

	if (state == CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_CANCELED)
	{
		if (*timeout)
		{
			return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_TIMEOUT;
		}

		return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_CANCELED;
	}

	return CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_ERROR;
#endif // __linux__
}

Connection_Asynchronous_Operation_Cancel_Return ConnectionAsynchronousOperation::virtualCancel()
{
#ifdef _WIN32
	// Windows logic needs to be implemented on a higher level
	return CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_ERROR;
#elif __linux__
	// Check if this operation is queued
	if (!queued)
	{
		return state;
	}

	// Notify the cancellation
	uint64_t notifcationValue = 1;

	write(cancelFD, &notifcationValue, sizeof(notifcationValue));

	// Join the thread
	join();

	return state;
#endif // __linux__
}

bool ConnectionAsynchronousOperation::virtualIsCompleted()
{
#ifdef _WIN32
	// Windows logic needs to be implemented on a higher level
	return false;
#elif __linux__
	if (completed)
	{
		join();
	}

	return completed;
#endif // __linux__
}

#ifdef __linux__

void ConnectionAsynchronousOperation::assignReadingAsynchronousFunction(std::function<ssize_t()> readingAsynchronousFunction)
{
	isReadingAynchronousOperation = true;
	assignAsynchronousFunction(readingAsynchronousFunction);
}

void ConnectionAsynchronousOperation::assignWritingAsynchronousFunction(std::function<ssize_t()> writingAsynchronousFunction)
{
	isReadingAynchronousOperation = false;
	assignAsynchronousFunction(writingAsynchronousFunction);
}

void* ConnectionAsynchronousOperation::asynchronousFunctionThread()
{
	int fileDescriptor = getFileDescriptor();

	// Set up file descriptor set for reading
	fd_set readFDSet;

	FD_ZERO(&readFDSet);
	FD_SET(cancelFD, &readFDSet);

	if (isReadingAynchronousOperation)
	{
		FD_SET(fileDescriptor, &readFDSet);
	}

	// Set up file descriptor set for writing
	fd_set writeFDSet;

	FD_ZERO(&writeFDSet);

	if (!isReadingAynchronousOperation)
	{
		FD_SET(fileDescriptor, &writeFDSet);
	}

	int fileDescriptorsSet = select(std::max(cancelFD, fileDescriptor) + 1, &readFDSet, &writeFDSet, NULL, NULL);

	if (fileDescriptorsSet > 0)
	{
		if (FD_ISSET(cancelFD, &readFDSet))
		{
			state = CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_CANCELED;
		}
		else
		{
			bytesTransferred = asynchronousFunction();

			// Check for errors
			if (bytesTransferred == -1)
			{
				state = CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_ERROR;
			}
			else
			{
				state = CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_ALREADY_FINISHED;
			}
		}
	}
	else
	{
		state = CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_ERROR;
	}

	completed = true;

	return NULL;
}

void ConnectionAsynchronousOperation::assignAsynchronousFunction(std::function<ssize_t()> asynchronousFunction)
{
	this->asynchronousFunction = asynchronousFunction;

	completed = false;
	joined = false;
	queued = true;

	cancelFD = eventfd(
		0, // Initial value
		0 // Flags
	);

	// Create the thread; As AsynchronousFunctionThread() is a member function we need to do some trickery (i.e. cast the function and suppress the resulting warning)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpmf-conversions"

	typedef void* (* asynchronous_function_t)(void*);

	pthread_create(&asynchronousThread, NULL, (asynchronous_function_t) &ConnectionAsynchronousOperation::asynchronousFunctionThread, this);

#pragma GCC diagnostic pop
}

void ConnectionAsynchronousOperation::join()
{
	joiningMutex.lock();
	if (!joined)
	{
		pthread_join(asynchronousThread, NULL);

		close(cancelFD);

		// Set this thread as finished
		joined = true;

		queued = false;
	}
	joiningMutex.unlock();
}

#endif // __linux__