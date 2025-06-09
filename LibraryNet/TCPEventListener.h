#pragma once
#include "TCPEvent.h"
#include "TCPMessageHandler.h"
#include <string>
#include <list>
#include <functional>
#include <atomic>

namespace dk
{
	class TCPEventListener
	{
	public:
		using ClosedHandler = std::function<void(TCPSessionPtr)>;
		using AcceptedHandler = std::function<void(TCPSessionPtr)>;
		using ConnectedHandler = std::function<void(TCPSessionPtr, const std::error_code&)>;
		using MessageNameHandler = std::function<std::string(TCPMessageNo)>;

	public:
		virtual ~TCPEventListener();
		virtual bool dispatch() = 0;
		virtual bool isAsync() = 0;
		virtual TCPEventContext& getContext() = 0;
		static void setMessageNameHandler(MessageNameHandler handler)
		{
			m_MessageNameHandler = handler;
		}

		void removeMessageHandler(TCPMessageNo messageNo)
		{
			m_MessageHandlers.erase(messageNo);
		}
		bool messageHandler(TCPMessageNo messageNo) const
		{
			return m_MessageHandlers.find(messageNo) != m_MessageHandlers.end();
		}
		void handleMessage(TCPSessionPtr session, Stream& stream);

	protected:
		TCPEventListener();
		bool dispatch(TCPEventContext& eventContext);
		void setClosedHandler(ClosedHandler handler) { m_ClosedHandler = handler; }
		void setAcceptedHandler(AcceptedHandler handler) { m_AcceptedHandler = handler; }
		void setConnectedHandler(ConnectedHandler handler) { m_ConnectedHandler = handler; }

		void addMessageHandler(TCPMessageHandlerPtr handler)
		{
			m_MessageHandlers[handler->getMessageNo()] = handler;
		}

		template<typename TCPMessageT>
		void addMessageCallback(typename TCPMessageHandlerT<TCPMessageT>::Callback callback)
		{
			addMessageHandler(std::make_shared<TCPMessageHandlerT<TCPMessage>>(callback));
		}

	protected:
		using MessageHandlerMap = std::unordered_map<TCPMessageNo, TCPMessageHandlerPtr>;
		MessageHandlerMap m_MessageHandlers;
		time_t m_lastCheckedTime;

	private:
		ClosedHandler m_ClosedHandler;
		AcceptedHandler m_AcceptedHandler;
		ConnectedHandler m_ConnectedHandler;
		static MessageNameHandler m_MessageNameHandler;
		enum
		{
			EVENTQUEUE_SHRINK_MIN = 30000,
			HEARTBEAT_CHECK_INTERVAL_MSECS = 10000,
			HEARTBEAT_PING_MSECS = 45000,
			HEARTBEAT_TIMEOUT_MSECS = 60000,
		};
	};

	class TCPAsyncEventListener : public TCPEventListener
	{
	public:
		TCPAsyncEventListener(size_t threadNum, time_t dispatchInterval = 10);
		virtual ~TCPAsyncEventListener();

	private:
		using TCPEventListener::dispatch;
		virtual bool dispatch() override { return true; }
		virtual bool isAsync() override { return true; }
		virtual TCPEventContext& getContext() override
		{
			return m_EventContexts[pos++ % m_EventContexts.size()];
		}

	private:
		using TCPEventContextList = std::vector<TCPEventContext>;
		TCPEventContextList m_EventContexts;
		std::list<std::thread> m_Dispatchers;
		volatile bool m_Stop{false};
		time_t m_DispatchInterval{ 0 };
		mutable std::atomic<size_t> pos{ 0 };
	};

	class TCPSyncEventListener : public TCPEventListener
	{
	public:
		TCPSyncEventListener();
		virtual ~TCPSyncEventListener();

	private:
		using TCPEventListener::dispatch;
		virtual bool dispatch() override { return dispatch(m_EventContext); }
		virtual bool isAsync() override { return false; }
		virtual TCPEventContext& getContext() override
		{
			return m_EventContext;
		}

	private:
		TCPEventContext m_EventContext;
	};
}

#define ON_FBMESSAGE(FBMessage) \
	void on##FBMessage(dk::TCPSessionPtr session, const FBMessage::TableType* message)

#define FBMESSAGE_UNPACK(FBMessage)\
	FBMessage recvMsg; \
	message->UnPackTo(&recvMsg);

#define FBMESSAGE_UNPACK_PTR(FBMessage) \
	auto recvMsgPtr = std::make_shared<FBMessage>(); \
	message->UnPackTo(recvMsgPtr.get());

#define REG_FBMESSAGE(FBMessage) \
	addMessageCallback<dk::FBMEssageAdaptorT<FBMessage>>)&on##FBMessage);