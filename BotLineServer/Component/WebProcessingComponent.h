#pragma once
#include "../framework.h"

namespace Component
{
	class NetworkComponent;
	class WebProcessingComponent : public BaseProcessingComponent<WebObject>
	{
	public:
		WebProcessingComponent(Util::Object* owner)		noexcept;
		virtual ~WebProcessingComponent();

		static	constexpr	double	sUpdateCycle = 0.1f;

		void		OnCreate()								noexcept	override;
		void		OnUpdate(const Util::Timer& timer)		noexcept	override;
		void		OnLateUpdate(const Util::Timer& timer)	noexcept	override;
		void		OnRender(const Util::Timer& timer)		noexcept	override;

	private:
		void		InfoCurrentState(InputMemoryBitStream& input, const SocketAddress& fromAddress, const WebObjectPtr& object) noexcept;
	};
};