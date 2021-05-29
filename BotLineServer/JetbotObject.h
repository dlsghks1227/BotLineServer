#pragma once
#include "stdafx.h"

class SocketAddress;
class JetbotObject final : public BotLineObject
{
public:
	JetbotObject(const SocketAddress& address) noexcept;
	~JetbotObject() = default;
};