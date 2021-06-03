#pragma once
#include "../../framework.h"

enum class MessageType : uint8_t
{
	CONNECT = 0U,
	DISCONNECT,

	INFORMATION_REQUEST,
	CONTROL,

	CONNECT_CHECK,
};