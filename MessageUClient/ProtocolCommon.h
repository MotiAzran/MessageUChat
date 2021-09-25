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

	struct ResponseHeader
	{
		uint8_t version;
		ResponseCode code;
		uint32_t payload_size;

		ResponseHeader(uint8_t version, ResponseCode code, uint32_t payload_size) :
			version(version), code(code), payload_size(payload_size) {}
	};
}
