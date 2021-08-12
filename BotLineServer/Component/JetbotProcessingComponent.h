#pragma once
#include "../framework.h"

namespace Component
{
	class NetworkComponent;
	class JetbotProcessingComponent : public Util::Component
	{
	public:
		JetbotProcessingComponent(Util::Object* owner)		noexcept;
		virtual ~JetbotProcessingComponent();

		using	JetbotObjectPtr = std::shared_ptr<JetbotObject>;

		void		OnCreate()								noexcept	override;
		void		OnUpdate(const Util::Timer& timer)		noexcept	override;
		void		OnLateUpdate(const Util::Timer& timer)	noexcept	override;
		void		OnRender(const Util::Timer& timer)		noexcept	override;

		void		PacketProcessing(InputMemoryBitStream& input, const SocketAddress& fromAddress);

	private:
		void		Connect(InputMemoryBitStream& input, const SocketAddress& fromAddress);

		std::unordered_map<SocketAddress, JetbotObjectPtr>	mJetbotObjects;

		std::map<MessageType, std::function<void(JetbotProcessingComponent&, InputMemoryBitStream&, const SocketAddress&)>>	mProcessingStore;
	};
};