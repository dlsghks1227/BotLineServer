#include "../framework.h"
#include "DataManager.h"

void Data::DataManager::Add(const ObjectType& type, const SocketAddress& fromAddress) noexcept
{

}

void Data::DataManager::Remove(const BotLineObjectPtr& object) noexcept
{
	const SocketAddress& address = object->GetSocketAddress();

	switch (object->GetObjectType())
	{
		case ObjectType::JETBOT:
		{
			mJetBotObjects.erase(address);
			break;
		}
		case ObjectType::XAVIER:
		{
			mJetBotObjects.erase(address);
			break;
		}
		default:
		{
			break;
		}
	}
	mBotLineObjects.erase(address);
}

void Data::DataManager::AddJetbotObject(const SocketAddress& fromAddress) noexcept
{
	JetbotObjectPtr object = std::make_shared<JetbotObject>(fromAddress);
	mBotLineObjects[fromAddress] = object;
	mJetBotObjects[fromAddress] = object;
}

void Data::DataManager::AddXavierObject(const SocketAddress& fromAddress) noexcept
{
	XavierObjectPtr object = std::make_shared<XavierObject>(fromAddress);
	mBotLineObjects[fromAddress] = object;
	mXavierObjects[fromAddress] = object;
}