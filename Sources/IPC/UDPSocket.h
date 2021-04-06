#pragma once

#include <mutex>
#include "Socket.h"
#include "UDPSocketAsynchronousOperation.h"

/**
 * @brief	A UDP socket class that supports Windows and Linux.
 */
class UDPSocket : public Socket
{
	friend class UDPSocketAsynchronousOperation;

private:

	char* targetAddress;
	socket_port_t targetPort;

	std::mutex sourceAddressMutex; // Locked when accessing the source address
	char* sourceAddress;
	socket_port_t sourcePort;

protected:

	virtual int getSocketType() override;

	virtual int getSocketProtocol() override;

	virtual void reset() override;

public:

	/**
	 * @brief	The constructor of UDPSocket
	 */
	UDPSocket();

	/**
	 * @brief	The destructor of UDPSocket
	 */
	virtual ~UDPSocket() override;

	/**
	 * @brief	Opens the UDP socket.
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_ALREADY_OPENED					The socket is already opened.
	 *			SOCKET_RETURN_ERROR_COULD_NOT_INITIALIZE_WINSOCK_DLL	Could not initialize the Winsock DLL. (Windows only)
	 *			SOCKET_RETURN_ERROR_COULD_NOT_CREATE_SOCKET				Could not create the socket.
	 */
	Connection_Return Open();

	/**
	 * @brief	Opens the UDP socket and binds it to a port.
	 *
	 * @param 	port	The port the UDP socket is to be bound to
	 *
	 * @return	CONNECTION_RETURN_SUCCESS								The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_ALREADY_OPENED					The socket is already opened.
	 *			SOCKET_RETURN_ERROR_COULD_NOT_INITIALIZE_WINSOCK_DLL	Could not initialize the Winsock DLL. (Windows only)
	 *			SOCKET_RETURN_ERROR_COULD_NOT_CREATE_SOCKET				Could not create the socket.
	 *			SOCKET_RETURN_ERROR_COULD_NOT_BIND_TO_PORT				Could not bind the socket to the port.
	 */
	Connection_Return OpenBound(socket_port_t port);

	virtual UDPSocketAsynchronousOperation* CreateAsynchronousOperation() override;

	/**
	 * @brief	Returns the source address of the last read operation.
	 *
	 * @param[out]	sourceAddress	Source address. May be NULL. If non-NULL the address is dynamically allocated. Empty string if no read operation has been performed yet.
	 * @param[out]	sourcePort		Source port. May be NULL. 0 if no read operation has been performed yet.
	 */
	void GetSourceAddress(char** sourceAddress, socket_port_t* sourcePort);

	/**
	 * @brief	Sets the target address of the next write operations. This address is not changed until this function is called again.
	 *
	 * @param[in]	targetAddress	New target address
	 * @param[in]	targetPort		New target port
	 *
	 * @return	CONNECTION_RETURN_SUCCESS			The operation was performed without any error.
	 *			CONNECTION_RETURN_ERROR_NOT_OPENED	The connection is not opened but needs to be for this operation.
	 */
	Connection_Return SetTargetAddress(const char* targetAddress, socket_port_t targetPort);

	/**
	 * @brief	Returns the target address of the next write operations
	 *
	 * @param[out]	targetAddress	Target address. May be NULL. If non-NULL the address is dynamically allocated. Empty string if the target address has not been set yet.
	 * @param[out]	targetPort		Target port. May be NULL. 0 if the target port has not been set yet.
	 */
	void GetTargetAddress(char** targetAddress, socket_port_t* targetPort);
};