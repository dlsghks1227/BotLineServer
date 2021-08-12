#include "../framework.h"
#include "JetbotProcessingComponent.h"

Component::JetbotProcessingComponent::JetbotProcessingComponent(Util::Object* owner) noexcept :
	Util::Component(owner)
{
	//std::function<OutputMemoryBitStream(JetbotProcessingComponent&, InputMemoryBitStream&, const SocketAddress&)> f = Connect;
	//std::make_pair(MessageType::CONNECT, &JetbotProcessingComponent::Connect),

	mProcessingStore = {
		{ MessageType::CONNECT, &JetbotProcessingComponent::Connect}
	};
}

Component::JetbotProcessingComponent::~JetbotProcessingComponent()
{
}

void Component::JetbotProcessingComponent::OnCreate() noexcept
{
}

void Component::JetbotProcessingComponent::OnUpdate(const Util::Timer& timer) noexcept
{
}

void Component::JetbotProcessingComponent::OnLateUpdate(const Util::Timer& timer) noexcept
{
}

void Component::JetbotProcessingComponent::OnRender(const Util::Timer& timer) noexcept
{
}

void Component::JetbotProcessingComponent::PacketProcessing(InputMemoryBitStream& input, const SocketAddress& fromAddress)
{
	MessageType messageType = MessageType::DEFAULT;
	input.Read(messageType);

	const auto itr = mProcessingStore.find(messageType);
	if (itr != mProcessingStore.cend())
	{
		itr->second(*this, input, fromAddress);
	}
}

void Component::JetbotProcessingComponent::Connect(InputMemoryBitStream& input, const SocketAddress& fromAddress)
{
	OutputMemoryBitStream outputStream = OutputMemoryBitStream();
	auto networkComponent = mObject->GetComponent<NetworkComponent>();

	const auto itr = mJetbotObjects.find(fromAddress);
	if (itr == mJetbotObjects.cend())
	{
		// 새로운 연결된 오브젝트 처리
		JetbotObjectPtr object = std::make_shared<JetbotObject>(fromAddress);
		mJetbotObjects[fromAddress] = object;
		networkComponent->HandlePacketFromNewObject(object, fromAddress);
	}

	outputStream.Write(MessageType::CONNECT);
	outputStream.Write(fromAddress.GetHash());
	networkComponent->SendPacket(outputStream, fromAddress);

	std::stringstream ss{};
	ss << "Jetbot Connected (" << fromAddress.ToString() << '-' << fromAddress.GetHash() << ')';
	mObject->GetSharedContext()->mUIManager->GetLog()->Add(ss.str());
}
