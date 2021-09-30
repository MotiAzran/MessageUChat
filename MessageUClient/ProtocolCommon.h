#pragma once

#include <cstdint>

namespace Protocol
{
	enum class RequestCode : uint16_t
	{
		Register = 1000,
		ClientsList,
		PublicKeyRequest,
		SendMessageToUser,
		WaitingMessagesRequest,
	};

	enum class ResponseCode : uint16_t
	{
		RegisterSuccess = 2000,
		ClientsListSent,
		PublicKeySent,
		MessageSentToUser,
		WaitingMessagesReceived,

		GeneralError = 9000
	};

	enum class MessageType : uint8_t
	{
		SymetricKeyRequest = 1,
		SendSymetricKey,
		Text
	};

	struct MessageEntry
	{
		Types::ClientID client_id;
		uint32_t message_id;
		MessageType message_type;
		uint32_t message_size;
		std::string message_content;

		MessageEntry(const Types::ClientID& client_id, const uint32_t message_id, const MessageType message_type,
			const uint32_t message_size, std::string&& message_content) :
			client_id(client_id), message_id(message_id), message_type(message_type),
			message_size(message_size), message_content(std::move(message_content)) {}
	};
}
