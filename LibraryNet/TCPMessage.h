#pragma once

#include "Prerequisites.h"
#include "Serializable.h"
#include <sstream>

#include <flatbuffers/flatbuffers.h>


namespace dk
{
	struct TCPMessage
		: public Serializable
	{
		virtual	~TCPMessage() = default;
		virtual	TCPMessageNo	getNo()	const = 0;
		void	reserve(std::size_t size) { size_ = size; }
		std::size_t	reserved()	const { return size_; }
	private:
		std::size_t	size_{ 0 };
	};

	template<TCPMessageNo messageNo>
	struct TCPMessageT
		: public TCPMessage
	{
		enum : TCPMessageNo { kNO = messageNo };
		virtual	TCPMessageNo	getNo()	const	override {
			return messageNo;
		}
	};

	template<typename FBMessageT, typename std::enable_if<std::is_base_of<flatbuffers::NativeTable, FBMessageT>::value>::type = 0>
	struct FBMessageAdaptorT : public TCPMessageT<FBMessageT::kNo>
	{
		FBMessageAdaptorT() = default;
		FBMessageAdaptorT(const FBMessageT& message) : m_Msg(message)
		{
			reserve(m_Msg.GetSize());
		}
		virtual void serialize(Stream& stream) const override
		{
			flatbuffers::FlatBufferBuilder fbb;
			fbb.Finish(FBMessageT:TableType::Pack(fbb, &m_Msg));
			auto msgSize = fbb.GetSize();
			OStreamVerifier::Verify(stream, msgSize);
			stream.write(fbb.GetBufferPointer(), msgSize);
		}

		virtual void deserialize(Stream& stream) override
		{
			if (!flatbuffers::Verifier(stream.getReadableBuffer(), stream.getReadableSize()).VerifyBuffer<typename FBMessageT::TableType>())
				throw StreamException();
			message = flatbuffers::GetRoot<typename FBMessageT::TableType>(stream.getReadableBuffer());
			stream.consume(stream.getReadableSize());
		}

		FBMessageT m_Msg;
		const typename FBMessageT::TableType* message{ nullptr };
	};
}