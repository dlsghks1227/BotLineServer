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
	mSocket = std::make_unique<UDPSocket>();
	mSocket->Initialize();

	mSocket->Bind(SocketAddress(INADDR_ANY, this->sPort));
	mSocket->SetNonBlockingMode(true);

	mJetbotProcessingComponent = mObject->GetComponent<JetbotProcessingComponent>();
}

void Component::NetworkComponent::OnUpdate(const Util::Timer& timer) noexcept
{
	this->ProcessIncomingPackets(timer);
}

void Component::NetworkComponent::OnLateUpdate(const Util::Timer& timer) noexcept
{
	static double checkingTime = 0.0;
	checkingTime += timer.GetElapsedSeconds();

	this->CheckForDisconnect();

	if (checkingTime > sCycleTime)
	{
		this->VerifyConnection();
		checkingTime = 0.0;
	}
}

void Component::NetworkComponent::OnRender(const Util::Timer& timer) noexcept
{
}

void Component::NetworkComponent::SendPacket(const OutputMemoryBitStream& outputStream, const SocketAddress& fromAddress) noexcept
{
	int sentByteCount = mSocket->SendTo(outputStream.GetBufferPtr(), outputStream.GetByteLength(), fromAddress);
}

void Component::NetworkComponent::HandlePacketFromNewObject(const BotLineObjectPtr& object, const SocketAddress& fromAddress) noexcept
{
	mBotLineObjects[fromAddress] = object;
}

void Component::NetworkComponent::ProcessIncomingPackets(const Util::Timer& timer) noexcept
{
	this->ReadIncomingPacketsIntoQueue();
	this->ProcessQueuedPackets();
}

void Component::NetworkComponent::ReadIncomingPacketsIntoQueue() noexcept
{
	char buffer[sBufferSize] = {};
	int bufferSize = sizeof(buffer);
	InputMemoryBitStream inputStream(buffer, bufferSize * 8);
	SocketAddress fromAddress;

	int readByteCount = mSocket->ReceiveFrom(buffer, bufferSize, fromAddress);
	if (readByteCount == 0)
	{
		return;
	}
	else if (readByteCount == -WSAECONNRESET)
	{
		const auto itr = mBotLineObjects.find(fromAddress);
		if (itr != mBotLineObjects.cend())
		{
			this->HandleObjectDisconnect(itr->second);
		}
		return;
	}
	else if (readByteCount > 0)
	{
		inputStream.ResetToCapacity(readByteCount);
		mPacketQueue.emplace(std::chrono::system_clock::now(), inputStream, fromAddress);
	}
}

void Component::NetworkComponent::ProcessQueuedPackets() noexcept
{
	while (mPacketQueue.empty() == false)
	{
		ReceivedPacket& nextPacket = mPacketQueue.front();

		// Process Packet ...
		SystemTime receivedTime = nextPacket.GetReceivedTime();
		InputMemoryBitStream inputStream = nextPacket.GetPacketBuffer();

		this->PacketProcessing(nextPacket.GetPacketBuffer(), nextPacket.GetFromAddress());

		mPacketQueue.pop();
	}
}

void Component::NetworkComponent::CheckForDisconnect() noexcept
{
}

void Component::NetworkComponent::VerifyConnection() noexcept
{
}

void Component::NetworkComponent::PacketProcessing(InputMemoryBitStream& input, const SocketAddress& fromAddress) noexcept
{
	ObjectType objectType = ObjectType::DEFAULT;
	input.Read(objectType);

	if (objectType == ObjectType::JETBOT)
	{
		mJetbotProcessingComponent->PacketProcessing(input, fromAddress);
	}
	//else if (objectType == ObjectType::XAVIER)
	//{

	//}
	else
	{
		mObject->GetSharedContext()->mUIManager->GetLog()->Add("Failed to add new object. Unknown Object Type!");
	}
}

void Component::NetworkComponent::HandleObjectDisconnect(const BotLineObjectPtr& object) noexcept
{
}
