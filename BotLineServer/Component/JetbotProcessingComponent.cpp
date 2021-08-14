#include "../framework.h"
#include "JetbotProcessingComponent.h"

Component::JetbotProcessingComponent::JetbotProcessingComponent(Util::Object* owner) noexcept :
	BaseProcessingComponent<JetbotObject>(owner)
{
	//this->AddPacketProcessing<JetbotProcessingComponent>(MessageType::CONNECT, &JetbotProcessingComponent::InformationRequest);
	//std::function<OutputMemoryBitStream(JetbotProcessingComponent&, InputMemoryBitStream&, const SocketAddress&)> f = Connect;
	//std::make_pair(MessageType::CONNECT, &JetbotProcessingComponent::Connect),
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
	//const auto jetbotObjects = mDataComponent->GetJetbotObjects();

	//if (jetbotObjects.empty() == false)
	//{
	//	for (const auto& itr : jetbotObjects)
	//	{
	//		OutputMemoryBitStream outputStream = OutputMemoryBitStream();
	//		outputStream.Write(MessageType::INFORMATION_REQUEST);
	//		mNetworkComponent->SendPacket(outputStream, itr.first);
	//	}
	//}
}
