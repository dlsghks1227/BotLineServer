#include "stdafx.h"
#include "JetbotObject.h"


JetbotObject::JetbotObject(const SocketAddress& address) noexcept :
	BotLineObject(address)
{
	mType = ObjectType::JETBOT;
}