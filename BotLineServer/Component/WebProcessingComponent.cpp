#include "../framework.h"
#include "WebProcessingComponent.h"

Component::WebProcessingComponent::WebProcessingComponent(Util::Object* owner) noexcept :
	Util::Component(owner)
{
}

Component::WebProcessingComponent::~WebProcessingComponent()
{
}

void Component::WebProcessingComponent::OnCreate() noexcept
{
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

void Component::WebProcessingComponent::PacketProcessing(InputMemoryBitStream& input, const SocketAddress& fromAddress) noexcept
{
}

void Component::WebProcessingComponent::Connect(InputMemoryBitStream& input, const SocketAddress& fromAddress, const JetbotObjectPtr& object) noexcept
{
}

void Component::WebProcessingComponent::InformationRequest(InputMemoryBitStream& input, const SocketAddress& fromAddress, const JetbotObjectPtr& object) noexcept
{
}
