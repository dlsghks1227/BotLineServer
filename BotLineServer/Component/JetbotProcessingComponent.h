#pragma once
#include "../framework.h"

namespace Component
{
	class JetbotProcessingComponent : public Util::Component
	{
	public:
		JetbotProcessingComponent(Util::Object* owner)		noexcept;
		virtual ~JetbotProcessingComponent();

		void		OnCreate()								noexcept	override;
		void		OnUpdate(const Util::Timer& timer)		noexcept	override;
		void		OnLateUpdate(const Util::Timer& timer)	noexcept	override;
		void		OnRender(const Util::Timer& timer)		noexcept	override;

	private:
	};
};