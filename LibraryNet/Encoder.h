#pragma once

#include "prerequisites.h"
#include "Stream.h"
#include <optional>
#include <system_error>


enum EncoderMessageNo : dk::TCPMessageNo
{
	P_HEARTBEAT_PING = 1,
	P_HEARTBEAT_PONG = 2,
	HEARTBEAT_MESSAGE_NO_END
};

struct HEARTBEAT_PING : public dk::TCPMessageT<P_HEARTBEAT_PING>
{
	HEARTBEAT_PING() : persistent(false) {}
	HEARTBEAT_PING(bool persistent) : persistent(persistent) {}
	virtual void serialize(dk::Stream& stream) const override
	{
		stream << persistent;
	}
	virtual void deserialize(dk::Stream& stream) override
	{
		stream >> persistent;
	}
	bool persistent;
};
struct HEARTBEAT_PONG : public dk::TCPMessageT<P_HEARTBEAT_PONG>
{
	HEARTBEAT_PONG() = default;
	virtual void serialize(dk::Stream& stream) const override
	{
	}
	virtual void deserialize(dk::Stream& stream) override
	{
	}
};

namespace dk
{
	class RecvQueue;
	class TCPSession;

	class Endcoder
	{
	public:
		Endcoder() = default;
		virtual ~Endcoder() = default;
		virtual void clear() {}
		virtual void startAccepting() { finishAccepting(); }
		virtual void startConnecting(const std::error_code& error) { finishConnecting(error); }
		virtual void startClosing() { finishClosing(); }
		virtual bool handleMessage(Stream&) { return false; }
		virtual bool limitOutgoingNum(size_t) const { return false; }
		virtual bool established() const { return true; }
		
		void OnCreated(TCPSession* session)
		{
			m_Session = session;
		}
		Stream encode(const TCPMessage& message, const options::Send& options);
		std::optional<Stream> decode(RecvQueue& recvQueue);
		virtual void incrNumOfPendingMessages() {}
		virtual void decrNumOfPendingMessages() {}

	protected:
		void finishAccepting();
		void finishConnecting(const std::error_code& error);
		void finishClosing();
		virtual std::size_t getHeaderSize() const = 0;

	private:
		virtual void doEncode(Stream& stream, const options::Send& options) = 0;
		virtual void doDecode(Stream& stream) = 0;
		virtual void limitSize(uint32_t) const {}
		void setSize(Stream& stream);
		bool validateSize(RecvQueue& recvQueue, uint32_t* size);
		bool enough(RecvQueue& recvQueue, uint32_t size);

	protected:
		TCPSession* m_Session = nullptr;

	};

	template<typename PacketHeaderT>
	class EncoderT : public Endcoder
	{
	protected:
		EncoderT() = default;
		virtual ~EncoderT() = default;
		virtual std::size_t getHeaderSize() const override
		{
			return sizeof(PacketHeaderT);
		}
	};
}