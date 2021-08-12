#include "../framework.h"
#include "ControllerObject.h"

ControllerObject::ControllerObject(const SocketAddress& address) noexcept :
	BotLineObject(address)
{
	mType = ObjectType::CONTROLLER;
}
