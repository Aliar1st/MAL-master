#pragma once

#include <functional>
#include <stdint.h>
#include <type_traits>
#include "ConnectionAsynchronousOperation.h"
#include "Threading/Timer.h"
#include "Utilities/StringUtil.h"

typedef unsigned int Connection_Return;

/**
 * All possible return values of the functions of Connection.
 */
enum : Connection_Return
{
	CONNECTION_RETURN_SUCCESS,
	CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED,
	CONNECTION_RETURN_ERROR_NOT_OPENED,
	CONNECTION_RETURN_ERROR_ALREADY_OPENED,
	CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION,
	CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION,
	CONNECTION_RETURN_ERROR_INVALID_OPERATION,
	CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION,
	CONNECTION_RETURN_ERROR_COULD_NOT_CANCEL_OPERATION,
	CONNECTION_RETURN_ERROR_OPERATION_ABORTED,
	CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT,
	CONNECTION_RETURN_AMOUNT
};

/**
 * @brief	An abstract connection
 */
class Connection
{
private:

	/**
	 * @brief	Used to pass the argument of the internal timer of Connection::WaitForOperationExtended(...)
	 */
	struct Connection_WaitForOperationExtended
	{
		bool* pTimeout;
		ConnectionAsynchronousOperation* asyncOperation;

		Connection_WaitForOperationExtended(bool* pTimeout, ConnectionAsynchronousOperation* asyncOperation)
		{
			this->pTimeout = pTimeout;
			this->asyncOperation = asyncOperation;
		}
	};

	/**
	 * @brief	Writes to the connection.
	 *
	 * @param[in]	buffer					The buffer that contains the bytes to write.
	 * @param		elementSize				The size of one element in bytes.
	 * @param		elementsToWrite		 	The number of elements to write.
	 * @param[out]	elementsWritten		 	The number of elements written. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION	Could not write to the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return writeToConnection(const void* buffer, uint8_t elementSize, transfer_size_t elementsToWrite, transfer_size_t* elementsWritten, ConnectionAsynchronousOperation* asynchronousOperation);

	/**
	 * @brief	Reads from the connection.
	 *
	 * @param[out]	buffer					The buffer to write in.
	 * @param		elementSize				The size of one element in bytes.
	 * @param 		elementsToRead			The number of elements to read.
	 * @param[out]	elementsRead			The number of elements read. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION	Could not read from the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return readFromConnection(void* buffer, uint8_t elementSize, transfer_size_t elementsToRead, transfer_size_t* elementsRead, ConnectionAsynchronousOperation* asynchronousOperation);

	/**
	 * @brief	The function that is called on another thread within WaitForOperationExtended after the given time interval
	 *
	 * @param		timePassed	True if the time has passed without an cancel; False the timer was canceled
	 * @param[in]	data		A pointer to the passed data.
	 */
	void waitForOperationExtendedTimerTimeout(bool timePassed, void* data);

protected:

	bool isOpened;

	/**
	 * @brief	Performs an operation
	 *
	 * @param		operation				The operation to perform. Returns true if it is successful; False otherwise.
	 *										The parameter is the used asynchronous operation and may differ from the parameter provided to this function.
	 * @param[out]	elementsTransfered	 	The number of elements transfered. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param		elementSize				The size of one element in bytes. Unused if elementsTransfered is NULL.
	 * @param		errorValue				The value to return if the operation was not performed successfully.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			errorValue												The operation could not be performed
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return performOperation(std::function<bool(ConnectionAsynchronousOperation* asynchronousOperationObject)>& operation, transfer_size_t* elementsTransfered, uint8_t elementSize, Connection_Return errorValue, ConnectionAsynchronousOperation* asynchronousOperation);

	/**
	 * @brief	Performs an operation and blocks until the operation is completed or a given interval is elapsed.
	 *
	 * @param		operation				The operation to perform.
	 * @param[out]	elementsTransfered	 	The number of elements transfered. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param		elementSize				The size of one element in bytes. Used to convert from bytes transfered to elements transfered.
	 *										Unused if elementsTransfered is NULL.
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 *			Additional return values depend on the operation given
	 */
	Connection_Return performOperationExtended(std::function<Connection_Return(ConnectionAsynchronousOperation* asynchronousOperationObject)>& operation, transfer_size_t* elementsTransfered, uint8_t elementSize, time_interval_t timeoutInterval);

	/**
	 * @brief	Resets the connection to its initialization state.
	 */
	virtual void reset();

public:

	constexpr static const time_interval_t NO_TIMEOUT = Semaphore::NO_TIMEOUT;

	/**
	 * @brief	The constructor of Connection.
	 */
	Connection();

	/**
	 * @brief	The destructor of Connection.
	 */
	virtual ~Connection();

	virtual Connection_Return Close() = 0;

	/**
	 * @brief	Writes a void array to the connection
	 *
	 * @param[in]	buffer				 	The void array.
	 * @param 		bytesToWrite		 	The number of bytes to write.
	 * @param[out]	bytesWritten		 	The number of bytes written. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION	Could not write to the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return WriteArray(void* buffer, transfer_size_t bytesToWrite, transfer_size_t* bytesWritten, ConnectionAsynchronousOperation* asynchronousOperation)
	{
		return writeToConnection(buffer, 1, bytesToWrite, bytesWritten, asynchronousOperation);
	}

	/**
	 * @brief	Writes an array to the connection
	 *
	 * @param[in]	buffer				 	The array to write to the connection.
	 * @param 		elementsToWrite	 		The number of elements to write.
	 * @param[out]	elementsWritten		 	The number of elements written. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION	Could not write to the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 *
	 * @tparam	The type of the array's elements. This cannot be void.
	 */
	template<typename T, typename std::enable_if<!std::is_void<T>::value>::type* = nullptr>
	Connection_Return WriteArray(T* buffer, transfer_size_t elementsToWrite, transfer_size_t* elementsWritten, ConnectionAsynchronousOperation* asynchronousOperation)
	{
		return writeToConnection(buffer, sizeof(T), elementsToWrite, elementsWritten, asynchronousOperation);
	}

	/**
	 * @brief	Writes a single variable of unknown type to the connection
	 *
	 * @param[in]	buffer				 	A pointer to the variable to write to the connection.
	 * @param 		elementSize	 			The size of the variable in bytes.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION	Could not write to the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return WriteSingle(void* buffer, uint8_t elementSize, ConnectionAsynchronousOperation* asynchronousOperation)
	{
		return WriteArray(buffer, elementSize, NULL, asynchronousOperation);
	}

	/**
	 * @brief	Writes a single variable to the connection
	 *
	 * @param[in]	value				 	The variable to write to the connection.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION	Could not write to the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 *
	 * @tparam	The type of the variable. This cannot be void nor can the variable be a constant.
	 */
	template<typename T, typename std::enable_if<!std::is_const<T>::value && !std::is_same<T, void*>::value>::type* = nullptr>
	Connection_Return WriteSingle(T& value, ConnectionAsynchronousOperation* asynchronousOperation)
	{
		return WriteArray(&value, 1, NULL, asynchronousOperation);
	}

	/**
	 * @brief	Writes a string to the connection
	 *
	 * @param[in]	buffer				 	The buffer that contains the char array to write to the connection.
	 * @param[out]	elementsWritten		 	The number of elements written. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION	Could not write to the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return WriteString(const char* buffer, transfer_size_t* elementsWritten, ConnectionAsynchronousOperation* asynchronousOperation)
	{
		return WriteArray(buffer, static_cast<transfer_size_t>(StringUtil::Strlen(buffer)) + 1, elementsWritten, asynchronousOperation);
	}

	/**
	 * @brief	Reads a void array from the connection.
	 *
	 * @param[out]	buffer				 	The void array.
	 * @param 		bytesToRead		 		The maximum number of bytes to read.
	 * @param[out]	bytesRead			 	The number of bytes read. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION	Could not read from the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return ReadArray(void* buffer, transfer_size_t bytesToRead, transfer_size_t* bytesRead, ConnectionAsynchronousOperation* asynchronousOperation)
	{
		return readFromConnection(buffer, 1, bytesToRead, bytesRead, asynchronousOperation);
	}

	/**
	 * @brief	Reads an array from the connection.
	 *
	 * @param[out]	buffer				 	The array to write in after reading from the connection.
	 * @param 		elementsToRead	 		The maximum number of elements to read.
	 * @param[out]	elementsRead		 	The number of elements read. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION	Could not read from the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 *
	 * @tparam	The type of the array's elements. This cannot be void.
	 */
	template<typename T, typename std::enable_if<!std::is_void<T>::value>::type* = nullptr>
	Connection_Return ReadArray(T* buffer, transfer_size_t elementsToRead, transfer_size_t* elementsRead, ConnectionAsynchronousOperation* asynchronousOperation)
	{
		return readFromConnection(buffer, sizeof(T), elementsToRead, elementsRead, asynchronousOperation);
	}

	/**
	 * @brief	Reads a single variable of unknown type from the socket
	 *
	 * @param[out]	buffer				 	A pointer to the variable to write in after reading from the socket.
	 * @param 		elementSize			 	The size of the variable in bytes.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION	Could not read from the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return ReadSingle(void* buffer, uint8_t elementSize, ConnectionAsynchronousOperation* asynchronousOperation)
	{
		return ReadArray(buffer, elementSize, NULL, asynchronousOperation);
	}

	/**
	 * @brief	Reads a single variable from the socket
	 *
	 * @param[out]	buffer				 	A pointer to the variable to write in after reading from the socket.
	 * @param[in]	asynchronousOperation	The operation object to use for this operation. If NULL, this function is blocking until it is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION	Could not read from the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 *
	 * @tparam	The type of the array's elements. This cannot be void.
	 */
	template<typename T, typename std::enable_if<!std::is_void<T>::value>::type* = nullptr>
	Connection_Return ReadSingle(T* buffer, ConnectionAsynchronousOperation* asynchronousOperation)
	{
		return ReadArray(buffer, 1, NULL, asynchronousOperation);
	}

	/**
	 * @brief	Writes a void array to the socket and blocks until the operation is completed or a given interval is elapsed.
	 *
	 * @param[in]	buffer				 	The void array.
	 * @param 		bytesToWrite		 	The number of bytes to write.
	 * @param[out]	bytesWritten		 	The number of bytes written. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION	Could not write to the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return WriteArrayExtended(void* buffer, transfer_size_t bytesToWrite, transfer_size_t* bytesWritten, time_interval_t timeoutInterval)
	{
		// Define the operation
		std::function<Connection_Return(ConnectionAsynchronousOperation* asynchronousOperation)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperation)
		{
			return WriteArray(buffer, bytesToWrite, NULL, asynchronousOperation);
		};

		return performOperationExtended(operation, bytesWritten, 1, timeoutInterval);
	}

	/**
	 * @brief	Writes an array to the socket and blocks until the operation is completed or a given interval is elapsed.
	 *
	 * @param[in]	buffer				 	The array to write to the socket.
	 * @param 		elementsToWrite	 		The number of elements to write.
	 * @param[out]	elementsWritten		 	The number of elements written. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION	Could not write to the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 *
	 * @tparam	The type of the array's elements. This cannot be void.
	 */
	template<typename T, typename std::enable_if<!std::is_void<T>::value>::type* = nullptr>
	Connection_Return WriteArrayExtended(T* buffer, transfer_size_t elementsToWrite, transfer_size_t* elementsWritten, time_interval_t timeoutInterval)
	{
		// Define the operation
		std::function<Connection_Return(ConnectionAsynchronousOperation* asynchronousOperation)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperation)
		{
			return WriteArray(buffer, elementsToWrite, NULL, asynchronousOperation);
		};

		return performOperationExtended(operation, elementsWritten, sizeof(T), timeoutInterval);
	}

	/**
	 * @brief	Writes a single variable of unknown type to the socket and blocks until the operation is completed or a given interval is elapsed.
	 *
	 * @param[in]	buffer				 	A pointer to the variable to write to the socket.
	 * @param 		elementSize	 			The size of the variable in bytes.
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION	Could not write to the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return WriteSingleExtended(void* buffer, uint8_t elementSize, time_interval_t timeoutInterval)
	{
		return WriteArrayExtended(buffer, elementSize, NULL, timeoutInterval);
	}

	/**
	 * @brief	Writes a single variable to the socket and blocks until the operation is completed or a given interval is elapsed.
	 *
	 * @param[in]	value				 	The variable to write to the socket.
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION	Could not write to the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 *
	 * @tparam	The type of the variable. This cannot be void nor can the variable be a constant.
	 */
	template<typename T, typename std::enable_if<!std::is_const<T>::value && !std::is_same<T, void*>::value>::type* = nullptr>
	Connection_Return WriteSingleExtended(T& value, time_interval_t timeoutInterval)
	{
		// Define the operation
		std::function<Connection_Return(ConnectionAsynchronousOperation* asynchronousOperation)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperation)
		{
			return WriteSingle(value, asynchronousOperation);
		};

		return performOperationExtended(operation, NULL, sizeof(T), timeoutInterval);
	}

	/**
	 * @brief	Writes a string to the socket and blocks until the operation is completed or a given interval is elapsed.
	 *
	 * @param[in]	buffer				 	The buffer that contains the char array to write to the socket.
	 * @param[out]	elementsWritten		 	The number of elements written. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WRITE_TO_CONNECTION	Could not write to the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return WriteStringExtended(const char* buffer, transfer_size_t* elementsWritten, time_interval_t timeoutInterval)
	{
		return WriteArrayExtended(buffer, static_cast<transfer_size_t>(StringUtil::Strlen(buffer)) + 1, elementsWritten, timeoutInterval);
	}

	/**
	 * @brief	Reads a void array from the socket and blocks until the operation is completed or a given interval is elapsed.
	 *
	 * @param[out]	buffer				 	The void array.
	 * @param 		bytesToRead		 		The maximum number of bytes to read.
	 * @param[out]	bytesRead			 	The number of bytes read. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION	Could not read from the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return ReadArrayExtended(void* buffer, transfer_size_t bytesToRead, transfer_size_t* bytesRead, time_interval_t timeoutInterval)
	{
		// Define the operation
		std::function<Connection_Return(ConnectionAsynchronousOperation* asynchronousOperation)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperation)
		{
			return ReadArray(buffer, bytesToRead, NULL, asynchronousOperation);
		};

		return performOperationExtended(operation, bytesRead, 1, timeoutInterval);
	}

	/**
	 * @brief	Reads an array from the socket and blocks until the operation is completed or a given interval is elapsed.
	 *
	 * @param[out]	buffer				 	The array to write in after reading from the socket.
	 * @param 		elementsToRead	 		The maximum number of elements to read.
	 * @param[out]	elementsRead		 	The number of elements read. A value is only assigned to this if it is non-NULL and if the function is blocking.
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION	Could not read from the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 *
	 * @tparam	The type of the array's elements. This cannot be void.
	 */
	template<typename T, typename std::enable_if<!std::is_void<T>::value>::type* = nullptr>
	Connection_Return ReadArrayExtended(T* buffer, transfer_size_t elementsToRead, transfer_size_t* elementsRead, time_interval_t timeoutInterval)
	{
		// Define the operation
		std::function<Connection_Return(ConnectionAsynchronousOperation* asynchronousOperation)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperation)
		{
			return ReadArray(buffer, elementsToRead, NULL, asynchronousOperation);
		};

		return performOperationExtended(operation, elementsRead, sizeof(T), timeoutInterval);
	}

	/**
	 * @brief	Reads a single variable of unknown type from the socket and blocks until the operation is completed or a given interval is elapsed.
	 *
	 * @param[out]	buffer				 	A pointer to the variable to write in after reading from the socket.
	 * @param 		elementSize			 	The size of the variable in bytes.
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION	Could not read from the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return ReadSingleExtended(void* buffer, uint8_t elementSize, time_interval_t timeoutInterval)
	{
		// Define the operation
		std::function<Connection_Return(ConnectionAsynchronousOperation* asynchronousOperation)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperation)
		{
			return ReadSingle(buffer, elementSize, asynchronousOperation);
		};

		return performOperationExtended(operation, NULL, elementSize, timeoutInterval);
	}

	/**
	 * @brief	Reads a single variable from the socket and blocks until the operation is completed or a given interval is elapsed.
	 *
	 * @param[out]	buffer				 	A pointer to the variable to write in after reading from the socket.
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED						The connection is not opened but needs to be for this operation.
	 *			CONNECTION_RETURN_ERROR_INVALID_OPERATION				The given asynchronous operation object is invalid.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_READ_FROM_CONNECTION	Could not read from the connection.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 *
	 * @tparam	The type of the array's elements. This cannot be void.
	 */
	template<typename T, typename std::enable_if<!std::is_void<T>::value>::type* = nullptr>
	Connection_Return ReadSingleExtended(T* buffer, time_interval_t timeoutInterval)
	{
		// Define the operation
		std::function<Connection_Return(ConnectionAsynchronousOperation* asynchronousOperation)> operation = [&](ConnectionAsynchronousOperation* asynchronousOperation)
		{
			return ReadSingle(buffer, asynchronousOperation);
		};

		return performOperationExtended(operation, NULL, sizeof(T), timeoutInterval);
	}

	/**
	 * @brief	Blocks until the given operation is completed.
	 *
	 * @param[in]	asynchronousOperation	The operation to wait for.
	 * @param[out]	bytesTransferred		A pointer to the number of bytes transferred. If NULL, this parameter is not written into
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return WaitForOperation(ConnectionAsynchronousOperation* asynchronousOperation, transfer_size_t* bytesTransferred);

	/**
	 * @brief	Blocks until the given operation is completed or an interval elapses.
	 *
	 * @param[in]	asynchronousOperation	The operation to wait for.
	 * @param[out]	bytesTransferred		A pointer to the number of bytes transferred. If NULL, this parameter is not written into
	 * @param		timeoutInterval			The timeout interval in milliseconds. If the operation is not completed after this interval, the operation is canceled. If Connection::NO_TIMEOUT, this call is blocking until the operation is completed.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_OPERATION_ABORTED				The operation was queued but aborted.
	 *			CONNECTION_RETURN_ERROR_OPERATION_TIMEOUT				The given interval elapsed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_WAIT_FOR_OPERATION	Could not wait for the operation.
	 */
	Connection_Return WaitForOperationExtended(ConnectionAsynchronousOperation* asynchronousOperation, transfer_size_t* bytesTransferred, time_interval_t timeoutInterval);

	/**
	 * @brief	Returns whether the given operation is completed
	 *
	 * @param[in]	asynchronousOperation	The operation to check.
	 *
	 * @return	True if the operation is completed; False if not.
	 */
	bool IsOperationCompleted(ConnectionAsynchronousOperation* asynchronousOperation);

	/**
	 * @brief	Cancels the given asynchronous operations queued for this connection.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was canceled.
	 *			CONNECTION_RETURN_SUCCESS_OPERATION_ALREADY_COMPLETED	The operation was already completed.
	 *			CONNECTION_RETURN_ERROR_COULD_NOT_CANCEL_OPERATION		Could not cancel the given operation.
	 */
	Connection_Return CancelOperation(ConnectionAsynchronousOperation* asynchronousOperation);

	/**
	 * @brief	Creates an asynchronous operation object used for asynchronous operations.
	 *			The object is dynamically allocated and needs to be freed using delete.
	 *
	 * @return	Asynchronous operation object
	 */
	virtual ConnectionAsynchronousOperation* CreateAsynchronousOperation() = 0;

	/**
	 * @brief	Returns whether the connection is opened.
	 *
	 * @return	True if the port is opened; false if not.
	 */
	bool IsOpened();

	/**
	 * @brief	Returns whether the given value represents an error.
	 *
	 * @return	True if the value represents an error; False otherwise.
	 */
	virtual bool IsError(Connection_Return returnValue);
};