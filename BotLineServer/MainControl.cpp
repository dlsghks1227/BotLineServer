#include "framework.h"
#include "MainControl.h"

ImguiWindow::MainControl::MainControl() noexcept
{
}

void ImguiWindow::MainControl::Initialize(const std::shared_ptr<UDPSocket>& socket) noexcept
{
	mSocket = socket;
}

void ImguiWindow::MainControl::UpdateObjects(const std::unordered_map<SocketAddress, std::shared_ptr<JetbotObject>>& objects) noexcept
{
	mJetbotObjects = objects;
}

void ImguiWindow::MainControl::Draw() noexcept
{
	if (!ImGui::Begin("Main Control"))
	{
		ImGui::End();
		return;
	}
	static bool isAllStop = true;
	if (ImGui::Button("All Stop Test"))
	{
		isAllStop = !isAllStop;
		for (const auto& pair : mJetbotObjects)
		{
			OutputMemoryBitStream output;
			output.Write(MessageType::ALL_STOP);
			output.Write(static_cast<uint8_t>(isAllStop));
			mSocket->SendTo(output.GetBufferPtr(), output.GetByteLength(), pair.first);
		}
	}


	ImGui::End();
}
