#pragma once
#include "../framework.h"


class SocketAddress;
class WebObject final : public BotLineObject
{
public:
	WebObject(const SocketAddress& address) noexcept;
	virtual ~WebObject() = default;

private:
};