#pragma once
#include "../framework.h"

namespace UI
{
	class Log;
	class UIManager
	{
	public:
		UIManager() noexcept;
		~UIManager();

		UIManager(UIManager&&) = default;
		UIManager& operator = (UIManager&&) = default;

		UIManager(const UIManager&) = delete;
		UIManager& operator = (const UIManager&) = delete;

		void	OnUpdate(const Util::Timer& timer)		noexcept;
		void	OnLateUpdate(const Util::Timer& timer)	noexcept;
		void	OnRender(const Util::Timer& timer)		noexcept;

		auto	GetLog()	noexcept { return mLog.get(); }

	private:
		void	Add(const std::shared_ptr<UIBase>& ui) noexcept;

		std::vector<std::shared_ptr<UIBase>>	mUIs;
		std::shared_ptr<Log>					mLog;
	};
};