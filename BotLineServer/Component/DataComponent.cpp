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

void Component::DataComponent::AddObject(const ObjectType& type, const SocketAddress& fromAddress) noexcept
{

	const auto itr = mBotLineObjects.find(fromAddress);
	if (itr == mBotLineObjects.cend()) 
	{
		BotLineObjectPtr object = std::make_shared<BotLineObject>(fromAddress);
		switch (type)
		{
			case ObjectType::JETBOT:
			{
				mJetBotObjects[fromAddress] = std::dynamic_pointer_cast<JetbotObject>(object);
				break;
			}
			case ObjectType::XAVIER:
			{
				mXavierObjects[fromAddress] = std::dynamic_pointer_cast<XavierObject>(object);
				break;
			}
			default:
			{
				break;
			}
		}

		mBotLineObjects[fromAddress] = object;
	}
}

void Component::DataComponent::RemoveObject(const BotLineObjectPtr& object) noexcept
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