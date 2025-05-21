#pragma once

#include <vector>
#include "prerequisite.h"
#include "Exception.h"

namespace dk
{
	using TCPSessionNo = uint64_t;
	const TCPSessionNo INVALID_SESSION_NO = 0;
	using TCPMessageNo = uint16_t;
	using NodeNo = uint16_t;
	const NodeNo INVALID_NODE_NO = 0;
	CLASS_SHARED_PTR(TCPService);
	CLASS_SHARED_PTR(TCPAcceptor);
	CLASS_SHARED_PTR(TCPSession);
	using TCPSessionList = std::vector<TCPSessionPtr>;
	CLASS_SHARED_PTR(TCPEventListener);
	STRUCT_SHARED_PTR(TCPMessageHandler);
	STRUCT_SHARED_PTR(TCPMessage);
	namespace options
	{
		class Send;
	}
	CLASS_SHARED_PTR(Encoder);
	
}