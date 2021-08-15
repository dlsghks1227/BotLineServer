#include "../framework.h"
#include "WebObject.h"

WebObject::WebObject(const SocketAddress& address) noexcept :
	 BotLineObject(address)
{
}
