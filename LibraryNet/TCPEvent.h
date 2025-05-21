#pragma once

#include "Prerequisites.h"
#include "Stream.h"
#include "OptimizedFlippableQueue.h"
#include <unordered_set>

namespace dk
{
	struct TCPEvent
	{
		enum Type
		{
			CLOSED,
			ACCEPTED,
			CONNECTED,
			RECEIVED,
			SENT,
			FINALIZED,
		};

		TCPEvent(Type type, TCPSessionPtr sesson = nullptr, const Stream& stream = Stream(), const std::error_code& error = std::error_code(0, std::generic_category()))
			: m_Type(type)
			, m_Session(sesson)
			, m_Stream(stream)
			, m_Error(error)
		{
		}
		Type m_Type;
		TCPSessionPtr m_Session;
		Stream m_Stream;
		std::error_code m_Error;
	};

	using TCPEventQueue = OptimizedFlippableQueueT<TCPEvent>;
	struct TCPEventContext
	{
		TCPEventQueue m_EventQueue;
		std::unordered_set<TCPSessionPtr> m_Sessions;
	};
}