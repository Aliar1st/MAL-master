#include "Socket_Test.h"

#include "IPC/Socket.h"

// #####
// TESTS
// #####
void IPC_Tests::Socket_Test::IsError()
{
	Connection_Test::IsError();

	ASSERT_TRUE(getConnection()->IsError(SOCKET_RETURN_ERROR_COULD_NOT_INITIALIZE_WINSOCK_DLL));
	ASSERT_TRUE(getConnection()->IsError(SOCKET_RETURN_ERROR_COULD_NOT_CREATE_SOCKET));
	ASSERT_TRUE(getConnection()->IsError(SOCKET_RETURN_ERROR_COULD_NOT_BIND_TO_PORT));
}