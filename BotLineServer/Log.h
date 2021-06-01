#pragma once

namespace ImguiWindow
{
	class Log
	{
	public:
		Log()	noexcept;
		~Log() = default;

		static	constexpr	ImGuiWindowFlags	sWindowFlags = ImGuiWindowFlags_NoResize;

		void	Initialize()	noexcept;

		void	Clear()							noexcept;
		void	Add(const std::string& log)		noexcept;
		void	Draw(const std::string& title)	noexcept;

	private:
		ImGuiTextBuffer		mTextBuffer;
		ImGuiTextFilter		mTextFilter;
		ImVector<int>		mLineOffsets;
		bool				mIsAutoScoll;
	};
};