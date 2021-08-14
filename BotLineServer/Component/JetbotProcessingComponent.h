#pragma once
#include "../framework.h"

namespace Component
{
	class NetworkComponent;
	class JetbotProcessingComponent : public BaseProcessingComponent<JetbotObject>
	{
	public:
		JetbotProcessingComponent(Util::Object* owner)		noexcept;
		virtual ~JetbotProcessingComponent();

		static	constexpr	double	sUpdateCycle = 1.0;

		void		OnCreate()								noexcept	override;
		void		OnUpdate(const Util::Timer& timer)		noexcept	override;
		void		OnLateUpdate(const Util::Timer& timer)	noexcept	override;
		void		OnRender(const Util::Timer& timer)		noexcept	override;

	private:
		void		InformationRequest(InputMemoryBitStream& input, const SocketAddress& fromAddress, const JetbotObjectPtr& object) noexcept;

		void		UpdateJetbotInfomation() noexcept;
	};
};