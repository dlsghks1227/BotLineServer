#include "../framework.h"
#include "DataComponent.h"

Component::DataComponent::DataComponent(Util::Object* owner) noexcept :
	Component(owner)
{
}

Component::DataComponent::~DataComponent()
{
}

void Component::DataComponent::OnCreate() noexcept
{
}

void Component::DataComponent::OnUpdate(const Util::Timer& timer) noexcept
{
}

void Component::DataComponent::OnLateUpdate(const Util::Timer& timer) noexcept
{
}

void Component::DataComponent::OnRender(const Util::Timer& timer) noexcept
{
}

void Component::DataComponent::AddJetbotObject(const SocketAddress& fromAddress) noexcept
{
	JetbotObjectPtr object = std::make_shared<JetbotObject>(fromAddress);
	mBotLineObjects[fromAddress] = object;
	mJetBotObjects[fromAddress] = object;
}

void Component::DataComponent::AddXavierObject(const SocketAddress& fromAddress) noexcept
{
	XavierObjectPtr object = std::make_shared<XavierObject>(fromAddress);
	mBotLineObjects[fromAddress] = object;
	mXavierObjects[fromAddress] = object;
}

void Component::DataComponent::Remove(const BotLineObjectPtr& object) noexcept
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

	mObject->GetSharedContext()->mUIManager->GetLog()->Add("Disconnected");
}