#include "../framework.h"
#include "WebProcessingComponent.h"

Component::WebProcessingComponent::WebProcessingComponent(Util::Object* owner) noexcept :
	BaseProcessingComponent<WebObject>(owner)
{
	mProcessingStorage.insert({ MessageType::INFO_CURRENT_STATE,	std::bind(&WebProcessingComponent::InfoCurrentState,	this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3) });

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
	BaseProcessingComponent::OnUpdate(timer);
}

void Component::WebProcessingComponent::OnLateUpdate(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnLateUpdate(timer);
}

void Component::WebProcessingComponent::OnRender(const Util::Timer& timer) noexcept
{
	BaseProcessingComponent::OnRender(timer);
}

void Component::WebProcessingComponent::InfoCurrentState(InputMemoryBitStream& input, const SocketAddress& fromAddress, const WebObjectPtr& object) noexcept
{
	const auto jetbotComponent = mObject->GetComponent<JetbotProcessingComponent>();
	const auto jetbots = jetbotComponent->GetObjects();
	if (jetbots.empty() == false)
	{
		for (const auto& itr : jetbotComponent->GetObjects())
		{
			OutputMemoryBitStream informationRequestMessage = OutputMemoryBitStream();
			informationRequestMessage.Write(MessageType::INFO_CURRENT_STATE);

			informationRequestMessage.Write(itr.first.GetHash());
			informationRequestMessage.Write(itr.second->GetJetbotState().mVoltage);
			informationRequestMessage.Write(itr.second->GetJetbotState().mCpuAverage);
			informationRequestMessage.Write(itr.second->GetJetbotState().mMemory);
			informationRequestMessage.Write(itr.second->GetJetbotState().mDisk);

			informationRequestMessage.Write(itr.second->GetJetbotState().mPosition);

			informationRequestMessage.Write(itr.second->GetJetbotState().mIsWorking);
			informationRequestMessage.Write(itr.second->GetJetbotState().mIsStop);

			mNetworkComponent->SendPacket(informationRequestMessage, fromAddress);
		}
	}

	OutputMemoryBitStream endDataMessage = OutputMemoryBitStream();
	endDataMessage.Write(MessageType::END_DATA);
	endDataMessage.Write(MessageType::INFO_CURRENT_STATE);
	mNetworkComponent->SendPacket(endDataMessage, fromAddress);
}
