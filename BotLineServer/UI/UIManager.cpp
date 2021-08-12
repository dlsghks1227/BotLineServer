#include "../framework.h"
#include "UIManager.h"

UI::UIManager::UIManager() noexcept
{
	mLog = std::make_shared<UI::Log>();
	this->Add(mLog);
}

UI::UIManager::~UIManager()
{
}

void UI::UIManager::OnUpdate(const Util::Timer& timer) noexcept
{
	for (const auto& itr : mUIs)
	{
		itr->OnUpdate(timer);
	}
}

void UI::UIManager::OnLateUpdate(const Util::Timer& timer) noexcept
{
	for (const auto& itr : mUIs)
	{
		itr->OnLateUpdate(timer);
	}
}

void UI::UIManager::OnRender(const Util::Timer& timer) noexcept
{
	for (const auto& itr : mUIs)
	{
		itr->OnRender(timer);
	}
}

void UI::UIManager::Add(const std::shared_ptr<UI::UIBase>& ui) noexcept
{
	mUIs.push_back(ui);
}
