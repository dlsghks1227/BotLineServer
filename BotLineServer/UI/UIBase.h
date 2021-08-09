#pragma once
#include "../framework.h"

namespace UI
{
	__interface UIBase
	{
		void	OnUpdate(const Util::Timer& timer)		noexcept;
		void	OnLateUpdate(const Util::Timer& timer)	noexcept;
		void	OnRender(const Util::Timer& timer)		noexcept;
	};
}