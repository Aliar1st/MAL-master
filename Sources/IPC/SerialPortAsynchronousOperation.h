#pragma once

#include "ConnectionAsynchronousOperation.h"

class SerialPort;

/**
 * @brief	A class used to detect the status of asynchronous operations in SerialPort.
 */
class SerialPortAsynchronousOperation : public ConnectionAsynchronousOperation
{
	friend class SerialPort;

private:

	SerialPort* serialPort;

protected:

	/**
	 * @brief	The constructor of SerialPortAsynchronousOperation.
	 *
	 * @param[in]	serialPort	The serial port
	 */
	SerialPortAsynchronousOperation(SerialPort* serialPort);

	virtual bool writeToConnection(const void* buffer, transfer_size_t bytesToWrite) override;

	virtual bool readFromConnection(void* buffer, transfer_size_t bytesToRead) override;

	virtual Connection_Asynchronous_Operation_Wait_For_Completion_Return virtualWaitForCompletion(transfer_size_t* bytesTransferred, bool* timeout) override;

	virtual Connection_Asynchronous_Operation_Cancel_Return virtualCancel() override;

	virtual bool virtualIsCompleted() override;

#ifdef __linux__
	virtual int getFileDescriptor() override;
#endif // __linux__
};