#include <pch.h>
#include "Encoder.h"
#include "TCPSession.h"
#include "TCPMessage.h"
#include "RecvQueue.h"

namespace dk
{
	Stream Encoder::encode(const TCPMessage& message, const options::Send& options)
	{
		Stream stream(message.reserved());
		stream.write(sizeof(uint32_t));
		stream.write(getHeaderSize());
		stream << message.getNo();
		message.serialize(stream);
		doEncode(stream, options);
		setSize(stream);
		return stream;
	}

	std::optional<Stream> Encoder::decode(RecvQueue& recvQueue)
	{
		uint32_t size = 0;
		if (!validateSize(recvQueue, &size) || !enough(recvQueue, size))
		{
			return std::nullopt;
		}

		recvQueue.pop(sizeof(uint32_t));

		Stream stream;
		if (recvQueue.detachable(size))
		{
			stream = recvQueue.detach();
		}
		else
		{
			Stream streamRecv(size);
			recvQueue.peek(streamRecv.getWritableBuffer(), size);
			streamRecv.commit(size);
			recvQueue.pop(size);
			stream = streamRecv;
		}

		doDecode(stream);
		stream.consume(getHeaderSize());
		return stream;
	}

	void Encoder::setSize(Stream& stream)
	{
		auto pSize = reinterpret_cast<uint32_t*>(stream.getBuffer());
		uint32_t size = uint32_t(stream.size() - sizeof(uint32_t));
		*pSize = size;
	}

	bool Encoder::validateSize(RecvQueue& recvQueue, uint32_t* size)
	{
		const auto minimumSize = sizeof(TCPMessageNo) + getHeaderSize();
		const auto minimumPacketSize = sizeof(uint32_t) + minimumSize;
		if (recvQueue.getReadableSize() < minimumPacketSize)
		{
			return false;
		}

		recvQueue.peek((void*)size, sizeof(uint32_t));

		limitSize(*size);
		if (*size < minimumSize)
		{
			throw TCPException("packet size too small!!!");
		}

		return true;
	}

	bool Encoder::enough(RecvQueue& recvQueue, uint32_t size)
	{
		const auto packetSize = size + sizeof(uint32_t);
		const auto readableSize = recvQueue.getReadableSize();
		if (readableSize < packetSize)
		{
			const auto currentSize = readableSize + recvQueue.getWritableSize();
			if (currentSize < packetSize)
			{
				recvQueue.prepare(packetSize - readableSize);
			}
			return false;
		}
		return true;
	}

	void Encoder::finishAccepting()
	{
		if (m_Session)
			m_Session->fireAccepted();
	}

	void Encoder::finishConnecting(const std::error_code& error)
	{
		if (m_Session)
			m_Session->fireConnected(error);
	}

	void Encoder::finishClosing()
	{
		if (m_Session)
		{
			m_Session->fireClosed();
		}
	}
}