#pragma once
#include "stdafx.h"

class SocketAddress;
class ControllerObject final : public BotLineObject
{
public:
	ControllerObject(const SocketAddress& address) noexcept;
	~ControllerObject() = default;
};

