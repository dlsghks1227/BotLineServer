#include "../framework.h"
#include "XavierObject.h"

XavierObject::XavierObject(const SocketAddress& address) noexcept :
	BotLineObject(address),
	mState()
{
	this->mType = ObjectType::XAVIER;
}
