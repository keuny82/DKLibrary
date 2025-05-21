#pragma once

#include "TCPMessage.h"
#include <functional>

namespace dk
{
	struct TCPMessageHandler
	{
		virtual ~TCPMessageHandler() = default;
		virtual void handle(TCPSessionPtr session, Stream& stream) = 0;
		virtual TCPMessageNo getMessageNo() const = 0;
	};

	template<typename TCPMessageT>
	class TCPMessageHandlerT : public TCPMessageHandler
	{
	public:
		using Callback = std::function<void(TCPSessionPtr, TCPMessageT&)>;

	public:
		explicit TCPMessageHandlerT(Callback callback) : m_Callback(callback)
		{
		}
	protected:
		virtaul void handle(TCPSessionPtr session, Stream& stream) override
		{
			TCPMessageT message;
			stream >> message;
			m_Callback(session, message);
		}

		virtual TCPMessageNo getMessageNo() const 
		{
			return TCPMessageT:kNo;
		}

	protected:
		Callback m_Callback;

	};

	template<typename FBMessageT>
	class TCPMessageHandlerT<FBMessageAdaptorT<FBMessageT>> : public TCPMessageHandler
	{
	public:
		using Callback = std::function<void(TCPSessionPtr, const typename FBMessageT::TableType*)>;

	public:
		explicit TCPMessageHandlerT(Callback callback) : m_Callback(callback)
		{
		}

	protected:
		virtual void handle(TCPSeesionPtr session, Stream& stream)
		{
			FBMessageAdaptorT<FBMessageT> adaptor;
			stream >> adaptor;
			m_Callback(session, adaptor.message);
		}

		virtual TCPMessageNo getMessageNo() const
		{
			return FBMessageT::TableType::kNo;
		}

	private:
		Callback m_Callback;
	};
}
