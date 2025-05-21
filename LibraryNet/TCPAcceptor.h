#pragma once
#include "TCPEvent.h"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <string>

namespace dk
{
	class TCPAcceptor
	{
	public:
		virtual ~TCPAcceptor() = default;
		void start();

	protected:
		TCPAcceptor(boost::asio::io_context& ioContext, const std::string& address, uint16_t port, TCPEventListenerPtr eventListener);
	
	private:
		virtual TCPSessionPtr createSession() const = 0;
		void asyncAccept();
		void onAccepted(const std::error_code& error, TCPSessionPtr session);

	protected:
		boost::asio::io_context& m_IoContext;
		TCPEventListenerPtr m_EventListener;

	private:
		boost::asio::ip::tcp::acceptor m_Acceptor;		
	};

	template<typename EncoderT>
	class TCPAcceptorT : public TCPAcceptor
	{
	public:
		TCPAcceptorT(boost::asio::io_context& ioContext, const std::string& address, uint16_t port, TCPEventListenerPtr eventListener)
			: TCPAcceptor(ioContext, address, port, eventListener)
		{
		}

		virtual ~TCPAcceptorT() = default;
	private:
		virtual TCPSessionPtr createSession() const override
		{
			return std::make_shared<TCPSession>(m_IoContext, m_EventListener->getContext().queue, new EncoderT, true);
		}
	};
}