#include "../framework.h"
#include "XavierProcessingComponent.h"

Component::XavierProcessingComponent::XavierProcessingComponent(Util::Object* owner) noexcept :
	BaseProcessingComponent(owner)
{
}

Component::XavierProcessingComponent::~XavierProcessingComponent()
{
}

void Component::XavierProcessingComponent::OnCreate() noexcept
{
	BaseProcessingComponent::OnCreate();
}

void Component::XavierProcessingComponent::OnUpdate(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnUpdate(timer);

}

void Component::XavierProcessingComponent::OnLateUpdate(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnLateUpdate(timer);

}

void Component::XavierProcessingComponent::OnRender(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnRender(timer);

}

void Component::XavierProcessingComponent::InformationRequest(InputMemoryBitStream& input, const SocketAddress& fromAddress, const XavierObjectPtr& object) noexcept
{
	XavierState state = XavierState();

	input.Read(state.mCpuAverage);
	input.Read(state.mMemory);
	input.Read(state.mDisk);

	object->SetXavierState(state);
}