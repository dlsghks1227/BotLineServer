#pragma once
#include "../framework.h"

namespace UI
{
	class Log : public UIBase
	{
	public:
		Log() noexcept;
		virtual ~Log() = default;

		static	constexpr	ImGuiWindowFlags	sWindowFlags = ImGuiWindowFlags_NoResize;

		void	OnUpdate(const Util::Timer& timer)		noexcept	override;
		void	OnLateUpdate(const Util::Timer& timer)	noexcept	override;
		void	OnRender(const Util::Timer& timer)		noexcept	override;

		void	Add(const std::string& text)			noexcept;
	private:
		void	Clear()		noexcept;

		ImGuiTextBuffer		mTextBuffer;
		ImGuiTextFilter		mTextFilter;
		ImVector<int>		mLineOffsets;
		bool				mIsAutoScoll;
	};
};