#include "../framework.h"
#include "JetbotObject.h"


JetbotObject::JetbotObject(const SocketAddress& address) noexcept :
	BotLineObject(address),
	mState()
{
	this->mType = ObjectType::JETBOT;
}