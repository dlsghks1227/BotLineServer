#include "../framework.h"
#include "Log.h"


UI::Log::Log() noexcept :
	mIsAutoScoll(true)
{
	Clear();
}

void UI::Log::OnUpdate(const Util::Timer& timer) noexcept
{
}

void UI::Log::OnLateUpdate(const Util::Timer& timer) noexcept
{
}

void UI::Log::OnRender(const Util::Timer& timer) noexcept
{
	if (!ImGui::Begin("Log", 0, sWindowFlags))
	{
		ImGui::End();
		return;
	}

	bool isClear = ImGui::Button("Clear");

	ImGui::SameLine();
	ImGui::Checkbox("Auto-scroll", &mIsAutoScoll);

	ImGui::SameLine();
	mTextFilter.Draw("Filter", -50.0f);

	ImGui::Separator();
	ImGui::BeginChild("scrolling", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

	if (isClear)
	{
		Clear();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	const char* buf = mTextBuffer.begin();
	const char* bufEnd = mTextBuffer.end();

	if (mTextFilter.IsActive())
	{
		for (int lineNumber = 0; lineNumber < mLineOffsets.Size; lineNumber++)
		{
			const char* line_start = buf + mLineOffsets[lineNumber];
			const char* line_end = (lineNumber + 1 < mLineOffsets.Size) ? (buf + mLineOffsets[lineNumber + 1] - 1) : bufEnd;
			if (mTextFilter.PassFilter(line_start, line_end))
			{
				ImGui::TextUnformatted(line_start, line_end);
			}
		}
	}
	else
	{
		ImGuiListClipper clipper;
		clipper.Begin(mLineOffsets.Size);
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				const char* line_start = buf + mLineOffsets[line_no];
				const char* line_end = (line_no + 1 < mLineOffsets.Size) ? (buf + mLineOffsets[line_no + 1] - 1) : bufEnd;
				ImGui::TextUnformatted(line_start, line_end);
			}
		}
		clipper.End();
	}

	ImGui::PopStyleVar();

	// 자동 스크롤
	if (mIsAutoScoll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		ImGui::SetScrollHereY(1.0f);

	ImGui::EndChild();
	ImGui::End();
}

void UI::Log::Add(const std::string& text) noexcept
{
	int oldSize = this->mTextBuffer.size();

	std::stringstream ss;
	ss.precision(6);
	ss << "[" << std::setw(10) << std::right << ImGui::GetTime() << "s] " << text << '\n';
	mTextBuffer.append(ss.str().c_str());
	for (int newSize = mTextBuffer.size(); oldSize < newSize; oldSize++)
	{
		if (mTextBuffer[oldSize] == '\n')
		{
			mLineOffsets.push_back(oldSize + 1);
		}
	}
}

void UI::Log::Clear() noexcept
{
	mTextBuffer.clear();
	mLineOffsets.clear();
	mLineOffsets.push_back(0);
}