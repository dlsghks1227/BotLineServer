#include "framework.h"
#include "Log.h"

ImguiWindow::Log::Log() noexcept
{
	mIsAutoScoll = true;
	Initialize();
}

void ImguiWindow::Log::Initialize() noexcept
{
	mTextBuffer.clear();
	mLineOffsets.clear();
	mLineOffsets.push_back(0);
}

void ImguiWindow::Log::AddLog(const std::string& log) noexcept
{
	int	oldSize = mTextBuffer.size();
	mTextBuffer.append(log.c_str());
	for (int newSize = mTextBuffer.size(); oldSize < newSize; oldSize++)
	{
		if (mTextBuffer[oldSize] == '\n')
		{
			mLineOffsets.push_back(oldSize + 1);
		}
	}
}

void ImguiWindow::Log::Draw(const std::string& title) noexcept
{
	if (!ImGui::Begin(title.c_str()))
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
	ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

	if (isClear)
	{
		Initialize();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

	const char* buf		= mTextBuffer.begin();
	const char* bufEnd	= mTextBuffer.end();

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
