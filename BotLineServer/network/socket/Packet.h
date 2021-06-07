#pragma once
#include "../../framework.h"

enum class MessageType : uint8_t
{
	CONNECT = 0U,
	DISCONNECT,

	INFORMATION_REQUEST,
	CONTROL,

	CONNECT_CHECK,

	ALL_STOP,

	DEFAULT = 0xFF
};

enum class JetbotMoveState : uint8_t
{
	GO = 0U,
	STOP,
};