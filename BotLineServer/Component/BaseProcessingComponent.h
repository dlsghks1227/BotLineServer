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

			mProcessingStorage.insert({MessageType::CONNECT,	std::bind(&BaseProcessingComponent::Connect,	this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
			mProcessingStorage.insert({MessageType::DISCONNECT,	std::bind(&BaseProcessingComponent::Disconnect,	this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
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

				const auto func = mProcessingStorage.find(messageType);
				if (func != mProcessingStorage.cend())
				{
					obj->second->SetLastMessageType(messageType);
					func->second(input, fromAddress, obj->second);
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

	protected:
		std::map<MessageType, std::function<void(InputMemoryBitStream&, const SocketAddress&, const std::shared_ptr<T>&)>>	mProcessingStorage;

		std::unordered_map<SocketAddress, std::shared_ptr<T>>	mObjects;
		std::shared_ptr<NetworkComponent>						mNetworkComponent;
	};
};