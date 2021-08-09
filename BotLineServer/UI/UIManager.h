#pragma once
#include "../framework.h"

namespace UI
{
	class UIManager final
	{
	public:
		UIManager() noexcept;
		~UIManager();

		UIManager(UIManager&&) = default;
		UIManager& operator = (UIManager&&) = default;

		UIManager(const UIManager&) = delete;
		UIManager& operator = (const UIManager&) = delete;

		void			OnUpdate(const Util::Timer& timer)		noexcept;
		void			OnLateUpdate(const Util::Timer& timer)	noexcept;
		void			OnRender(const Util::Timer& timer)		noexcept;

	private:
		void			Add(const std::shared_ptr<UI::UIBase>& ui) noexcept;

		std::vector<std::shared_ptr<UI::UIBase>>		mUIs;
	};
};