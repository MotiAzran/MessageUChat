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
}
