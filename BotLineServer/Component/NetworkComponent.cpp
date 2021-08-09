#include "NetworkComponent.h"

Component::NetworkComponent::NetworkComponent(Util::Object* owner) noexcept
	: Util::Component(owner)
{
}

Component::NetworkComponent::~NetworkComponent()
{
}

void Component::NetworkComponent::Initialize(const std::shared_ptr<Util::Object>& ui) noexcept
{
	mUI = ui;
}
