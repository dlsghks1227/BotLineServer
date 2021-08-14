#pragma once
#include "../framework.h"

namespace Component
{
	class NetworkComponent;
	class WebProcessingComponent : public Util::Component
	{
	public:
		WebProcessingComponent(Util::Object* owner)		noexcept;
		virtual ~WebProcessingComponent();

		using	JetbotObjectPtr = std::shared_ptr<JetbotObject>;

		static	constexpr	double	sUpdateCycle = 1.0;

		void		OnCreate()								noexcept	override;
		void		OnUpdate(const Util::Timer& timer)		noexcept	override;
		void		OnLateUpdate(const Util::Timer& timer)	noexcept	override;
		void		OnRender(const Util::Timer& timer)		noexcept	override;

		void		PacketProcessing(InputMemoryBitStream& input, const SocketAddress& fromAddress) noexcept;

	private:
		void		Connect(InputMemoryBitStream& input, const SocketAddress& fromAddress, const JetbotObjectPtr& object) noexcept;
		void		InformationRequest(InputMemoryBitStream& input, const SocketAddress& fromAddress, const JetbotObjectPtr& object) noexcept;

		void		UpdateJetbotInfomation() noexcept;

		std::map<MessageType, std::function<void(JetbotProcessingComponent&, InputMemoryBitStream&, const SocketAddress&, const JetbotObjectPtr&)>>	mProcessingStore;

		std::shared_ptr<DataComponent>		mDataComponent;
		std::shared_ptr<NetworkComponent>	mNetworkComponent;

		JetbotState							mJetbotState;
	};
};