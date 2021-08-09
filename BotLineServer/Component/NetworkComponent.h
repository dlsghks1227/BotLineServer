#pragma once
#include "../framework.h"

namespace Component
{
	class NetworkComponent : public Util::Component
	{
	public:
		NetworkComponent(Util::Object* owner)		noexcept;
		virtual ~NetworkComponent();

		static constexpr		uint16_t	sPort		= 8000;
		static constexpr		uint32_t	sBufferSize = 2048;
		static constexpr		double		sTimeout	= 5.0;

		void		OnCreate()								noexcept	override;
		void		OnUpdate(const Util::Timer& timer)		noexcept	override;
		void		OnLateUpdate(const Util::Timer& timer)	noexcept	override;
		void		OnRender(const Util::Timer& timer)		noexcept	override;
	private:
	};
};