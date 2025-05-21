#pragma once

#include "Prerequisites.h"
#include "TCPAcceptor.h"
#include "Encoder.h"
#include <string>
#include <vector>
#include <set>

namespace dk
{
#pragma pack(push, 1)
	struct NullPacketHeader
	{ };
#pragma pack(pop)
	class NullEncoder : public EncoderT<NullPacketHeader>
	{
	public:
		NullEncoder() = default;
		virtual ~NullEncoder() = default;
	private:
		virtual void doEncode(Stream&, const options::Send&) override
		{
			// No encoding needed for null encoder
		}
		virtual void doDecode(Stream&) override
		{
			// No decoding needed for null encoder
		}
	};


	class TCPService
	{
	public:
		TCPService();
		~TCPService();

		void open(std::size_t numWokers);
		void close();
		void run();

		template<typename EncoderT = NullEncoder>
		void startServer(const std::string& address, uint16_t port, TCPEventListenerPtr eventListener)
		{
			auto acceptor = std::make_shared<TCPAcceptorT<EncoderT>>(m_IoContext, address, port, eventListener);
			acceptor->start();
			m_Acceptors.emplace_back(acceptor);
			addEventListener(eventListener);
		}

		template<typename EncoderT = NullEncoder>
		TCPSessionPtr connectServer(const std::string& address, uint16_t port, TCPEventListenerPtr eventListener)
		{
			auto session = std::make_shared<TCPSession>(m_IoContext, eventListener->getContext().queue, new EncoderT, false);
			session->connect(address, port);
			addEventListener(eventListener);
			return session;
		}

	private:
		void addEventListener(TCPEventListenerPtr eventListener);

	private:
		boost::asio::io_context m_IoContext;
		std::list<std::thread> m_Workers;
		boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_WorkGuard;
		using AcceptorList = std::vector<TCPAcceptorPtr>;
		AcceptorList m_Acceptors;

		using EventListenerSet = std::set<TCPEventListenerPtr>;
		EventListenerSet asyncEventListeners;
		EventListenerSet syncEventListeners;
	};
}