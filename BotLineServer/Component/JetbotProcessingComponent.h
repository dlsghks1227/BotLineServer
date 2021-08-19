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

		void		JetbotChangePosition(const SocketAddress& address, const Position& pos)	noexcept;
		void		JetbotArrived(const SocketAddress& address) noexcept;

	private:
		void		InfoCurrentState(InputMemoryBitStream& input, const SocketAddress& fromAddress, const JetbotObjectPtr& object) noexcept;

		void		UpdateCurrentState() noexcept;
	};
};