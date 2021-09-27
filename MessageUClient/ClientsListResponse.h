#pragma once

#include <iterator>
#include "Response.h"
#include "Common.h"
#include "Types.h"

namespace Protocol
{
	class ClientsListResponse : public Response
	{
	public:
		struct ClientEntry
		{
			static const uint32_t SIZE = Common::CLIENT_ID_SIZE_BYTES + Common::MAX_CLIENT_NAME_LENGTH;

			Types::ClientID id;
			std::string name;

			ClientEntry(const Types::ClientID& id, const std::string& name) :
				id(id), name(name) {}
		};

	public:
		explicit ClientsListResponse(Stream* stream);
		virtual ~ClientsListResponse() = default;

		ClientEntry get_next_entry();
		bool is_done() const { return 0 == _remaining_entries; }

	private:
		Stream* _stream;
		uint32_t _remaining_entries;
	};
}
