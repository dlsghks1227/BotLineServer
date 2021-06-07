#include "../../framework.h"
#include "XavierObject.h"

XavierObject::XavierObject(const SocketAddress& address) noexcept :
	 BotLineObject(address),
	mCpuAverage(0.0f),
	mMemory(0.0f),
	mDisk(0.0f)
{
	mType = ObjectType::XAVIER;
}
