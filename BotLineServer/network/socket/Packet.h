#pragma once
#include "../../framework.h"

enum class MessageType : uint8_t
{
	CONNECT = 0U,
	DISCONNECT,

	INFORMATION_REQUEST,
	END_DATA,
	CONTROL,

	CONNECT_CHECK,

	ALL_STOP,

	DEFAULT = 0xFF
};