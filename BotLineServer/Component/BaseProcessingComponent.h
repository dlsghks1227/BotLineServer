#pragma once
#include "../framework.h"

namespace Component
{
	class NetworkComponent;
	template<typename T>
	class BaseProcessingComponent : public Util::Component
	{
	public:
		BaseProcessingComponent(Util::Object* owner) noexcept : Util::Component(owner)
		{
			static_assert(std::is_base_of<BotLineObject, T>::value, "T must derive from BotLineObject");

			mProcessingStore = {
				{MessageType::CONNECT,		&BaseProcessingComponent::Connect},
				{MessageType::DISCONNECT,	&BaseProcessingComponent::Disconnect},
			};
		}
		virtual ~BaseProcessingComponent() = default;

		void	OnCreate()	noexcept	override
		{
			mNetworkComponent = mObject->GetComponent<NetworkComponent>();
		}

		void	PacketProcessing(InputMemoryBitStream& input, const SocketAddress& fromAddress) noexcept
		{
			MessageType messageType = MessageType::DEFAULT;
			input.Read(messageType);

			const auto obj = mObjects.find(fromAddress);
			if (obj != mObjects.cend())
			{
				obj->second->UpdateLastPacketTime();

				const auto func = mProcessingStore.find(messageType);
				if (func != mProcessingStore.cend())
				{
					obj->second->SetLastMessageType(messageType);
					func->second(*this, input, fromAddress, obj->second);
				}
			}
			else
			{
				if (messageType == MessageType::CONNECT)
				{
					this->Connect(input, fromAddress, nullptr);

					std::shared_ptr<T> object = std::make_shared<T>(fromAddress);
					mObjects[fromAddress] = object;

					mNetworkComponent->AddObject(object);
				}
			}
		}

		void	RemoveObject(const SocketAddress& address) noexcept
		{
			mObjects.erase(address);
			mNetworkComponent->RemoveObject(address);

			mObject->GetSharedContext()->mUIManager->GetLog()->Add("Disconnected");
		}

		const	std::unordered_map<SocketAddress, std::shared_ptr<T>>&	GetObjects()	const	noexcept { return mObjects; }

	
	private:
		void	Connect(InputMemoryBitStream& input, const SocketAddress& fromAddress, const std::shared_ptr<T>& object) noexcept
		{
			OutputMemoryBitStream outputStream = OutputMemoryBitStream();
			outputStream.Write(MessageType::CONNECT);
			outputStream.Write(fromAddress.GetHash());

			mNetworkComponent->SendPacket(outputStream, fromAddress);

			std::stringstream ss{};
			ss << "Connected (" << fromAddress.ToString() << '-' << fromAddress.GetHash() << ')';
			mObject->GetSharedContext()->mUIManager->GetLog()->Add(ss.str());
		}

		void	Disconnect(InputMemoryBitStream& input, const SocketAddress& fromAddress, const std::shared_ptr<T>& object) noexcept
		{
			this->RemoveObject(object->GetSocketAddress());
		}

		std::map<MessageType, std::function<void(BaseProcessingComponent&, InputMemoryBitStream&, const SocketAddress&, const  std::shared_ptr<T>&)>>	mProcessingStore;

		std::unordered_map<SocketAddress, std::shared_ptr<T>> mObjects;

		std::shared_ptr<NetworkComponent>	mNetworkComponent;

	};

	class TestProcessingComponent : public BaseProcessingComponent<JetbotObject>
	{
	public:
		TestProcessingComponent(Util::Object* owner) noexcept : BaseProcessingComponent<JetbotObject>(owner)
		{

		}
		virtual ~TestProcessingComponent() = default;

		void	OnCreate()	noexcept	override
		{
			BaseProcessingComponent::OnCreate();
		}
	};
};