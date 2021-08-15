#include "../framework.h"
#include "WebProcessingComponent.h"

Component::WebProcessingComponent::WebProcessingComponent(Util::Object* owner) noexcept :
	BaseProcessingComponent<WebObject>(owner)
{
}

Component::WebProcessingComponent::~WebProcessingComponent()
{
}

void Component::WebProcessingComponent::OnCreate() noexcept
{
	BaseProcessingComponent::OnCreate();
}

void Component::WebProcessingComponent::OnUpdate(const Util::Timer& timer) noexcept
{
}

void Component::WebProcessingComponent::OnLateUpdate(const Util::Timer& timer) noexcept
{
}

void Component::WebProcessingComponent::OnRender(const Util::Timer& timer) noexcept
{
}

void Component::WebProcessingComponent::InformationRequest(InputMemoryBitStream& input, const SocketAddress& fromAddress, const WebObjectPtr& object) noexcept
{
}
