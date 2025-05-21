#pragma once

#include "Prerequisites.h"
#include "SendQueue.h"
#include "RecvQueue.h"
#include "TCPMessage.h"
#include "TCPEvent.h"
#include "Options.h"
#include <atomic>
#include <optional>
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <flatbuffers/flatbuffers.h>

namespace dk
{
	struct EndPoint
	{
		EndPoint() = default;
		EndPoint(const std::string& ip, uint16_t port)
			: m_IP(ip)
			, m_Port(port)
		{
		}
		virtual ~EndPoint() = default;
		bool operator==(const EndPoint& other) const
		{
			return m_IP == other.m_IP && m_Port == other.m_Port;
		}
		bool operator!=(const EndPoint& other) const
		{
			return !(*this == other);
		}
		std::string toString() const
		{
			return m_IP + ":" + std::to_string(m_Port);
		}

		std::string m_IP;
		uint16_t m_Port = 0;
	};

	using SessionList = std::list<TCPSessionPtr>;
	using SessionListPtr = std::shared_ptr<SessionList>;
	using SenderCallback = std::function<void(const Stream&)>;

	class TCPSession : public std::enable_shared_from_this<TCPSession>
	{
	public:
		TCPSession(boost::asio::io_context& io_context, const EndPoint& local, const EndPoint& remote);
		~TCPSession();
		
		void close();
		void lazyClose();
		void connect(const std::string& ip, uint16_t port);
		void reconnect();
		void send(const TCPMessage& message, const options::Send& opts);
		void send(const TCPMessage& message, const options::Send& opts, const SessionListPtr targets)
		{
			if (targets)
			{
				if (opts.multi())
				{
					Stream stream = encode(message, opts);
					for (const auto& t : *targets)
						t->sendBytes(stream);
				}
				else
				{
					for (const auto& t : *targets)
						t->send(message, opts);
				}
			}
			else
			{
				send(message, opts);
			}
		}

		void sendMulticast(const TCPMessage& message, const SenderCallback& senderCallback, const options::Send& opts)
		{
			auto opt = opts;
			opt.multi(true);
			Stream stream = encode(message, opt);
			senderCallback(stream);
		}

		template<typename FBMessageT, typename std::enable_if<std::is_base_of<flatbuffers::NativeTable, FBMessageT>::value>::type* = nullptr>
		void send(const FBMessageT& message, const options::Send& opts, const SessionListPtr targets = nullptr)
		{
			send(FBMessageAdaptorT<FBMessageT>(message), opts, targets);
		}

		template<typename FBMessageT, typename std::enable_if<std::is_base_of<flatbuffers::NativeTable, FBMessageT>::value>::type* = nullptr>
		void sendMulticast(const FBMessageT& message, const SenderCallback& senderCallback, const options::Send& opts)
		{
			sendMulticast(FBMessageAdaptorT<FBMessageT>(message), senderCallback, opts);
		}

		void sendBytes(const Stream& stream);
		Stream encode(const TCPMessage& message, const options::Send& opts);
		void flush();
		void setNoDelay(bool noDelay);
		bool inbound() const { return m_Inbound; }
		bool connected() const { return m_Connected; }
		void setNo(TCPSessionNo no) { m_No = no; }
		TCPSessionNo getNo() const { return m_No; }
		const EndPoint& getEndPoint() const { return m_EndPoint; }
		void decrNumOfPendingMessages();
		void serPersistent(bool persistent);
		bool isPersistent() const { return m_Persistent; }
		void setLastReceivedTime(time_t time);
		time_t getLastReceivedTime() const { return m_LastReceivedTime; }

	private:
		void clear();
		void asyncSend();
		void asyncReceive();
		void onAccepted();
		void onResolved(const std::error_code& error, const boost::asio::ip::tcp::resolver::results_type& endpoints);
		void onConnected(const std::error_code& error);
		void onClosed();
		void onSent(const std::error_code& error, std::size_t bytesTransferred);
		void onReceived(const std::error_code& error, std::size_t bytesTransferred);
		void send(Stream stream);
		using Sendable = std::function<bool(void)>;
		void send(const TCPMessage& message, const options::Send& options, const Sendable& sendable);
		void sendBytes(const Stream& stream, const Sendable& sendable);
		void postSend();
		void fetch();
		void setConnected();
		void fireAccepted();
		void fireConnected(const std::error_code& error);
		void fireClosed();
		void incrNumOfPendingMessages();

	private:
		boost::asio::io_context& m_IoContext;
		boost::asio::ip::tcp::socket	m_Socket;
		boost::asio::io_context::strand	m_Strand;
		boost::asio::ip::tcp::resolver	m_Resolver;
		TCPEventQueue& eventQueue_;
		std::mutex	m_lock;
		SendQueue	m_SendQueue;
		RecvQueue	m_RecvQueue;
		EncoderPtr	m_Encoder;
		EndPoint	m_EndPoint{};
		bool	m_Inbound;
		bool	m_Connecting;
		bool	m_Connected;
		bool	m_LazyClose;
		TCPSessionNo	m_No;
		bool	m_Persistent;
		time_t	m_LastReceivedTime;

		friend	class	TCPAcceptor;
		friend	class	Encoder;
		friend	class	EncryptableEncoder;
	};
}