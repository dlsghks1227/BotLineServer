#pragma once
#include "framework.h"

#include "Dialog.h"

namespace Dialog
{
	class Dialog;
	class Component
	{
	public:
		Component(const std::shared_ptr<Dialog>& owner) noexcept :
			mDialog(owner) {}
		virtual ~Component() = default;

		virtual void	Initialize()								noexcept {};

		virtual void	OnUpdate(const Utility::Timer& timer)		noexcept {};
		virtual void	OnLateUpdate(const Utility::Timer& timer)	noexcept {};

		virtual	void	OnRender(const Utility::Timer& timer)		noexcept {};

	protected:
		std::shared_ptr<Dialog>		mDialog;
	};
}