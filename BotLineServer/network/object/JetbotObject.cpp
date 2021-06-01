#include "../../framework.h"
#include "JetbotObject.h"


JetbotObject::JetbotObject(const SocketAddress& address) noexcept :
	BotLineObject(address),
	mVoltage(0.0f),
	mCpuAverage(0.0f),
	mMemory(0.0f),
	mDisk(0.0f)
{
	mType = ObjectType::JETBOT;
}