#pragma once

namespace ImguiWindow
{
	class Log
	{
	public:
		Log()	noexcept;
		~Log() = default;

		void	Initialize() noexcept;
		void	AddLog(const std::string& log) noexcept;

		void	Draw(const std::string& title) noexcept;

	private:
		ImGuiTextBuffer		mTextBuffer;
		ImGuiTextFilter		mTextFilter;
		ImVector<int>		mLineOffsets;
		bool				mIsAutoScoll;
	};
};