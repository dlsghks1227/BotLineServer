#include "../framework.h"
#include "JetbotProcessingComponent.h"

Component::JetbotProcessingComponent::JetbotProcessingComponent(Util::Object* owner) noexcept :
	BaseProcessingComponent<JetbotObject>(owner)
{
	mProcessingStorage.insert({ MessageType::INFORMATION_REQUEST,	std::bind(&JetbotProcessingComponent::InformationRequest,	this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
}

Component::JetbotProcessingComponent::~JetbotProcessingComponent()
{
}

void Component::JetbotProcessingComponent::OnCreate() noexcept
{
	BaseProcessingComponent::OnCreate();
}

void Component::JetbotProcessingComponent::OnUpdate(const Util::Timer& timer) noexcept
{
}

void Component::JetbotProcessingComponent::OnLateUpdate(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnLateUpdate(timer);

	static double elapsedTime = 0.0;
	elapsedTime += timer.GetElapsedSeconds();

	if (elapsedTime > sUpdateCycle)
	{
		this->UpdateJetbotInfomation();
		elapsedTime = 0.0;
	}
}

void Component::JetbotProcessingComponent::OnRender(const Util::Timer& timer) noexcept
{
}

void Component::JetbotProcessingComponent::InformationRequest(InputMemoryBitStream& input, const SocketAddress& fromAddress, const JetbotObjectPtr& object) noexcept
{
	JetbotState state;

	input.Read(state.mVoltage);
	input.Read(state.mCpuAverage);
	input.Read(state.mMemory);
	input.Read(state.mDisk);

	object->SetJetbotState(state);
}

void Component::JetbotProcessingComponent::UpdateJetbotInfomation() noexcept
{
	if (this->mObjects.empty() == false)
	{
		for (const auto& itr : this->mObjects)
		{
			OutputMemoryBitStream outputStream = OutputMemoryBitStream();
			outputStream.Write(MessageType::INFORMATION_REQUEST);
			mNetworkComponent->SendPacket(outputStream, itr.first);
		}
	}
}
