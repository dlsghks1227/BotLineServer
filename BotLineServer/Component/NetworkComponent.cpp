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
	mXavierProcessingComponent = mObject->GetComponent<XavierProcessingComponent>();
	mWebProcessingComponent = mObject->GetComponent<WebProcessingComponent>();
}

void Component::NetworkComponent::OnUpdate(const Util::Timer& timer) noexcept
{
	this->ProcessIncomingPackets(timer);
}

void Component::NetworkComponent::OnLateUpdate(const Util::Timer& timer) noexcept
{
	static double elapsedTime = 0.0;
	elapsedTime += timer.GetElapsedSeconds();

	this->CheckForDisconnect();

	if (elapsedTime > sCycleTime)
	{
		this->VerifyConnection();
		elapsedTime = 0.0;
	}
}

void Component::NetworkComponent::OnRender(const Util::Timer& timer) noexcept
{
}

void Component::NetworkComponent::SendPacket(const OutputMemoryBitStream& outputStream, const SocketAddress& fromAddress) noexcept
{
	int sentByteCount = mSocket->SendTo(outputStream.GetBufferPtr(), outputStream.GetByteLength(), fromAddress);
}

void Component::NetworkComponent::AddObject(const BotLineObjectPtr& object) noexcept
{
	mBotLineObjects[object->GetSocketAddress()] = object;
}

void Component::NetworkComponent::RemoveObject(const SocketAddress& address) noexcept
{
	mBotLineObjects.erase(address);
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
			this->HandleDisconnectedObject(itr->second);
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

		//SystemTime receivedTime = nextPacket.GetReceivedTime();
		//InputMemoryBitStream inputStream = nextPacket.GetPacketBuffer();

		// Process Packet ...
		this->PacketProcessing(nextPacket.GetPacketBuffer(), nextPacket.GetFromAddress());

		mPacketQueue.pop();
	}
}

void Component::NetworkComponent::CheckForDisconnect() noexcept
{
	std::vector<BotLineObjectPtr> disconnectedObjects;
	SystemTime currentTime = std::chrono::system_clock::now();

	for (const auto& itr : mBotLineObjects)
	{
		std::chrono::duration<double> elapsedTime = currentTime - itr.second->GetLastPacketTime();
		if (elapsedTime.count() > sTimeout)
		{
			disconnectedObjects.push_back(itr.second);
		}
	}

	if (disconnectedObjects.empty() == false)
	{
		for (const auto& itr : disconnectedObjects)
		{
			this->HandleDisconnectedObject(itr);
		}
	}
}

void Component::NetworkComponent::VerifyConnection() noexcept
{
	if (mBotLineObjects.empty() == false)
	{
		for (const auto& itr : mBotLineObjects)
		{
			OutputMemoryBitStream outputStream = OutputMemoryBitStream();
			outputStream.Write(MessageType::CONNECT_CHECK);

			this->SendPacket(outputStream, itr.first);
		}
	}
}

void Component::NetworkComponent::PacketProcessing(InputMemoryBitStream& input, const SocketAddress& fromAddress) noexcept
{
	ObjectType objectType = ObjectType::DEFAULT;
	input.Read(objectType);

	switch (objectType)
	{
		case ObjectType::JETBOT:
		{
			mJetbotProcessingComponent->PacketProcessing(input, fromAddress);
			break;
		}
		case ObjectType::XAVIER:
		{
			mXavierProcessingComponent->PacketProcessing(input, fromAddress);
			break;
		}
		case ObjectType::WEB:
		{
			mWebProcessingComponent->PacketProcessing(input, fromAddress);
			break;
		}
		default:
		{
			mObject->GetSharedContext()->mUIManager->GetLog()->Add("Failed to add new object. Unknown Object Type!");
			break;
		}
	}
}

void Component::NetworkComponent::HandleDisconnectedObject(const BotLineObjectPtr& object) noexcept
{
	switch (object->GetObjectType())
	{
		case ObjectType::JETBOT:
		{
			mJetbotProcessingComponent->RemoveObject(object->GetSocketAddress());
			break;
		}
		case ObjectType::XAVIER:
		{
			mXavierProcessingComponent->RemoveObject(object->GetSocketAddress());
			break;
		}
		case ObjectType::WEB:
		{
			mWebProcessingComponent->RemoveObject(object->GetSocketAddress());
			break;
		}
	}
}