#include "../framework.h"
#include "WebProcessingComponent.h"

Component::WebProcessingComponent::WebProcessingComponent(Util::Object* owner) noexcept :
	BaseProcessingComponent<WebObject>(owner)
{
	mProcessingStorage.insert({ MessageType::INFORMATION_REQUEST,	std::bind(&WebProcessingComponent::InformationRequest,	this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) });

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
	mObject->GetSharedContext()->mUIManager->GetLog()->Add("InformationRequest");

	const auto jetbotComponent = mObject->GetComponent<JetbotProcessingComponent>();
	const auto jetbots = jetbotComponent->GetObjects();
	if (jetbots.empty() == false)
	{
		for (const auto& itr : jetbotComponent->GetObjects())
		{
			OutputMemoryBitStream informationRequestMessage = OutputMemoryBitStream();
			informationRequestMessage.Write(MessageType::INFORMATION_REQUEST);
			informationRequestMessage.Write(itr.first.GetHash());
			informationRequestMessage.Write(itr.second->GetJetbotState().mVoltage);
			informationRequestMessage.Write(itr.second->GetJetbotState().mCpuAverage);
			informationRequestMessage.Write(itr.second->GetJetbotState().mMemory);
			informationRequestMessage.Write(itr.second->GetJetbotState().mDisk);
			mNetworkComponent->SendPacket(informationRequestMessage, fromAddress);
		}
	}

	OutputMemoryBitStream endDataMessage = OutputMemoryBitStream();
	endDataMessage.Write(MessageType::END_DATA);
	mNetworkComponent->SendPacket(endDataMessage, fromAddress);
}
