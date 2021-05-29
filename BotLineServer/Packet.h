#pragma once
#include "stdafx.h"

enum class MessageType : uint8_t
{
	JETBOT_CONNECT		= 0U,
	JETBOT_DISCONNECT,

	CONTROL_CONNECT,
	CONTROL_DISCONNECT,

	CONNECT_CHECK,
};