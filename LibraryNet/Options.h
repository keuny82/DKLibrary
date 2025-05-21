#pragma once
#include <stdint.h>

namespace dk
{
	namespace options
	{
		class Send
		{
		private:
			enum
			{
				PACKET_OPTION_ENCRYPTED = 0x01,
				PACKET_OPTION_COMPRESSED = 0x02,
				PACKET_OPTION_MULTI = 0x04,
			};

		public:
			Send() = default;
			~Send() = default;
			bool encrypted() const { return m_Options & PACKET_OPTION_ENCRYPTED; }
			Send& encrypted(bool value)
			{
				if (value)
					m_Options |= PACKET_OPTION_ENCRYPTED;
				else
					m_Options &= ~PACKET_OPTION_ENCRYPTED;
				return *this;
			}
			bool compressed() const { return m_Options & PACKET_OPTION_COMPRESSED; }
			Send& compressed(bool value)
			{
				if (value)
					m_Options |= PACKET_OPTION_COMPRESSED;
				else
					m_Options &= ~PACKET_OPTION_COMPRESSED;
				return *this;
			}
			bool multi() const { return m_Options & PACKET_OPTION_MULTI; }
			Send& multi(bool value)
			{
				if (value)
					m_Options |= PACKET_OPTION_MULTI;
				else
					m_Options &= ~PACKET_OPTION_MULTI;
				return *this;
			}
			void saveTo(uint8_t& options) const
			{
				options = m_Options;
			}
			void loadFrom(const uint8_t& options)
			{
				m_Options = options;
			}
		private:
			uint8_t m_Options = 0;
		};
	}
}