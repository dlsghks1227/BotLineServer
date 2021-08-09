#include "NetworkComponent.h"

Component::NetworkComponent::NetworkComponent(Util::Object* owner) noexcept :
	Util::Component(owner)
{
}

Component::NetworkComponent::~NetworkComponent()
{
}

void Component::NetworkComponent::OnCreate() noexcept
{
	auto jetbotProcessingComponent = mObject->GetComponent<JetbotProcessingComponent>();
}

void Component::NetworkComponent::OnUpdate(const Util::Timer& timer) noexcept
{
}

void Component::NetworkComponent::OnLateUpdate(const Util::Timer& timer) noexcept
{
}

void Component::NetworkComponent::OnRender(const Util::Timer& timer) noexcept
{
}
