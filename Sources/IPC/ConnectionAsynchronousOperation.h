#pragma once

#ifdef _WIN32

#include <Windows.h>

typedef DWORD transfer_size_t;

#elif __linux__

#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

typedef ssize_t transfer_size_t;

#endif // __linux__

/**
 * @brief	Enumerates all possible canceling return values
 */
enum Connection_Asynchronous_Operation_Cancel_Return
{
	CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_CANCELED,
	CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_ALREADY_FINISHED,
	CONNECTION_ASYNCHRONOUS_OPERATION_CANCEL_RETURN_ERROR
};

/**
 * @brief	Enumerates all possible wait for operation return values
 */
enum Connection_Asynchronous_Operation_Wait_For_Completion_Return
{
	CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS,
	CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_TIMEOUT,
	CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_CANCELED,
	CONNECTION_ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_ERROR
};

/**
 * @brief	Abstract class used to detect the status of asynchronous operations
 */
class ConnectionAsynchronousOperation
{
	friend class Connection;

private:

#ifdef __linux__
	/**
	 * @brief	The thread routine executing the asynchronous function.
	 *
	 * @return	Unused generic result.
	 */
	void* asynchronousFunctionThread();

	/**
	 * @brief	Assigned and starts the asynchronous function that shall be executed.
	 *
	 * @param	asynchronousFunction	Asynchronous function
	 */
	void assignAsynchronousFunction(std::function<ssize_t()> asynchronousFunction);

	/**
	 * @brief	Joins the thread.
	 */
	void join();
#endif // __linux__

protected:

#ifdef _WIN32
	OVERLAPPED overlapped;
#elif __linux__
	pthread_t asynchronousThread;
	std::function<ssize_t()> asynchronousFunction;

	bool completed;
	bool joined;
	bool queued;

	bool isReadingAynchronousOperation;
	int cancelFD;

	std::mutex joiningMutex;
	Connection_Asynchronous_Operation_Cancel_Return state;

	ssize_t bytesTransferred;
#endif // __linux__

	/**
	 * @brief	The constructor of ConnectionAsynchronousOperation.
	 */
	ConnectionAsynchronousOperation();

	/**
	 * @brief	Queues this asynchronous operation to write to a connection.
	 *
	 * @param[out]	buffer			The buffer to read the data to write from
	 * @param		bytesToWrite	The amount of bytes to write
	 *
	 * @return	True if there was no error queuing the operation; False otherwise
	 */
	virtual bool writeToConnection(const void* buffer, transfer_size_t bytesToWrite) = 0;

	/**
	 * @brief	Queues this asynchronous operation to read from a connection
	 *
	 * @param[out]	buffer			The buffer to write the read data in
	 * @param		bytesToRead		The amount of bytes to read
	 *
	 * @return	True if there was no error queuing the operation; False otherwise
	 */
	virtual bool readFromConnection(void* buffer, transfer_size_t bytesToRead) = 0;

	/**
	 * @brief	Cancels this operations
	 *
	 * @param[out]	bytesTransferred	A pointer to the variable to store the bytes that were transferred in.
	 * @param[in]	timeout				A pointer to the variable storing whether there was a timeout.
	 *
	 * @return	ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS			The operation was completed.
	 *			ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_TIMEOUT	The operation was not completed but a timeout happened. This is signaled by canceling the operation and the timeout parameter being true.
	 *			ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_CANCELED	The operation was canceled by an external source.
	 *			ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_ERROR				There was an error waiting for the operation.
	 */
	Connection_Asynchronous_Operation_Wait_For_Completion_Return waitForCompletion(transfer_size_t* bytesTransferred, bool* timeout);

	/**
	 * @brief	Cancels this operations
	 *
	 * @return	ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_CANCELED			The operation was canceled.
	 *			ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_ALREADY_FINISHED	The operation is already finished.
	 *			ASYNCHRONOUS_OPERATION_CANCEL_RETURN_ERROR						There was an error canceling the operation.
	 */
	Connection_Asynchronous_Operation_Cancel_Return cancel();

	/**
	 * @brief	Returns whether this operation is completed.
	 *
	 * @return	True if it is completed; False otherwise
	 */
	bool isCompleted();

	/**
	 * @brief	An event that is called, whenever the operation is completed.
	 *			Note that this event is only fired, when the completion status is checked.
	 *			Make sure that multiple checks of the completion status do not break anything.
	 *
	 * @param	completedSuccessfully	True if the operation was completed successfully, i.e. no error occurred and the operation was not canceled
	 *									False otherwise
	 */
	virtual void onOperationCompleted(bool completedSuccessfully);

	/**
	 * @brief	Sets this operation to be signaled (i.e. completed).
	 */
	void setSignaled();

	/**
	 * @brief	Cancels this operations. Override this function when implementing logic.
	 *
	 * @param[out]	bytesTransferred	A pointer to the variable to store the bytes that were transferred in.
	 * @param[in]	timeout				A pointer to the variable storing whether there was a timeout.
	 *
	 * @return	ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS			The operation was completed.
	 *			ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_TIMEOUT	The operation was not completed but a timeout happened. This is signaled by canceling the operation and the timeout parameter being true.
	 *			ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_SUCCESS_CANCELED	The operation was canceled by an external source.
	 *			ASYNCHRONOUS_OPERATION_WAIT_FOR_COMPLETION_RETURN_ERROR				There was an error waiting for the operation.
	 */
	virtual Connection_Asynchronous_Operation_Wait_For_Completion_Return virtualWaitForCompletion(transfer_size_t* bytesTransferred, bool* timeout);

	/**
	 * @brief	Cancels this operations. Override this function when implementing logic.
	 *
	 * @return	ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_CANCELED			The operation was canceled.
	 *			ASYNCHRONOUS_OPERATION_CANCEL_RETURN_SUCCESS_ALREADY_FINISHED	The operation is already finished.
	 *			ASYNCHRONOUS_OPERATION_CANCEL_RETURN_ERROR						There was an error canceling the operation.
	 */
	virtual Connection_Asynchronous_Operation_Cancel_Return virtualCancel();

	/**
	 * @brief	Returns whether this operation is completed. Override this function when implementing logic.
	 *
	 * @return	True if it is completed; False otherwise
	 */
	virtual bool virtualIsCompleted();

#ifdef __linux__
	/**
	 * @brief	Assigned and starts the reading asynchronous function that shall be executed.
	 *
	 * @param	readingAsynchronousFunction		Reading asynchronous function
	 */
	void assignReadingAsynchronousFunction(std::function<ssize_t()> readingAsynchronousFunction);

	/**
	 * @brief	Assigned and starts the writing asynchronous function that shall be executed.
	 *
	 * @param	writingAsynchronousFunction		Writing asynchronous function
	 */
	void assignWritingAsynchronousFunction(std::function<ssize_t()> writingAsynchronousFunction);

	/**
	 * @brief	Returns the file descriptor of the used connection object
	 *
	 * @return	File descriptor
	 */
	virtual int getFileDescriptor() = 0;
#endif // __linux__

public:

	/**
	 * @brief	The destructor of ConnectionAsynchronousOperation.
	 */
	virtual ~ConnectionAsynchronousOperation();

	/**
	 * @brief	Returns whether this object has valid values.
	 *
	 * @return	True if this object has valid values; False if not.
	 */
	bool IsValid();
};